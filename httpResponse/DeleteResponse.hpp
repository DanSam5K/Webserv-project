#ifndef DELETE_RESPONSE_HPP
#define DELETE_RESPONSE_HPP

// #include "Response.hpp"
#include "../config/ServerConfiguration.hpp"
#include <string>

/*
    A server's response to a DELETE request can result in:

        - 204: No Content
        - 302: Found
        - 400: Bad Request
        - 411: Length Required
        - 413: Payload Too Large
        - 415: Unsupported Media Type
        - 500: Internal Server Error
        - 505: HTTP Version Not Supported

    Example:
        HTTP/1.1 204 No Content\r\n
        Server: MyServer\r\n
        Date: Mon, 14 Jan 2023 12:00:00 GMT\r\n
        Content-Length: 0\r\n
        \r\n
*/
// Forward declarations to avoid circular dependencies
// need to remove and header reference included
class Request
{
    // Assume Request class is defined elsewhere
    public:
        std::string getRawBody();
        std::string getURI() const;
};

class Response{
    protected:
        std::string _raw_status_line;
        std::string _raw_headers;
        std::string _raw_body;
        std::string _http_version = "HTTP/1.1";
        std::string _resource;
        HttpResponseStatus _status_code;
        std::map<std::string, std::string> _headers;

        void setDateHeader(){};
        void setConnectionHeader(const std::string& connectionType){};
        void setCacheControl(const std::string& cacheControl){};
        void setHost(const char* host){};
        void setRawHeaders() {};
        void setRawResponse() {};
        void setResource(const Request& request) {};
        void setContentLengthHeader() {};
        void setContentTypeHeader() {};
        void setRetryAfter(int seconds) {};


    public:
        virtual ~Response() = default;

        // Setters
        void setHttpResponseStatusCode(HttpResponseStatus status) {
            _status_code = status;
        }

        void printResponse() const {}
        
}; // Forward declaration to avoid circular dependency


class DeleteResponse : public Response 
{    
    private:
        // Internal Helpers
        void setHeaders();
        void setRawBody();
        void setLocationHeader();

        // Internal State
        std::string _rootFolder;
        std::string _host;
    
    public:
        // Constructors / Destructor
        DeleteResponse();
        DeleteResponse(ServerConfiguration* serverBlock);
        virtual ~DeleteResponse();

        // Core Response Builders
        void constructResponse(Request& request);
        void constructDefaultResponseWithBody(Request& request, const std::string& bodyContent);
        void constructConfigurationResponse(Request& request, const std::string& targetFilePath);

        // File Handling Logic
        void executeDeleteResponse(Request& request);

        // Utility & Debugging
        void setHttpResponseStatusCode(HttpResponseStatus statusCode);
        void printResponse() const;

};

#endif // DELETE_RESPONSE_HPP