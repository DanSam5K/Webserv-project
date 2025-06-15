#ifndef FAIL_ACTION_RESPONSE_HPP
# define FAIL_ACTION_RESPONSE_HPP

# include "DeleteActionResponse.hpp"
# include <string>
# include <map>
# include <iostream>
# include <fstream>
# include <sstream>

class FailActionResponse : public Response 
{
    private:

    public:
        // Constructorss
        FailActionResponse();
		virtual ~FailActionResponse();

        // Public methods
        void        constructResponse(Request& req);
        void        constructDefaultResponseWithBody(Request& req, std::string raw_body);
        void        constructConfigResponse(Request& req, std::string filePath);
        void        setHeaders();
        void        setRawBody();
        void        setLocationHeader();
        void        printResponse(void);
};

#endif
