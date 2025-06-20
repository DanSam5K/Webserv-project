/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 23:49:52 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/20 10:35:32 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPages.hpp"

ErrorPages::ErrorPages() {}

void ErrorPages::setErrorPage(int statusCode, const std::string& filePath)
{
    _customPages[statusCode] = filePath;
}

std::string ErrorPages::getErrorResponse(int statusCode)
{
    std::map<int, std::string>::iterator it = _customPages.find(statusCode);
    if (it != _customPages.end()) {
        std::string content = loadFileContent(it->second);
        if (!content.empty())
            return content;
    }
    return generateDefaultPage(statusCode);
}

std::string ErrorPages::loadFileContent(const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return "";

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string ErrorPages::generateDefaultPage(int statusCode) {
    std::ostringstream oss;
    oss << "<html><head><title>Error " << statusCode << "</title></head>"
        << "<body><h1>Error " << statusCode << "</h1><p>Something went wrong.</p></body></html>";
    return oss.str();
}
