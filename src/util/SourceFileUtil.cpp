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

#include "SourceFileUtil.h"
#include "../Logger.h"

SourceFileUtil::SourceFileUtil(const std::string &fileName)
        : mFileName(fileName), mInputStream(fileName) {
    logger.trace() << "New SourceFileUtil(" << fileName << ")";
}

std::string SourceFileUtil::getLine(size_t line, size_t currentCursorLine, bool resetToHead) {
    std::string buffer;
    size_t i = currentCursorLine;
    while (i < line - 1) { // Moves the cursor to the right line
        mInputStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        i++;
    }
    std::getline(mInputStream, buffer);
    if (resetToHead) {
        mInputStream.seekg(0);
    }
    return buffer;
}