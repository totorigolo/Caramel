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

#include <iostream>

namespace caramel::colors {

#define DEFINE_COLOR(name, special_str)        \
inline std::ostream &name(std::ostream &os) {  \
    return os << (special_str);                \
}

DEFINE_COLOR(reset, "\x1B[00m")

DEFINE_COLOR(bold, "\x1B[01m")
DEFINE_COLOR(italic, "\x1B[03m")
DEFINE_COLOR(underlined, "\x1B[04m")

DEFINE_COLOR(white, "\x1B[37m")
DEFINE_COLOR(red, "\x1B[31m")
DEFINE_COLOR(green, "\x1B[32m")
DEFINE_COLOR(yellow, "\x1B[33m")
DEFINE_COLOR(blue, "\x1B[34m")
DEFINE_COLOR(magenta, "\x1B[35m")
DEFINE_COLOR(cyan, "\x1B[36m")
DEFINE_COLOR(grey, "\x1B[02m")
DEFINE_COLOR(darkgrey, "\x1B[30m")

DEFINE_COLOR(white_bckg, "\x1B[07m")
DEFINE_COLOR(red_bckg, "\x1B[41m")
DEFINE_COLOR(green_bckg, "\x1B[42m")
DEFINE_COLOR(yellow_bckg, "\x1B[43m")
DEFINE_COLOR(blue_bckg, "\x1B[44m")
DEFINE_COLOR(magenta_bckg, "\x1B[45m")
DEFINE_COLOR(cyan_bckg, "\x1B[46m")
DEFINE_COLOR(grey_bckg, "\x1B[47m")
DEFINE_COLOR(darkgrey_bckg, "\x1B[40m")

} // namespace caramel::colors
