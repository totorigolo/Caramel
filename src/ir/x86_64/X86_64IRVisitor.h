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

#include "../IR.h"
#include "../IRVisitor.h"

#include <memory>
#include <ostream>


namespace caramel::ir::x86_64 {

class X86_64IRVisitor : public IRVisitor {
public:
    using Ptr = std::shared_ptr<X86_64IRVisitor>;
    using WeakPtr = std::shared_ptr<X86_64IRVisitor>;

public:
    explicit X86_64IRVisitor() = default;
    virtual ~X86_64IRVisitor() = default;

    void visitCopy(caramel::ir::CopyInstruction *instruction, std::ostream &os) override;

    void visitEmpty(caramel::ir::EmptyInstruction *instruction, std::ostream &os) override;

    void visitProlog(caramel::ir::PrologInstruction *instruction, std::ostream &os) override;

    void visitEpilog(caramel::ir::EpilogInstruction *instruction, std::ostream &os) override;

    void visitAddition(caramel::ir::AdditionInstruction *instruction, std::ostream &os) override;

    void visitLdConst(caramel::ir::LDConstInstruction *instruction, std::ostream &os) override;

    void visitNope(caramel::ir::NopInstruction *instruction, std::ostream &os) override;

    void visitFunctionCall(caramel::ir::FunctionCallInstruction *instruction, std::ostream &os) override;

private:
    std::string address(std::string const &symbol);
    std::string registerToAssembly(std::string const &register_, size_t bitSize = 32U);
    std::string toAssembly(ir::IR *instruction, std::string const &anySymbol, size_t bitSize = 32U);
    std::string getSizeSuffix(size_t bitSize);
    std::string getFunctionCallRegister(size_t index, size_t bitSize);
};
} // namespace caramel::ir::x86_64


