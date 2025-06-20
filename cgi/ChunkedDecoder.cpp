/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkedDecoder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: remijacobs <remijacobs@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:02:15 by remijacobs        #+#    #+#             */
/*   Updated: 2025/06/11 13:03:19 by remijacobs       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkedDecoder.hpp"


std::string ChunkedDecoder::decode(const std::string& rawBody)
{
    std::stringstream input(rawBody);
    std::string decoded, line;
    
    while (std::getline(input, line)) {
        // Remove \r if present at end of chunk size line
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        size_t chunkSize;
        std::stringstream chunkLine(line);
        chunkLine >> std::hex >> chunkSize;

        if (chunkSize == 0)
            break;

        char* buffer = new char[chunkSize];
        input.read(buffer, chunkSize);
        decoded.append(buffer, chunkSize);
        delete[] buffer;

        // Read the trailing \r\n after the chunk
        std::getline(input, line); // skip the remaining \r\n
    }

    return decoded;
}
