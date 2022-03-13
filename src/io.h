// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <vector>

#include "lca.h"

namespace io {

    /**
     * Reads next argument from std::istream until the first whitespace char
     * into a std::string
     * @param[in] in_stream         std::istream to read from
     * @return Input argument from std::istream as std::string
     */
    std::string readString(std::istream &in_stream);

    /**
     * Reads next argument from std::istream until the first whitespace char
     * and interprets argument as an int
     * @param[in] in_stream         std::istream to read from
     * @return Input argument from std::istream as an int
     */
    int readInt(std::istream &in_stream);

    /**
     * Reads next argument from std::istream until the first whitespace char
     * and interprets argument as an unsigned long
     * @param[in] in_stream         std::istream to read from
     * @return Input argument from std::istream as an unsigned long
     */
    unsigned long readULong(std::istream &in_stream);

}  // namespace io
