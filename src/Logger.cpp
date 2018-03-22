/*
 * MIT License
 *
 * Copyright (c) 2018 insa.4if.hexanome_kalate
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "Logger.h"

#include "Common.h"
#include "Console.h"


// The global logger
Logger logger;

Logger::Logger(LoggerLevel level)
        : mLevel{level} {
}

LoggerLevel Logger::getLevel() const {
    return mLevel;
}

Logger::Fatal Logger::Logger::fatal() {
    return Logger::Fatal(*this);
}

Logger::Warning Logger::warning() {
    return Logger::Warning(*this);
}

Logger::Info Logger::info() {
    return Logger::Info(*this);
}

Logger::Debug Logger::debug() {
    return Logger::Debug(*this);
}

Logger::Trace Logger::trace() {
    return Logger::Trace(*this);
}

void Logger::Fatal::show() {
    using namespace Caramel::Colors;
    if (mLogger.getLevel() >= FATAL) {
        std::cerr << red << bold << "[FATAL]" << reset << ' '
                  << red << mLoggedStream.str() << reset << std::endl;
    }
    mLoggedStream.clear();
}

void Logger::Warning::show() {
    using namespace Caramel::Colors;
    if (mLogger.getLevel() >= WARN) {
        std::cerr << yellow << bold << "[WARNING]" << reset << ' '
                  << yellow << mLoggedStream.str() << reset << std::endl;
    }
    mLoggedStream.clear();
}

void Logger::Info::show() {
    using namespace Caramel::Colors;
    if (mLogger.getLevel() >= INFO) {
        std::cerr << blue << bold << "[INFO]" << reset << ' '
                  << mLoggedStream.str() << std::endl;
    }
    mLoggedStream.clear();
}

void Logger::Debug::show() {
    using namespace Caramel::Colors;
    if (mLogger.getLevel() >= DEBUG) {
        std::cerr << cyan << bold << "[DEBUG]" << reset << ' '
                  << mLoggedStream.str() << std::endl;
    }
    mLoggedStream.clear();
}

void Logger::Trace::show() {
    using namespace Caramel::Colors;
    if (mLogger.getLevel() >= TRACE) {
        std::cerr << magenta << bold << "[TRACE]" << reset << ' '
                  << magenta << mLoggedStream.str() << reset << std::endl;
    }
    mLoggedStream.clear();
}
