/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkedDecoder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:01:40 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/11 13:03:27 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>
#include <iostream>
class ChunkedDecoder {
public:
    static std::string decode(const std::string& rawBody);
};
