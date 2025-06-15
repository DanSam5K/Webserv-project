// Constructors
// FailActionResponse::FailActionResponse() {

// }

// FailActionResponse::~FailActionResponse() {

// }

// void    FailActionResponse::setLocationHeader() {
//     std::stringstream ss;

//     std::string location = "/";
//     _headers.insert(std::make_pair("Location", location));
// }

// void    FailActionResponse::setHeaders() {
//     setDateHeader();
//     setContentTypeHeader();
//     if (_raw_status_line.find("Found") != std::string::npos)
//         setLocationHeader();
//     // add more headers if desired below...
//     // add here...
// }
// void    FailActionResponse::setRawBody() {
//     std::ifstream   file(_resource.c_str());
//     std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

//     std::cout << _resource << std::endl;
//     _raw_body = content;
//     setContentLengthHeader();
//     // std::cout << "raw body after processing is : " << _raw_body << std::endl;
// }

// void	FailActionResponse::constructResponse(Request& req) {
// 	(void)req;
// }

// void    FailActionResponse::constructDefaultResponseWithBody(Request& req, std::string raw_body) {
//     this->_status_code = HTTP_VERSION_NOT_SUPPORTED;
// 	switch (req.getStatusCode()) {
// 		case HTTP_VERSION_NOT_SUPPORTED:
// 			_raw_status_line = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
// 			break;
// 		case HTTP_PAYLOAD_TOO_LARGE:
// 			_raw_status_line = "HTTP/1.1 413 Content Too Large\r\n";
// 			break;
// 		case HTTP_NOT_FOUND:
// 			_raw_status_line = "HTTP/1.1 404 Not Found\r\n";
// 			break;
// 		case HTTP_INTERNAL_SERVER_ERROR:
// 			_raw_status_line = "HTTP/1.1 500 HTTP Version Not Supported\r\n";
// 			break;
//         case HTTP_UNAUTHORIZED:
// 			_raw_status_line = "HTTP/1.1 401 Unauthorized\r\n";
// 			break;
//         case HTTP_FOUND:
//             _raw_status_line = "HTTP/1.1 302 Found\r\n";
//             setLocationHeader();
//             break;
// 		default:
// 			// ?
// 			break;
// 	}
//     setHeaders();
//     setRawHeaders();
//     _raw_body = raw_body;
//     setRawResponse();
//     std::cout << "raw resp is: " << _raw_response << std::endl;
// }

// static std::string statusToString(HttpResponseStatus statusCode) {
//     switch (statusCode) {
//         case HTTP_BAD_REQUEST:
//             return "400 Bad Request";
//         case HTTP_UNAUTHORIZED:
//             return "401 Unauthorized";
//         case HTTP_NOT_FOUND:
//             return "404 Not Found";
//         case HTTP_LENGTH_REQUIRED:
//             return "411 Length Required";
//         case HTTP_PAYLOAD_TOO_LARGE:
//             return "413 Content Too Large";
//         case HTTP_INTERNAL_SERVER_ERROR:
//             return "500 Internal Server Error";
//         case HTTP_VERSION_NOT_SUPPORTED:
//             return "505 HTTP Version Not Supported";
//         default:
//             return "Unknown";
//     }
// }

// void        FailActionResponse::constructConfigResponse(Request& req, std::string filePath) {
//     _resource = filePath;
//     _raw_status_line = _http_version + " " + statusToString(req.getStatusCode()) + "\r\n";
//     // std::cout << "STATUS LINE IS: " << _raw_status_line << std::endl;
//     setHeaders();
//     setRawHeaders();
//     setRawBody();
//     setRawResponse();
//     std::cout << "raw resp is: " << _raw_response << std::endl;
//     //printResponse();
// }

#include "FailActionResponse.hpp"

// Constructor
FailActionResponse::FailActionResponse() {}

// Destructor
FailActionResponse::~FailActionResponse() {}

// Sets the "Location" header to redirect the client
void FailActionResponse::setLocationHeader() {
    _headers.insert(std::make_pair("Location", "/"));
}

// Set HTTP headers based on status and response context
void FailActionResponse::setHeaders() {
    setDateHeader();
    setContentTypeHeader();

    if (_raw_status_line.find("Found") != std::string::npos) {
        setLocationHeader();
    }

    // Additional headers may be added here
}

// Read content from the file indicated by _resource and set it as body
void FailActionResponse::setRawBody() {
    std::ifstream file(_resource.c_str());
    std::string content;

    if (file) {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
    }

    _raw_body = content;
    setContentLengthHeader();
}

// No-op for now, could be used for additional logic later
void FailActionResponse::constructResponse(Request& req) {
    (void)req;
}

// Set response body and headers based on a raw body and request status
void FailActionResponse::constructDefaultResponseWithBody(Request& req, std::string raw_body) {
    _status_code = HTTP_VERSION_NOT_SUPPORTED;

    switch (req.getStatusCode()) {
        case HTTP_VERSION_NOT_SUPPORTED:
            _raw_status_line = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
            break;
        case HTTP_PAYLOAD_TOO_LARGE:
            _raw_status_line = "HTTP/1.1 413 Content Too Large\r\n";
            break;
        case HTTP_NOT_FOUND:
            _raw_status_line = "HTTP/1.1 404 Not Found\r\n";
            break;
        case HTTP_INTERNAL_SERVER_ERROR:
            _raw_status_line = "HTTP/1.1 500 Internal Server Error\r\n";
            break;
        case HTTP_UNAUTHORIZED:
            _raw_status_line = "HTTP/1.1 401 Unauthorized\r\n";
            break;
        case HTTP_FOUND:
            _raw_status_line = "HTTP/1.1 302 Found\r\n";
            setLocationHeader();
            break;
        default:
            _raw_status_line = "HTTP/1.1 400 Bad Request\r\n";
            break;
    }

    setHeaders();
    setRawHeaders();
    _raw_body = raw_body;
    setRawResponse();

    std::cout << "Raw response is:\n" << _raw_response << std::endl;
}

// Converts an enum status code to its string representation
static std::string statusToString(HttpResponseStatus status_code) {
    switch (status_code) {
        case HTTP_BAD_REQUEST:
            return "400 Bad Request";
        case HTTP_UNAUTHORIZED:
            return "401 Unauthorized";
        case HTTP_NOT_FOUND:
            return "404 Not Found";
        case HTTP_LENGTH_REQUIRED:
            return "411 Length Required";
        case HTTP_PAYLOAD_TOO_LARGE:
            return "413 Content Too Large";
        case HTTP_INTERNAL_SERVER_ERROR:
            return "500 Internal Server Error";
        case HTTP_VERSION_NOT_SUPPORTED:
            return "505 HTTP Version Not Supported";
        default:
            return "400 Bad Request";
    }
}

// Constructs a response based on a static error HTML file
void FailActionResponse::constructConfigResponse(Request& req, std::string file_path) {
    _resource = file_path;
    _raw_status_line = _http_version + " " + statusToString(req.getStatusCode()) + "\r\n";

    setHeaders();
    setRawHeaders();
    setRawBody();
    setRawResponse();

    std::cout << "Raw response is:\n" << _raw_response << std::endl;
}