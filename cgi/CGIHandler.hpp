/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:50:54 by remijacobs        #+#    #+#             */
/*   Updated: 2025/05/16 17:51:35 by remijacobs       ###   ########.fr       */
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
#include <sstream>

class CGIHandler {
public:
    CGIHandler(const std::string& scriptPath,
               const std::string& method,
               const std::string& body,
               const std::string& queryString = "");

    std::string execute();

private:
    std::string _scriptPath;
    std::string _method;
    std::string _body;
    std::string _queryString;

    std::vector<std::string> createEnv();
    char** convertEnv(const std::vector<std::string>& env);
};

#endif
