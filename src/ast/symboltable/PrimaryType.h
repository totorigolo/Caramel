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

#include "Common.h"

#include <cstddef>
#include <memory>
#include <limits>

namespace caramel::ast {

class PrimaryType {
public:
    using Ptr = std::shared_ptr<PrimaryType>;
    using WeakPtr = std::weak_ptr<PrimaryType>;

    virtual size_t getMemoryLength() const = 0;
    virtual std::string getIdentifier() const = 0;

    static PrimaryType::Ptr max(PrimaryType::Ptr a, PrimaryType::Ptr b) {
        if(a->getMemoryLength() > b->getMemoryLength()) {
            return a;
        } else {
            return b;
        }
    }

    bool equals(PrimaryType::Ptr const & other) {
        return typeid(*this) == typeid(*other);
    }

    bool greaterThan(PrimaryType::Ptr const &other) {
        return getMemoryLength() >= other->getMemoryLength();
    }
};

class Void_t : public PrimaryType {
public:
    static Ptr Create() {
        return Ptr(new Void_t);
    }

    size_t getMemoryLength() const override {
        return 0;
    }

    std::string getIdentifier() const override {
        return "void";
    }

    static bool fits(caramel_unused long long value) {
        return false;
    }
};

class Int8_t : public PrimaryType {
public:
    static Ptr Create() {
        return Ptr(new Int8_t);
    }

    size_t getMemoryLength() const override {
        return 8;
    }

    std::string getIdentifier() const override {
        return "int8_t";
    }

    static bool fits(long long value) {
        return std::numeric_limits<int8_t>::min() <= value && value <= std::numeric_limits<int8_t>::max();
    }
};

class Char : public Int8_t {
public:
    static Ptr Create() {
        return Ptr(new Char);
    }

    std::string getIdentifier() const override {
        return "char";
    }
};

class Int16_t : public PrimaryType {
public:
    static Ptr Create() {
        return Ptr(new Int16_t);
    }

    size_t getMemoryLength() const override {
        return 16;
    }

    std::string getIdentifier() const override {
        return "int16_t";
    }

    static bool fits(long long value) {
        return std::numeric_limits<int16_t>::min() <= value && value <= std::numeric_limits<int16_t>::max();
    }
};

class Int32_t : public PrimaryType {
public:
    static Ptr Create() {
        return Ptr(new Int32_t);
    }

    size_t getMemoryLength() const override {
        return 32;
    }

    std::string getIdentifier() const override {
        return "int32_t";
    }

    static bool fits(long long value) {
        return std::numeric_limits<int32_t>::min() <= value && value <= std::numeric_limits<int32_t>::max();
    }
};

class Int64_t : public PrimaryType {
public:
    static Ptr Create() {
        return Ptr(new Int64_t);
    }

    size_t getMemoryLength() const override {
        return 64;
    }

    std::string getIdentifier() const override {
        return "int64_t";
    }

    static bool fits(long long value) {
        return std::numeric_limits<int64_t>::min() <= value && value <= std::numeric_limits<int64_t>::max();
    }
};

} // namespace caramel::dataStructure::symbolTable