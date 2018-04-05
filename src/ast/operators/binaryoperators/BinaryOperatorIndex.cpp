/*
 * MIT License
 *
 * Copyright (c) 2018 Kalate Hexanome, 4IF, INSA Lyon
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

#include "BinaryOperatorIndex.h"
#include "PlusOperator.h"
#include "MultOperator.h"
#include "RightShiftOperator.h"
#include "MinusOperator.h"
#include "DivOperator.h"
#include "LeftShiftOperator.h"
#include "LowerThanOperator.h"
#include "LowerEqThanOperator.h"
#include "GreaterThanOperator.h"
#include "GreaterEqThanOperator.h"
#include "EqualityOperator.h"
#include "NotEqOperator.h"
#include "BitwiseAndOperator.h"
#include "BitwiseOrOperator.h"
#include "BitwiseXorOperator.h"
#include "DisjunctionOperator.h"
#include "ConjunctionOperator.h"
#include "AssignmentOperator.h"
#include "ModOperator.h"


#define BIND(op) index.insert(make_pair(op::SYMBOL, dynamic_pointer_cast<BinaryOperator>(make_shared<op>())))

using namespace std;

caramel::ast::BinaryOperatorIndex::BinaryOperatorIndex() {

    // Additive operators
    BIND(PlusOperator);
    BIND(MinusOperator);

    // Multiplicative operators
    BIND(MultOperator);
    BIND(DivOperator);

    // Modulo
    BIND(ModOperator);

    // Bitwise shift operators
    BIND(RightShiftOperator);
    BIND(LeftShiftOperator);

    // Comparison operators
    BIND(LowerThanOperator);
    BIND(LowerEqThanOperator);
    BIND(GreaterThanOperator);
    BIND(GreaterEqThanOperator);

    // Equality operators
    BIND(EqualityOperator);
    BIND(NotEqOperator);

    // Bitwise operators
    BIND(BitwiseAndOperator);
    BIND(BitwiseXorOperator);
    BIND(BitwiseOrOperator);

    // Conjunction & Disjunction operators
    BIND(ConjunctionOperator);
    BIND(DisjunctionOperator);
}

caramel::ast::BinaryOperator::Ptr caramel::ast::BinaryOperatorIndex::getOpForToken(std::string token) {
    auto it = index.find(token);

    if (it == index.end()) {
        if (token == "=") {
            logger.fatal() << "The = operator has to be handled manually, because it needs an lvalue.";
            exit(1);
        }

        logger.fatal() << "Operator " << token << " not found.";
        exit(1);
    }

    return it->second;
}