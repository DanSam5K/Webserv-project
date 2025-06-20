/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultipartParser.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:55:28 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/20 10:44:00 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MultipartParser.hpp"
#include <iostream>
#include <cstdlib>

MultipartParser::MultipartParser(const std::string& body, const std::string& boundary)
    : _body(body), _boundary("--" + boundary) {}

void MultipartParser::parse()
{
    std::istringstream stream(_body);
    std::string line;
    std::string filename, field, content;
    bool inFile = false;

    std::map<std::string, std::string> fileEntry;

    while (std::getline(stream, line))
    {
        if (line.find(_boundary) != std::string::npos)
        {
            if (!fileEntry.empty()) {
                fileEntry["content"] = content;
                _parsedFiles.push_back(fileEntry);
                fileEntry.clear();
                content.clear();
            }
            inFile = false;
        } else if (line.find("Content-Disposition:") != std::string::npos)
        {
            size_t fnamePos = line.find("filename=");
            size_t fieldPos = line.find("name=");

            if (fieldPos != std::string::npos) {
                size_t start = line.find("\"", fieldPos) + 1;
                size_t end = line.find("\"", start);
                field = line.substr(start, end - start);
                fileEntry["field"] = field;
            }

            if (fnamePos != std::string::npos)
            {
                size_t start = line.find("\"", fnamePos) + 1;
                size_t end = line.find("\"", start);
                filename = line.substr(start, end - start);
                fileEntry["filename"] = filename;
            }
        }
        else if (line == "\r" || line == "")
        {
            inFile = true;
        } else if (inFile)
        {
            content += line + "\n";
        }
    }

    // Handle last part
    if (!fileEntry.empty())
    {
        fileEntry["content"] = content;
        _parsedFiles.push_back(fileEntry);
    }
}

const std::vector<std::map<std::string, std::string> >& MultipartParser::getParsedFiles() const
{
    return _parsedFiles;
}
