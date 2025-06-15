#include "GetActionResponse.hpp"

#include <fstream>
#include <cstdio>
#include <string>
#include <iterator>

// Utility function to check if a filename has a specific file extension
bool hasFileExtension(const std::string& filename, const std::string& extension) {
    //proper implmemntation of hasFileExtension needed
    if (extension.empty() || filename.empty()) {
        return false; // If either is empty, return false
    }
    // Check if the filename ends with the given extension
    // This checks if the filename is at least as long as the extension
    // and compares the end of the filename with the extension
    // This is a simple implementation that checks the end of the filename
    // for the extension, ensuring case sensitivity.

    return filename.size() >= extension.size() &&
           filename.compare(filename.size() - extension.size(), extension.size(), extension) == 0;
}

// utilty function



// Constructors
GetActionResponse::GetActionResponse() {}

GetActionResponse::GetActionResponse(ServerConfiguration* serverBlock) {
    _host = serverBlock->getHostname();
}

GetActionResponse::~GetActionResponse() {}

// Utility function to execute a PHP file and capture its output
static std::string executePhpScript(const std::string& scriptPath) {
    std::string command = "php " + scriptPath;
    std::string output;

    FILE* pipe = popen(command.c_str(), "r");
    if (pipe) {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), pipe)) {
            output += buffer;
        }
        pclose(pipe);
    }
    return output;
}

// Loads the content of a file or processes a PHP script if applicable
void GetActionResponse::setRawBody() {
    std::ifstream fileStream(_resource.c_str());
    std::string fileContent((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    if (hasFileExtension(_resource, ".html") || hasFileExtension(_resource, ".php")) {
        _raw_body = executePhpScript(_resource);
    } else {
        _raw_body = fileContent;
    }

    setContentLengthHeader();
}

// Sets the required HTTP headers for the response
void GetActionResponse::setHeaders() {
    setDateHeader();
    setConnectionHeader("keep-alive");
    setContentTypeHeader();
    setRetryAfter(2); // Optional retry header
    setHost(_host.c_str());
}

// Builds a complete HTTP response based on the GET request
void GetActionResponse::constructResponse(Request& request) {
    setResource(request);
    _raw_status_line = _http_version + " 200 OK\r\n";

    setHeaders();
    setRawBody();
    setRawHeaders();
    setRawResponse();
}

// Placeholder for default error response with custom body
void GetActionResponse::constructDefaultResponseWithBody(Request& request, const std::string& rawBody) {
    (void)request;
    (void)rawBody;
}

// Placeholder for serving a configured response from a file path
void GetActionResponse::constructConfigResponse(Request& request, const std::string& filePath) {
    (void)request;
    (void)filePath;
}