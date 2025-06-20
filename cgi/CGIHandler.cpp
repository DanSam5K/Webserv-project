/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:51:52 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/20 10:31:48 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"


//Stand Alone implementation
// CGIHandler::CGIHandler(const std::string& scriptPath,
//                        const std::string& method,
//                        const std::string& body,
//                        const std::string& queryString)
//     : _scriptPath(scriptPath), _method(method), _body(body), _queryString(queryString) {}

// std::vector<std::string> CGIHandler::createEnv() {
//     std::vector<std::string> env;

//     env.push_back("REQUEST_METHOD=" + _method);
//     env.push_back("SCRIPT_NAME=" + _scriptPath);
//     env.push_back("PATH_INFO=" + _scriptPath);

//     if (_method == "POST") {
//         std::ostringstream oss;
//         oss << "CONTENT_LENGTH=" << _body.length();
//         env.push_back(oss.str());
//     } else if (_method == "GET") {
//         env.push_back("QUERY_STRING=" + _queryString);
//     }

//     return env;
// }

// char** CGIHandler::convertEnv(const std::vector<std::string>& env) {
//     char** envp = new char*[env.size() + 1];
//     for (size_t i = 0; i < env.size(); ++i) {
//         envp[i] = strdup(env[i].c_str());
//     }
//     envp[env.size()] = NULL;
//     return envp;
// }

// std::string CGIHandler::execute() {
//     std::vector<std::string> env = createEnv();
//     char** envp = convertEnv(env);

//     int outputPipe[2];
//     int inputPipe[2];
//     bool isPost = (_method == "POST");

//     if (pipe(outputPipe) < 0) throw std::runtime_error("Failed to create output pipe");
//     if (isPost && pipe(inputPipe) < 0) throw std::runtime_error("Failed to create input pipe");

//     pid_t pid = fork();
//     if (pid < 0) throw std::runtime_error("Fork failed");

//     if (pid == 0) {
//         // Child process
//         dup2(outputPipe[1], STDOUT_FILENO);
//         close(outputPipe[0]);
//         close(outputPipe[1]);

//         if (isPost) {
//             dup2(inputPipe[0], STDIN_FILENO);
//             close(inputPipe[0]);
//             close(inputPipe[1]);
//         }

//         char* argv[] = { const_cast<char*>(_scriptPath.c_str()), NULL };
//         execve(argv[0], argv, envp);
//         perror("execve failed");
//         exit(1);
//     }

//     // Parent
//     close(outputPipe[1]);
//     if (isPost) {
//         close(inputPipe[0]);
//         write(inputPipe[1], _body.c_str(), _body.size());
//         close(inputPipe[1]);
//     }

//     char buffer[4096];
//     std::string output;
//     ssize_t bytesRead;

//     while ((bytesRead = read(outputPipe[0], buffer, sizeof(buffer))) > 0) {
//         output.append(buffer, bytesRead);
//     }
//     close(outputPipe[0]);

//     int status;
//     waitpid(pid, &status, 0);

//     // Clean up envp
//     for (size_t i = 0; envp[i] != NULL; ++i)
//         free(envp[i]);
//     delete[] envp;

//     return output;
// }

CGIHandler::CGIHandler(const std::string& scriptPath,
    const std::string& method,
    const std::string& body,
    const std::string& contentType,
    const std::string& queryString,
    const std::string& uploadPath)
: _scriptPath(scriptPath), _method(method), _body(body),
_contentType(contentType), _queryString(queryString), _uploadPath(uploadPath) {}

std::vector<std::string> CGIHandler::createEnv() 
{
    std::vector<std::string> env;
    env.push_back("REQUEST_METHOD=" + _method);
    env.push_back("SCRIPT_FILENAME=" + _scriptPath);
    if (!_queryString.empty()) env.push_back("QUERY_STRING=" + _queryString);
    if (!_contentType.empty()) env.push_back("CONTENT_TYPE=" + _contentType);
    if (!_uploadPath.empty()) env.push_back("UPLOAD_PATH=" + _uploadPath);
    env.push_back("CONTENT_LENGTH=" + std::to_string(_body.size()));
    return env;
}

char** CGIHandler::convertEnv(const std::vector<std::string>& env) 
{
    char** envp = new char*[env.size() + 1];
    for (size_t i = 0; i < env.size(); ++i) {
    envp[i] = strdup(env[i].c_str());
    }
    envp[env.size()] = NULL;
    return envp;
}

void CGIHandler::cleanupEnv(char** env) 
{
    for (int i = 0; env[i] != NULL; ++i)
    {
        free(env[i]);
    }
    delete[] env;
}

std::string CGIHandler::execute()
{
    int inPipe[2], outPipe[2];
    pipe(inPipe);
    pipe(outPipe);

    pid_t pid = fork();
    if (pid == 0)
    {
    dup2(inPipe[0], STDIN_FILENO);
    dup2(outPipe[1], STDOUT_FILENO);
    close(inPipe[1]);
    close(outPipe[0]);

    std::vector<std::string> envVector = createEnv();
    char** envp = convertEnv(envVector);

    char* const args[] = {const_cast<char*>(_scriptPath.c_str()), NULL};
    execve(_scriptPath.c_str(), args, envp);

    perror("execve");
    exit(1);
    }

    // Parent process
    close(inPipe[0]);
    close(outPipe[1]);
    write(inPipe[1], _body.c_str(), _body.size());
    close(inPipe[1]);

    char buffer[1024];
    std::stringstream response;
    ssize_t n;
    while ((n = read(outPipe[0], buffer, sizeof(buffer))) > 0) {
    response.write(buffer, n);
    }
    close(outPipe[0]);

    int status;
    waitpid(pid, &status, 0);

    return response.str();
}
