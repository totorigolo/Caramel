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

#pragma once

#include <fstream>

namespace caramel::utils {

/**
 * SourceFileUtil is used to work with the source code
 */
class SourceFileUtil {

public:
    explicit SourceFileUtil(const std::string &fileName);

    virtual ~SourceFileUtil() = default;

    /**
     * Return the line at `line` line of the source code
     * Pre : 0 <= currentCursorLine <= line < number of lines in the cursor
     * @param line The line number
     * @param currentCursorLine Indicate the cursor line position in the source code (default to 0)
     * @param resetToHead Indicates if the cursor must be moved to the beginning of the source code
     * @return the line of the source code
     */
    std::string getLine(size_t line, size_t currentCursorLine = 0, bool resetToHead = true);

private:
    std::string mFileName;
    std::ifstream mInputStream;
};

} // namespace caramel::utils
