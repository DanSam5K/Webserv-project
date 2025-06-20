/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uploadhandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:39:48 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/20 10:46:38 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UploadHandler.hpp"

UploadHandler::UploadHandler(const std::string& uploadDir) : _uploadDir(uploadDir), _lastError("") 
{
    ensureDirectoryExists(_uploadDir);
}

UploadHandler::~UploadHandler() {}

bool UploadHandler::ensureDirectoryExists(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        // Try to create directory
        if (mkdir(path.c_str(), 0755) != 0) {
            _lastError = "Failed to create upload directory: " + std::string(strerror(errno));
            return false;
        }
    } else if (!(info.st_mode & S_IFDIR)) {
        _lastError = "Upload path exists but is not a directory.";
        return false;
    }
    return true;
}

bool UploadHandler::saveFile(const std::string& fieldName, const std::string& fileName, const std::string& fileContent) 
{
    if (!ensureDirectoryExists(_uploadDir))
        return false;

    std::string fullPath = _uploadDir + "/" + fileName;

    std::ofstream file(fullPath.c_str(), std::ios::binary);
    if (!file)
    {
        _lastError = "Failed to open file for writing: " + fullPath;
        return false;
    }

    file.write(fileContent.c_str(), fileContent.size());
    if (!file)
    {
        _lastError = "Failed to write file: " + fullPath;
        return false;
    }

    file.close();
    return true;
}

bool UploadHandler::saveAll(const std::vector<std::map<std::string, std::string> >& files)
{
    for (size_t i = 0; i < files.size(); ++i)
    {
        const std::map<std::string, std::string>& file = files[i];

        std::string fieldName = file.at("field");
        std::string fileName = file.at("filename");
        std::string fileContent = file.at("content");

        if (!saveFile(fieldName, fileName, fileContent))
        {
            return false;
        }
    }
    return true;
}

std::string UploadHandler::getLastError() const
{
    return _lastError;
}
