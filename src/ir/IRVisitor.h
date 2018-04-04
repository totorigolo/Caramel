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

#include "IR.h"
#include "instructions/CallParameterInstruction.h"


namespace caramel::ir {

class CopyInstruction;

class EmptyInstruction;

class PrologInstruction;

class EpilogInstruction;

class AdditionInstruction;
class SubtractionInstruction;

class LDConstInstruction;

class FunctionCallInstruction;

class NopInstruction;

class ReturnInstruction;
class JumpInstruction;
class JumpEqualInstruction;
class JumpLessInstruction;
class JumpLessOrEqualInstruction;
class JumpGreaterOrEqualInstruction;
class JumpGreaterInstruction;
class GTEInstruction;
class PushInstruction;
class PopInstruction;

class IRVisitor {
public:

    virtual void visitCopy(
            CopyInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitEmpty(
            EmptyInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitProlog(
            PrologInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitEpilog(
            EpilogInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitAddition(
            AdditionInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitLdConst(
            LDConstInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitNope(
            NopInstruction *instjump_eqruction,
            std::ostream &os
    ) = 0;

    virtual void visitFunctionCall(
            FunctionCallInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitReturn(
            ReturnInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitJump(
            JumpInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitJumpEqual(
            JumpEqualInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitJumpLess(
            JumpLessInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitJumpLessOrEqual(
            JumpLessOrEqualInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitJumpGreaterOrEqual(
            JumpGreaterOrEqualInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitJumpGreater(
            JumpGreaterInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitGTE(
            GTEInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitCallParameter(
            CallParameterInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitSubtraction(
            SubtractionInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitPush(
            PushInstruction *instruction,
            std::ostream &os
    ) = 0;

    virtual void visitPop(
            PopInstruction *instruction,
            std::ostream &os
    ) = 0;

};

} // namespace caramel::ir



