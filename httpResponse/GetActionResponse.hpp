/*
A client sends an HTTP GET request to retrieve a resource from the server.
The server responds with a status line, headers, and optionally a message body.

Example Response:
    HTTP/1.1 200 OK\r\n
    Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n
    Server: Apache\r\n
    Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n
    ETag: "34aa387-d-1568eb00"\r\n
    Accept-Ranges: bytes\r\n
    Content-Length: 14\r\n
    Vary: Accept-Encoding\r\n
    Content-Type: text/plain\r\n
    \r\n
    Hello World!
*/

#ifndef GET_ACTION_RESPONSE_HPP
#define GET_ACTION_RESPONSE_HPP

#include "DeleteActionResponse.hpp"
#include <string>

class GetActionResponse : public Response {
private:
    std::string _host;

    // Internal helpers
    void setHeaders();
    void setRawBody();

public:
    // Constructors & Destructor
    GetActionResponse();
    explicit GetActionResponse(ServerConfiguration* serverBlock);
    virtual ~GetActionResponse();

    // Core methods
    void constructResponse(Request& request);
    void constructDefaultResponseWithBody(Request& request, const std::string& rawBody);
    void constructConfigResponse(Request& request, const std::string& filePath);
    void printResponse();
    void setStatusCode(HttpResponseStatus statusCode);
};

#endif // CLASS_GET_RESPONSE_HPP