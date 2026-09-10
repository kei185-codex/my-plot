
#pragma once

#include <string_view>

namespace error
{

enum class Error
{
        OPEN_FILE_FAILED,
        CLOSE_FILE_FAILED,
        NODE_INIT_FAILED,
        FILE_INTERNAL_ERROR,
};

constexpr std::string_view toString(Error error)
{
        switch (error) {
                case Error::OPEN_FILE_FAILED:
                        return "OPEN_FILE_FAILED";
                case Error::CLOSE_FILE_FAILED:
                        return "CLOSE_FILE_FAILED";
                case Error::NODE_INIT_FAILED:
                        return "NODE_INIT_FAILED";
                case Error::FILE_INTERNAL_ERROR:
                        return "FRAME_PARSE_FAILED";
                default:
                        return "UNKNOWN";
        }
}

} // namespace error
