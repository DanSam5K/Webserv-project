/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:58:54 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/19 10:53:04 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "MultipartParser.hpp"
#include "UploadHandler.hpp"
#include "CGIHandler.hpp"
#include <string>
#include "ErrorPages.hpp"
#include "ChunkedDecoder.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <dirent.h>

class MethodHandler {
public:
    MethodHandler();

    std::string handleRequest(const std::string& method,
        const std::string& path,
        const std::string& body,
        const std::string& contentType,
        const std::string& transferEncoding,
        int& responseCode);
        
private:
    ErrorPages _errorPages;
    std::string handleGet(const std::string& path, int& responseCode);
    std::string handlePost(const std::string& path, const std::string& body, const std::string& contentType, const std::string& transferEncoding, int& responseCode);
    std::string handleDelete(const std::string& path, int& responseCode);
    
    std::string generateErrorResponse(int code);
};
