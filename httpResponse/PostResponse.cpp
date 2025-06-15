// #include "PostResponse.hpp"
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <iostream>

// // Constructors
// PostResponse::PostResponse() {

// }

// PostResponse::PostResponse(ServerConfiguration* sb) {
//     _root_folder = sb->getDocumentRoot();
// 	_host = sb->getHostname();
// }

// PostResponse::~PostResponse() {

// }

// // Public methods
// std::string trimNonNumerical(std::string input) {
//     std::string output = "";
//     for (unsigned int i = 0; i < input.length(); i++) {
//         if (isdigit(input[i])) {
//             output += input[i];
//         }
//     }
//     return output;
// }

// void parseMultipartFormData(std::ifstream& tmpFile) {
//     std::string line;
//     std::string separator;
//     std::string name;
//     std::string value;
//     std::string picture;
//     std::string description;
//     std::string price;
//     std::string phone;

//     std::getline(tmpFile, separator);
//     while (tmpFile.good())
//     {
//         std::getline(tmpFile, line);
//         if (line.find("Content-Disposition: form-data; name=") != std::string::npos)
//         {
//             name = line.substr(line.find("name=") + 6, line.length() - 1);
//             name = name.substr(0, name.find("\""));
//             std::getline(tmpFile, line);
//             if (name != "picture")
//                 std::getline(tmpFile, line);
//             value = line.substr(0, line.length() - 1);
//         }
//         if (name == "description")
//         {
//             description = value;
//         }
//         else if (name == "price")
//         {
//             price = value;
//         }
//         else if (name == "phone")
//         {
//             phone = value;
//         }
//         else if (name == "picture")
//         {
//             if (line.find("Content-Type: image/jpeg") != std::string::npos)
//             {
//                 std::getline(tmpFile, line);
//                 while (std::getline(tmpFile, line))
//                 {
//                     if (line.find(trimNonNumerical(separator)) == std::string::npos)
//                     {
//                         picture += line;
//                         picture += '\n';
//                     }
//                 }
//             }
//             else
//             {
//                 break;
//             }
//         }
//     }
//     std::ofstream dataFile("tmp/data-entry.txt");
//     dataFile << "description=" << description << "&price=" << price << "&phone=" << phone;
//     dataFile.close();

//     if (picture.empty())
//         return ;
//     std::ofstream pictureFile("tmp/pic-entry.jpeg", std::ios::binary);
//     pictureFile.write(picture.c_str(), picture.size());
//     pictureFile.close();
// }

// void moveAndRenameFile(const std::string& currentPath, const std::string& newPath) {
//     if (rename(currentPath.c_str(), newPath.c_str()) != 0) {
//         std::perror("Error renaming file");
//     }
// }

// void    PostResponse::executePostResponse(Request& req) {
//     std::ofstream   tmpFile;
//     std::ifstream   inputFile;
//     const char*     file_path = "tmp/data-entry.txt";
//     std::string     command;

//     // write the body of the POST request to a tmp file
//     tmpFile.open(file_path);
//     tmpFile << req.getRawBody();
//     tmpFile.close();

//     // take the contents of the tmpFile, and split them
//     inputFile.open(file_path);
//     parseMultipartFormData(inputFile);
//     inputFile.close();

//     // execute the php script with the contents of the file to add to the json
//     const char*     filePath2 = "tmp/id_file";
//     command = "php cgi-bin/add-entry.php " + std::string(file_path) + " > " + filePath2;
//     //command = "php " + _resource + " " + file_path + " > " + filePath2;
//     std::cout << "command is " << command << std::endl;
//     int exit_status = std::system(command.c_str());
//     std::cout << "exit status: " << exit_status << std::endl;
//     // Exit if no picture curl request
//     std::ifstream picture_file("tmp/pic-entry.jpeg");
//     if (picture_file)
//     {
//         // take the id number and move the pic-entry.jpeg into the correct name in the images folder
//         std::ifstream   tmpFile2(filePath2);
//         std::string     id_string((std::istreambuf_iterator<char>(tmpFile2)), std::istreambuf_iterator<char>());
//         std::cout << "name: public/www/images/" + id_string + ".jpeg" << std::endl;
//         std::cout << "root folder is " << _root_folder << std::endl;
//         moveAndRenameFile("tmp/pic-entry.jpeg", _root_folder + "/images/" + id_string + ".jpeg");
//     }
//     // delete the tmp file
//     remove(file_path);
//     remove("tmp/post-entry.txt");
//     remove(filePath2);
// }

// void    PostResponse::executePostDeleteResponse(Request& req) {
//     std::ofstream   tmpFile;
//     const char*     file_path = "tmp/delete-entry.txt";
//     std::string     command;

//     // write the body of the POST request to a tmp file
//     tmpFile.open(file_path);
//     // std::cout << "DELETE BODY" << req.getRawBody() << std::endl;
//     tmpFile << req.getRawBody();
//     tmpFile.close();

//     // execute the php script with the contents of the file
//     command = "php cgi-bin/delete-entry.php " + std::string(file_path);
//     std::system(command.c_str());

//     // grab the id from the tmpFile
//     std::string id = req.getRawBody().substr(req.getRawBody().find("&") + 4);
//     std::cout << "ID FOR DELETE: " << id << std::endl;
//     // delete the relevant entry from the folder public/www/images
//     std::string remove_path = _root_folder + "/images/" + id + ".jpeg";
//     remove(remove_path.c_str());
//     // TODO: check return of above path! does file not exist, what then?

//     // delete the tmp file
//     remove(file_path);
//     // TODO: check return value ret and throw error if fail...
// }

// void    PostResponse::setLocationHeader(void) {
//     _headers.insert(std::make_pair("Location", "/"));
// }

// void    PostResponse::setRefreshHeader(double sec) {
//     std::stringstream   ss;
//     std::string         str;

//     ss << sec;
//     str = ss.str();
//     _headers.insert(std::make_pair("Refresh", str + ";url=/"));
// }

// void    PostResponse::setHeaders(void) {
//     setDateHeader();
//     // setContentLengthHeader();
//     setConnectionHeader("close");
//     // setContentTypeHeader();
//     setLocationHeader();
// 	setHost(_host.c_str());
//     // setCacheControl("no-cache");
//     // setRefreshHeader(0.3);
//     // add more headers if desired below...
//     // add here...
// }

// void    PostResponse::constructResponse(Request& req) {


//     // need to send message to retireve the actual getPath() for the request URI
//     std::string path = (req.getURI()).substr(1); // remove the leading slash

//     setResource(req);
//     _raw_status_line = _http_version + " 302 Found" + "\r\n";

//     // execute the php...
//     if (path.find("add-entry.php") != std::string::npos)
//         executePostResponse(req);
//     else
//         executePostDeleteResponse(req);
//     // TODO: what if curl launches a dumb post request? -->Prevent!

//     // set the headers
//     setHeaders();
//     setRawHeaders();

//     // set response
//     setRawResponse();
//     // printResponse();
// }

// void	PostResponse::constructDefaultResponseWithBody(Request &req, std::string raw_body) {
// 	(void)req;
// 	(void)raw_body;
// }

// void    PostResponse::constructConfigResponse(Request& req, std::string filePath) {
//     (void)req;
//     (void)filePath;
// }


#include "PostResponse.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cctype>

namespace {

std::string trimNonNumerical(const std::string& input) {
    std::string output;
    for (char ch : input) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            output += ch;
        }
    }
    return output;
}

void parseMultipartFormData(std::ifstream& tmpFile) {
    std::string line, separator, name, value;
    std::string picture, description, price, phone;

    std::getline(tmpFile, separator);
    while (std::getline(tmpFile, line)) {
        if (line.find("Content-Disposition: form-data; name=") != std::string::npos) {
            name = line.substr(line.find("name=") + 6);
            name = name.substr(0, name.find("\""));

            std::getline(tmpFile, line); // Skip content-type or blank line
            if (name != "picture") std::getline(tmpFile, line); // value line

            value = line.substr(0, line.length() - 1);

            if (name == "description") description = value;
            else if (name == "price") price = value;
            else if (name == "phone") phone = value;
        } else if (name == "picture" && line.find("Content-Type: image/jpeg") != std::string::npos) {
            std::getline(tmpFile, line); // Skip empty line
            while (std::getline(tmpFile, line)) {
                if (line.find(trimNonNumerical(separator)) == std::string::npos) {
                    picture += line + '\n';
                } else {
                    break;
                }
            }
        }
    }

    std::ofstream dataFile("tmp/data-entry.txt");
    if (dataFile.is_open()) {
        dataFile << "description=" << description << "&price=" << price << "&phone=" << phone;
        dataFile.close();
    }

    if (!picture.empty()) {
        std::ofstream pictureFile("tmp/pic-entry.jpeg", std::ios::binary);
        if (pictureFile.is_open()) {
            pictureFile.write(picture.c_str(), picture.size());
            pictureFile.close();
        }
    }
}

void moveAndRenameFile(const std::string& from, const std::string& to) {
    if (std::rename(from.c_str(), to.c_str()) != 0) {
        std::perror(("Error renaming " + from + " to " + to).c_str());
    }
}

} // anonymous namespace

// Constructors
PostResponse::PostResponse() {}

PostResponse::PostResponse(ServerConfiguration* serverBlock)
    : _rootDirectory(serverBlock->getDocumentRoot()), _host(serverBlock->getHostname()) {}

PostResponse::~PostResponse() {}

void PostResponse::executePostResponse(Request& req) {
    const std::string dataFilePath = "tmp/data-entry.txt";
    const std::string idFilePath = "tmp/id_file";
    const std::string picTempPath = "tmp/pic-entry.jpeg";

    {
        std::ofstream tmpFile(dataFilePath);
        if (tmpFile) {
            tmpFile << req.getRawBody();
        }
    }

    {
        std::ifstream inputFile(dataFilePath);
        if (inputFile) {
            parseMultipartFormData(inputFile);
        }
    }

    std::string command = "php cgi-bin/add-entry.php " + dataFilePath + " > " + idFilePath;
    int status = std::system(command.c_str());
    std::cout << "[PostResponse] PHP command: " << command << "\n";
    std::cout << "[PostResponse] Exit status: " << status << "\n";

    std::ifstream pictureFile(picTempPath);
    if (pictureFile) {
        std::ifstream idFile(idFilePath);
        std::string id((std::istreambuf_iterator<char>(idFile)), std::istreambuf_iterator<char>());
        std::string finalImagePath = _rootDirectory + "/images/" + id + ".jpeg";

        std::cout << "[PostResponse] Moving image to: " << finalImagePath << "\n";
        moveAndRenameFile(picTempPath, finalImagePath);
    }

    std::remove(dataFilePath.c_str());
    std::remove("tmp/post-entry.txt");
    std::remove(idFilePath.c_str());
}

void PostResponse::executePostDeleteResponse(Request& req) {
    const std::string deleteFilePath = "tmp/delete-entry.txt";

    {
        std::ofstream tmpFile(deleteFilePath);
        if (tmpFile) {
            tmpFile << req.getRawBody();
        }
    }

    std::string command = "php cgi-bin/delete-entry.php " + deleteFilePath;
    std::system(command.c_str());

    std::string id = req.getRawBody().substr(req.getRawBody().find("&") + 4);
    std::string removePath = _rootDirectory + "/images/" + id + ".jpeg";

    std::cout << "[PostResponse] Deleting image: " << removePath << "\n";
    std::remove(removePath.c_str());
    std::remove(deleteFilePath.c_str());
}

void PostResponse::setLocationHeader() {
    _headers["Location"] = "/";
}

void PostResponse::setRefreshHeader(double sec) {
    std::ostringstream ss;
    ss << sec;
    _headers["Refresh"] = ss.str() + ";url=/";
}

void PostResponse::setHeaders() {
    setDateHeader();
    setConnectionHeader("close");
    setLocationHeader();
    setHost(_host.c_str());
    // Optionally: setCacheControl("no-cache");
}

void PostResponse::constructResponse(Request& req) {
    std::string path = req.getURI().substr(1); // strip leading '/'

    setResource(req);
    _raw_status_line = _http_version + " 302 Found\r\n";

    if (path.find("add-entry.php") != std::string::npos)
        executePostResponse(req);
    else
        executePostDeleteResponse(req);

    setHeaders();
    setRawHeaders();
    setRawResponse();
}

void PostResponse::constructDefaultResponseWithBody(Request& req, const std::string& raw_body) {
    (void)req;
    (void)raw_body;
}

void PostResponse::constructConfigResponse(Request& req, const std::string& filePath) {
    (void)req;
    (void)filePath;
}

void PostResponse::printResponse() {
    std::cout << "PostResponse:\n"
              << _raw_status_line << _raw_headers << _raw_body << std::endl;
}

void PostResponse::setStatusCode(HttpResponseStatus statusCode) {
    setHttpResponseStatusCode(statusCode);
}



