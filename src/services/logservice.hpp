// MIT License
//
// Copyright (c) 2021 Happy GardenPI
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cstdint>
#include <functional>
#include <vector>

namespace hgardenpi
{
    inline namespace v1
    {

        using std::function;
        using std::string;
        using std::vector;

        enum class LogLevel {
            TRACE,
            DBG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };

        using LogWriter = function<void(LogLevel level, const string &source, const string &param, const vector<uint8_t> &params)>;

        /**
         * @brief Abstract LogService to write log
         * 
         */
        class LogService
        {

        public:

            virtual ~LogService() = default;

            /**
             * @brief Write log on system 
             * 
             * @param level of log
             * @param msg message to write
             * @param ...
             */
            virtual void write(uint8_t level, const char *msg, ...) const noexcept = 0;
        };

    }
}