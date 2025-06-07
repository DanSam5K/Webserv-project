/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:51:52 by remijacobs        #+#    #+#             */
/*   Updated: 2025/05/18 16:21:41 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler(const std::string& scriptPath,
                       const std::string& method,
                       const std::string& body,
                       const std::string& queryString)
    : _scriptPath(scriptPath), _method(method), _body(body), _queryString(queryString) {}

std::vector<std::string> CGIHandler::createEnv() {
    std::vector<std::string> env;

    env.push_back("REQUEST_METHOD=" + _method);
    env.push_back("SCRIPT_NAME=" + _scriptPath);
    env.push_back("PATH_INFO=" + _scriptPath);

    if (_method == "POST") {
        std::ostringstream oss;
        oss << "CONTENT_LENGTH=" << _body.length();
        env.push_back(oss.str());
    } else if (_method == "GET") {
        env.push_back("QUERY_STRING=" + _queryString);
    }

    return env;
}

char** CGIHandler::convertEnv(const std::vector<std::string>& env) {
    char** envp = new char*[env.size() + 1];
    for (size_t i = 0; i < env.size(); ++i) {
        envp[i] = strdup(env[i].c_str());
    }
    envp[env.size()] = NULL;
    return envp;
}

std::string CGIHandler::execute() {
    std::vector<std::string> env = createEnv();
    char** envp = convertEnv(env);

    int outputPipe[2];
    int inputPipe[2];
    bool isPost = (_method == "POST");

    if (pipe(outputPipe) < 0) throw std::runtime_error("Failed to create output pipe");
    if (isPost && pipe(inputPipe) < 0) throw std::runtime_error("Failed to create input pipe");

    pid_t pid = fork();
    if (pid < 0) throw std::runtime_error("Fork failed");

    if (pid == 0) {
        // Child process
        dup2(outputPipe[1], STDOUT_FILENO);
        close(outputPipe[0]);
        close(outputPipe[1]);

        if (isPost) {
            dup2(inputPipe[0], STDIN_FILENO);
            close(inputPipe[0]);
            close(inputPipe[1]);
        }

        char* argv[] = { const_cast<char*>(_scriptPath.c_str()), NULL };
        execve(argv[0], argv, envp);
        perror("execve failed");
        exit(1);
    }

    // Parent
    close(outputPipe[1]);
    if (isPost) {
        close(inputPipe[0]);
        write(inputPipe[1], _body.c_str(), _body.size());
        close(inputPipe[1]);
    }

    char buffer[4096];
    std::string output;
    ssize_t bytesRead;

    while ((bytesRead = read(outputPipe[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, bytesRead);
    }
    close(outputPipe[0]);

    int status;
    waitpid(pid, &status, 0);

    // Clean up envp
    for (size_t i = 0; envp[i] != NULL; ++i)
        free(envp[i]);
    delete[] envp;

    return output;
}
