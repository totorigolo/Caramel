//
// Created by aurore on 21/03/18.
//

#pragma once

#include "Definition.h"
#include "FunctionSymbol.h"

#include <memory>


namespace Caramel::DataStructure {

class FunctionDeclaration : public Definition {
public:
    static Ptr Create(FunctionSymbol::WeakPtr symbol) {
        return Ptr(new FunctionDeclaration(std::forward<FunctionSymbol::WeakPtr>(symbol)));
    }

private:
    explicit FunctionDeclaration(FunctionSymbol::WeakPtr symbol)
    : mSymbol(std::move(symbol)) {
    }

private:
    FunctionSymbol::WeakPtr mSymbol;
};

};

}


