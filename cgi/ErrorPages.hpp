/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 23:49:41 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/13 23:50:33 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_PAGES_HPP
#define ERROR_PAGES_HPP

#include <string>
#include <map>
#include <fstream>
#include <sstream>

class ErrorPages {
public:
    ErrorPages();
    void setErrorPage(int statusCode, const std::string& filePath);
    std::string getErrorResponse(int statusCode);

private:
    std::map<int, std::string> _customPages;

    std::string loadFileContent(const std::string& path);
    std::string generateDefaultPage(int statusCode);
};

#endif
