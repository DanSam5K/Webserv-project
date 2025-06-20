/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:50:54 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/07 15:37:34 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../http/Request.hpp"
#include "../config/ConfigParser.hpp"
#include "MultipartParser.hpp"

// Stand Alone implementation
// class CGIHandler {
// public:
//     CGIHandler(const std::string& scriptPath,
//                const std::string& method,
//                const std::string& body,
//                const std::string& queryString = "");

//     std::string execute();

// private:
//     std::string _scriptPath;
//     std::string _method;
//     std::string _body;
//     std::string _queryString;

//     std::vector<std::string> createEnv();
//     char** convertEnv(const std::vector<std::string>& env);
// }; 

class CGIHandler {
    public:
        CGIHandler(const std::string& scriptPath,
                   const std::string& method,
                   const std::string& body,
                   const std::string& contentType = "",
                   const std::string& queryString = "",
                   const std::string& uploadPath = "");
    
        std::string execute();
    
    private:
        std::string _scriptPath;
        std::string _method;
        std::string _body;
        std::string _contentType;
        std::string _queryString;
        std::string _uploadPath;
    
        std::vector<std::string> createEnv();
        char** convertEnv(const std::vector<std::string>& env);
        void cleanupEnv(char** env);
    };

#endif
