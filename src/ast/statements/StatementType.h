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

namespace caramel::ast {

enum class StatementType {
    Unknown,
    Declaration,
    VariableDeclaration,
    FunctionDeclaration,
    ArrayDeclaration,
    Definition,
    VariableDefinition,
    FunctionDefinition,
    ArrayDefinition,
    Jump,
    ReturnStatement,
    BreakStatement,
    Expression,
    CommaExpression,
    AtomicExpression,
    Constant,
    ConstString,
    LValue,
    Identifier,
    FunctionCall,
    ArrayAccess,

    // BinaryExpression w/ sub-types
    BinaryExpression,
    BitwiseShiftExpression,
    AdditiveExpression,
    MultiplicativeExpression,
    ComparativeExpression,
    EqualityExpression,
    BitwiseExpression,
    ConjunctionExpression,
    DisjunctionExpression,

    // UnaryExpression w/ sub-types
    UnaryExpression,
    UnaryAdditiveExpression,
    LogicalNotExpression,
    NegativeExpression
};

inline std::ostream & operator<<(std::ostream &os, const StatementType &type) {
    switch (type) {
        case StatementType::Unknown: return os << "Unknown";
        case StatementType::Declaration: return os << "Declaration";
        case StatementType::VariableDeclaration: return os << "VariableDeclaration";
        case StatementType::FunctionDeclaration: return os << "FunctionDeclaration";
        case StatementType::ArrayDeclaration: return os << "ArrayDeclaration";
        case StatementType::Definition: return os << "Definition";
        case StatementType::VariableDefinition: return os << "VariableDefinition";
        case StatementType::FunctionDefinition: return os << "FunctionDefinition";
        case StatementType::ArrayDefinition: return os << "ArrayDefinition";
        case StatementType::Jump: return os << "Jump";
        case StatementType::ReturnStatement: return os << "ReturnStatement";
        case StatementType::BreakStatement: return os << "BreakStatement";
        case StatementType::Expression: return os << "Expression";
        case StatementType::CommaExpression: return os << "CommaExpression";
        case StatementType::AtomicExpression: return os << "AtomicExpression";
        case StatementType::Constant: return os << "Constant";
        case StatementType::ConstString: return os << "ConstString";
        case StatementType::LValue: return os << "LValue";
        case StatementType::Identifier: return os << "Identifier";
        case StatementType::FunctionCall: return os << "FunctionCall";
        case StatementType::ArrayAccess: return os << "ArrayAccess";
        case StatementType::BinaryExpression: return os << "BinaryExpression";
        case StatementType::BitwiseShiftExpression: return os << "BitwiseShiftExpression";
        case StatementType::AdditiveExpression: return os << "AdditiveExpression";
        case StatementType::MultiplicativeExpression: return os << "MultiplicativeExpression";
        case StatementType::ComparativeExpression: return os << "ComparativeExpression";
        case StatementType::EqualityExpression: return os << "EqualityExpression";
        case StatementType::BitwiseExpression: return os << "BitwiseExpression";
        case StatementType::ConjunctionExpression: return os << "ConjunctionExpression";
        case StatementType::DisjunctionExpression: return os << "DisjunctionExpression";
        case StatementType::UnaryExpression: return os << "UnaryExpression";
        case StatementType::UnaryAdditiveExpression: return os << "UnaryAdditiveExpression";
        case StatementType::LogicalNotExpression: return os << "LogicalNotExpression";
        case StatementType::NegativeExpression: return os << "NegativeExpression";
    }
    return os;
}

} // namespace caramel::ast
