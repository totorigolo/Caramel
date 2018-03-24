//
// Created by aurore on 23/03/18.
//

#include "ArraySymbol.h"

Caramel::DataStructure::ArraySymbol::ArraySymbol(const std::string &mName,
                                                 const Caramel::DataStructure::PrimaryType::Ptr &mType, const long &mSize)
        : VariableSymbol(mName, mType), mSize(mSize) {

}

void Caramel::DataStructure::ArraySymbol::setSize(const long &size) {
    this->mSize=size;

}

void Caramel::DataStructure::ArraySymbol::getSize() {
    return mSize;

}

