// SPDX-License-Identifier: MIT
#include "io.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace io;

std::string io::readString(std::istream &in_stream) {
    if (in_stream.peek() == EOF) {
        // We reached EOF of stream, but we needed more data
        throw std::invalid_argument("Failed reading input");
    }

    std::string buffer;
    in_stream >> buffer;
    if (in_stream.fail()) {
        throw std::invalid_argument("Failed reading input");
    }

    return buffer;
}

int io::readInt(std::istream &in_stream) {
    std::string buff = readString(in_stream);
    if (!std::all_of(buff.begin(), buff.end(), ::isdigit)) {
        throw std::invalid_argument("Failed reading input");
    }

    return std::stoi(buff);
}

unsigned long io::readULong(std::istream &in_stream) {
    std::string buff = readString(in_stream);
    if (!std::all_of(buff.begin(), buff.end(), ::isdigit)) {
        throw std::invalid_argument("Failed reading input");
    }

    return std::stoul(buff);
}
