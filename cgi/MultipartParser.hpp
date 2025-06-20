/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultipartParser.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:54:38 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/17 22:12:57 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPARTPARSER_HPP
#define MULTIPARTPARSER_HPP

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

class MultipartParser {
public:
    MultipartParser(const std::string& body, const std::string& boundary);
    void parse();

    const std::vector<std::map<std::string, std::string> >& getParsedFiles() const;

private:
    std::string _body;
    std::string _boundary;
    std::vector<std::map<std::string, std::string> > _parsedFiles;
};

#endif
