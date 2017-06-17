//
// Created by islam on 16.06.17.
//

#pragma once

#include <vector>
#include <cstdint>


const int E_OK = 0;
const int E_INVALID_ARGS = 1;
const int E_FAILURE = 2;

using integer = uint32_t;

namespace utils {
    template<class T, class S>
    class binary_iterator {
        T current_value;
        S & stream;
    public:
        binary_iterator(S & stream) : stream(stream) {
            ++(*this);
        }

        bool operator++() {
            if (stream.eof()) {
                return false;
            }
            stream.read(reinterpret_cast<char *>(&current_value), sizeof(T));
            return true;
        }

        T & operator*() {
            return current_value;
        }

        operator bool() {
            return !stream.eof();
        }
    };

    template<class S, class T>
    bool binary_read(S * stream, T & value) {
        return !!stream->read(reinterpret_cast<char *>(&value), sizeof(T));
    };

    template<class S, class T>
    bool binary_read(S * stream, std::vector <T> & buffer) {
        return !!stream->read(reinterpret_cast<char *>(buffer.data()), buffer.size() * sizeof(T));
    };

    template<class S, class T>
    bool binary_write(S * stream, T & value) {
        return !!stream->write(reinterpret_cast<char *>(&value), sizeof(T));
    };

    template<class S, class T>
    bool binary_write(S * stream, std::vector <T> & buffer) {
        return !!stream->write(reinterpret_cast<char *>(buffer.data()), buffer.size() * sizeof(T));
    };

    template<class S, class T>
    bool binary_write(S & stream, T & value) {
        return binary_write(&stream, value);
    };
}
