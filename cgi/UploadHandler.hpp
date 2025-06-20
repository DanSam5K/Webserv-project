/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:39:25 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/17 22:18:37 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HANDLER_HPP
#define UPLOAD_HANDLER_HPP

#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <string>
#include <vector>
#include <map>

class UploadHandler {
public:
    UploadHandler(const std::string& uploadDir);
    ~UploadHandler();

    // Saves one file to disk
    bool saveFile(const std::string& fieldName,
                  const std::string& fileName,
                  const std::string& fileContent);

    // Batch saving support (from MultipartParser::getFiles())
    bool saveAll(const std::vector<std::map<std::string, std::string> >& files);

    std::string getLastError() const;

private:
    std::string _uploadDir;
    std::string _lastError;

    bool ensureDirectoryExists(const std::string& path);
};

#endif
