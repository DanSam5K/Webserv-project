#include "DeleteActionResponse.hpp"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cstdio>

// Constructors
// Expected when implemented expected

DeleteActionResponse::DeleteActionResponse() {}

DeleteActionResponse::DeleteActionResponse(ServerConfiguration* serverBlock) {
    _rootFolder = serverBlock->getDocumentRoot();
    _host = serverBlock->getHostname();
}

DeleteActionResponse::~DeleteActionResponse() {}


// Helper Function

static std::string extractImageIdFromPath(const std::string& resourcePath) {
    std::string::size_type lastSlash = resourcePath.rfind('/');
    std::string::size_type extensionPos = resourcePath.find(".jpeg", lastSlash);

    if (lastSlash != std::string::npos && extensionPos != std::string::npos) {
        return resourcePath.substr(lastSlash + 1, extensionPos - lastSlash - 1);
    }
    return "";
}


// Core DELETE logic

void DeleteResponse::executeDeleteResponse(Request& request) {
    const std::string tempFilePath = "tmp/delete-entry.txt";
    const std::string resourceId = extractImageIdFromPath(_resource);

    // Write DELETE parameters to temporary file
    std::ofstream tempFile(tempFilePath.c_str(), std::ios::out);
    if (!tempFile.is_open()) {
        std::cerr << "Failed to open temp file for DELETE operation." << std::endl;
        return;
    }

    tempFile << "_method=DELETE&id=" + resourceId;
    tempFile.close();

    // Execute the CGI PHP script
    std::string command = "php cgi-bin/delete-entry.php " + tempFilePath;
    std::system(command.c_str());

    // Attempt to delete the actual resource file
    const std::string imagePath = _rootFolder + "/images/" + resourceId + ".jpeg";
    if (std::remove(imagePath.c_str()) != 0) {
        std::cerr << "Warning: failed to remove image file at path: " << imagePath << std::endl;
    }

    // Clean up temporary request body file
    if (std::remove(tempFilePath.c_str()) != 0) {
        std::cerr << "Warning: failed to remove temp file at path: " << tempFilePath << std::endl;
    }
}


// Header Setup

void DeleteResponse::setLocationHeader() {
    _headers.insert(std::make_pair("Location", "/"));
}

void DeleteResponse::setHeaders() {
    setDateHeader();
    setConnectionHeader("close");
    setLocationHeader();
    setCacheControl("no-cache");
    setHost(_host.c_str());

    // Extend with more headers as needed
}


// Full Response Construction

void DeleteResponse::constructResponse(Request& request) {
    setResource(request);

    // Set status line
    _raw_status_line = _http_version + " 204 No Content\r\n";

    // Handle the DELETE operation
    executeDeleteResponse(request);

    // Configure headers and body
    setHeaders();
    setRawHeaders();

    // Finalize full response
    setRawResponse();

    std::cout << "Generated DELETE response:\n"
              << _raw_status_line << _raw_headers << _raw_body << std::endl;
}

// Stub Methods (no-op / placeholder)

void DeleteResponse::constructDefaultResponseWithBody(Request& request, const std::string& bodyContent) {
    (void)request;
    (void)bodyContent;
}

void DeleteResponse::constructConfigurationResponse(Request& request, const std::string& filePath) {
    (void)request;
    (void)filePath;
}