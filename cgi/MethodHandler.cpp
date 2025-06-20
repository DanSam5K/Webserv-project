/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:59:58 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/20 10:37:55 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodHandler.hpp"

bool ends_with(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

MethodHandler::MethodHandler()
{
    _errorPages.setErrorPage(404, "../public/www/error/404.html");
    _errorPages.setErrorPage(401, "../public/www/error/401.html");
    _errorPages.setErrorPage(500, "../public/www/error/500.html");
}

std::string MethodHandler::handleRequest(const std::string& method,
                                         const std::string& path,
                                         const std::string& body,
                                         const std::string& contentType,
                                         const std::string& transferEncoding,
                                         int& responseCode)
{
    if (method == "GET") {
        return handleGet(path, responseCode);
    } else if (method == "POST") {
        return handlePost(path, body, contentType, transferEncoding, responseCode);
    } else if (method == "DELETE") {
        return handleDelete(path, responseCode);
    } else {
        responseCode = 405;
        return _errorPages.getErrorResponse(405);
    }
}

//added implementation for Index fallback if the path is a directory (e.g., /public/ serves /public/index.html).
//And Directory listing if no index is found and listing is allowed.
std::string MethodHandler::handleGet(const std::string& path, int& responseCode)
{
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0)
    {
        responseCode = 404;
        return _errorPages.getErrorResponse(404);
    }

    // If it's a directory
    if (S_ISDIR(pathStat.st_mode))
    {
        std::string indexPath = path + "/index.html";
        std::ifstream indexFile(indexPath.c_str());
        if (indexFile.is_open())
        {
            std::stringstream buffer;
            buffer << indexFile.rdbuf();
            indexFile.close();
            responseCode = 200;
            return buffer.str();
        } else {
            // Generate directory listing
            DIR* dir = opendir(path.c_str());
            if (!dir)
            {
                responseCode = 403;
                return _errorPages.getErrorResponse(403);
            }

            std::ostringstream html;
            html << "<html><head><title>Directory listing for " << path
                 << "</title></head><body><h1>Index of " << path << "</h1><ul>";

            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL)
            {
                std::string name = entry->d_name;
                if (name == ".") continue;
                html << "<li><a href=\"" << name << "\">" << name << "</a></li>";
            }

            html << "</ul></body></html>";
            closedir(dir);
            responseCode = 200;
            return html.str();
        }
    }

    // If it's a regular file
    if (S_ISREG(pathStat.st_mode))
    {
        std::ifstream file(path.c_str());
        if (!file.is_open())
        {
            responseCode = 403;
            return _errorPages.getErrorResponse(403);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        responseCode = 200;
        return buffer.str();
    }

    // Anything else
    responseCode = 403;
    return _errorPages.getErrorResponse(403);
}

// std::string MethodHandler::handleGet(const std::string& path, int& responseCode) {
//     if (ends_with(path, ".py") || ends_with(path, ".php")) {
//         CGIHandler cgi(path, "GET", "", "");
//         std::string output = cgi.execute();
//         responseCode = 200;
//         return output;
//     }

//     std::ifstream file(path.c_str());
//     if (!file.is_open()) {
//         responseCode = 404;
//         return _errorPages.getErrorResponse(404);
//     }

//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     file.close();

//     responseCode = 200;
//     return buffer.str();
// }

std::string MethodHandler::handlePost(const std::string& path,
                                      const std::string& rawBody,
                                      const std::string& contentType,
                                      const std::string& transferEncoding,
                                      int& responseCode) {
    std::string decodedBody = rawBody;

    if (transferEncoding == "chunked") {
        ChunkedDecoder decoder;
        decodedBody = decoder.decode(rawBody);
    }

    if (ends_with(path, ".py") || ends_with(path, ".php")) {
        CGIHandler cgi(path, "POST", decodedBody);
        std::string output = cgi.execute();
        responseCode = 200;
        return output;
    }

    if (contentType.find("multipart/form-data") != std::string::npos) {
        size_t pos = contentType.find("boundary=");
        if (pos == std::string::npos) {
            responseCode = 400;
            return "Error 400: Missing boundary in Content-Type\n";
        }

        std::string boundary = contentType.substr(pos + 9);
        MultipartParser parser(decodedBody, boundary);
        parser.parse();

        UploadHandler uploader("upload");
        if (!uploader.saveAll(parser.getParsedFiles())) {
            responseCode = 500;
            return _errorPages.getErrorResponse(500);
        }

        responseCode = 201;
        return "Upload successful\n";
    }

    // Regular plain-text POST
    std::ofstream file("./uploads/uploaded_file.txt");
    if (!file.is_open()) {
        responseCode = 500;
        return _errorPages.getErrorResponse(500);
    }

    file << decodedBody;
    file.close();

    responseCode = 201;
    return "File uploaded successfully\n";
}

std::string MethodHandler::handleDelete(const std::string& path, int& responseCode)
{
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) != 0) {
        responseCode = 404;
        return _errorPages.getErrorResponse(404);
    }

    if (!S_ISREG(fileStat.st_mode)) {
        responseCode = 403;
        return "Error 403: Not a regular file\n";
    }

    if (access(path.c_str(), W_OK) != 0) {
        responseCode = 403;
        return "Error 403: Permission denied\n";
    }

    if (unlink(path.c_str()) != 0) {
        responseCode = 500;
        return _errorPages.getErrorResponse(500);
    }

    responseCode = 204; // No Content
    return "";
}




//For main server loop or request dispatcher 
/*MethodHandler handler;

int responseCode;
std::string response = handler.handleRequest(request.method, request.filePath, request.body, responseCode); */

// Use response and responseCode to generate full HTTP reply
