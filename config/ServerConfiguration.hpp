#ifndef SERVER_CONFIGURATION_HPP
#define SERVER_CONFIGURATION_HPP

# include "LocationBlock.hpp"
# include <string>

#include <string>
#include <vector>
#include <map>
// #include "HttpResponseStatus.hpp"  // Make sure this includes the HttpResponseStatus enum or equivalent
#include "LocationBlock.hpp"

// refatcoring needed and move to the appropriate folder structure
// Enumeration representing standard HTTP response codes with descriptive naming
enum HttpResponseStatus {
    // --- 2xx: Success responses ---
    HTTP_OK = 200,                      // Request succeeded (e.g., GET)
    HTTP_CREATED = 201,                 // Resource successfully created (e.g., POST)
    HTTP_NO_CONTENT = 204,             // No content to return (e.g., successful DELETE)

    // --- 3xx: Redirection messages ---
    HTTP_FOUND = 302,                  // Temporary redirect (commonly used after a successful POST)

    // --- 4xx: Client error responses ---
    HTTP_BAD_REQUEST = 400,            // Malformed request syntax or invalid request
    HTTP_UNAUTHORIZED = 401,           // Authentication is required and has failed or not been provided
    HTTP_FORBIDDEN = 403,              // Request is understood but refused by the server
    HTTP_NOT_FOUND = 404,              // Requested resource not found on the server
    HTTP_LENGTH_REQUIRED = 411,        // Content-Length header required but missing
    HTTP_PAYLOAD_TOO_LARGE = 413,      // Request body exceeds server limits
    HTTP_UNSUPPORTED_MEDIA_TYPE = 415, // Media format of the request is not supported

    // --- 5xx: Server error responses ---
    HTTP_INTERNAL_SERVER_ERROR = 500,       // Server encountered an unexpected condition
    HTTP_VERSION_NOT_SUPPORTED = 505        // HTTP version used is not supported by the server
};



// class ServerBlock {
// private:
// 	unsigned long   _client_max_body_size;
// 	std::string		_listening_port;
//     std::string     _root_folder;
//     std::string     _index_page;
//     std::string     _server_name;


// 	std::map<HttpResponseStatus, std::string>	_error_pages;
//     std::vector<LocationBlock*>          _locationBlocks; // TODO: memory leak check

// public:
// 	ServerBlock();
// 	ServerBlock(const ServerBlock& other);
// 	~ServerBlock();

// 	ServerBlock&	operator=(const ServerBlock& other);

//     // Allowed methods
//     bool            _GET_allowed;
//     bool            _POST_allowed;
//     bool            _DELETE_allowed;


//     // Member Functions
//     void        printLocationBlocks(void);
//     std::string getErrorPath(HttpResponseStatus sc);

//     // Getters
// 	unsigned long	getClientMaxBodySize(void);
// 	std::string		getListeningPort(void);
// 	std::string		getRootFolder(void);
// 	std::string		getIndexPage(void);
// 	std::string		getServerName(void);
//     std::vector<LocationBlock*>& getLocationBlocks(void);

//     // Setters
// 	void	            setClientMaxBodySize(unsigned long maxBodySize);
// 	void    		    setListeningPort(std::string listeningPort);
// 	void    		    setServerName(std::string serverName);
// 	void    		    setIndexPage(std::string indexPage);
// 	void    		    setRootFolder(std::string rootFolder);
// 	void    		    setLimitExceptFlag(std::vector<std::string> tokens);
// 	void    		    setDefaultErrorPage(std::string statusCode, std::string filePath);
//     void                setNewLocationBlock(LocationBlock* lb);
// };

// #endif


class ServerConfiguration {
	private:
		unsigned long   _maxClientBodySize;
		std::string     _port;
		std::string     _documentRoot;
		std::string     _defaultIndex;
		std::string     _hostname;

		std::map<HttpResponseStatus, std::string> _errorPagePaths;
		std::vector<LocationBlock*> _routes; // Ensure memory is handled externally or with smart pointers (if permitted)

		// HTTP method permissions
		bool _allowGET;
		bool _allowPOST;
		bool _allowDELETE;

	public:
		ServerConfiguration();
		ServerConfiguration(const ServerConfiguration& source);
		~ServerConfiguration();

		ServerConfiguration& operator=(const ServerConfiguration& source);

		// Utility Methods
		void printRoutes() const;
		std::string fetchErrorPagePath(HttpResponseStatus statusCode) const;

		// Getters
		unsigned long getMaxClientBodySize() const;
		std::string getPort() const;
		std::string getDocumentRoot() const;
		std::string getDefaultIndex() const;
		std::string getHostname() const;
		const std::vector<LocationBlock*>& getRoutes() const;

		// Setters
		void setMaxClientBodySize(unsigned long size);
		void setPort(const std::string& port);
		void setHostname(const std::string& name);
		void setDefaultIndex(const std::string& indexFile);
		void setDocumentRoot(const std::string& rootPath);
		void setAllowedMethods(const std::vector<std::string>& methods);
		void setErrorPage(const std::string& code, const std::string& path);
		void addRoute(LocationBlock* route);
};

#endif // SERVER_CONFIGURATION_HPP
