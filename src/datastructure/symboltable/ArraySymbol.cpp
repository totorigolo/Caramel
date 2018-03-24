//
// Created by aurore on 23/03/18.
//

#include "ArraySymbol.h"

namespace caramel::dataStructure::symbolTable {

ArraySymbol::ArraySymbol(
        const std::string &mName,
        const PrimaryType::Ptr &mType,
        const long &mSize
)
        : VariableSymbol(mName, mType), mSize(mSize) {

}

void ArraySymbol::setSize(const long &size) {
    this->mSize = size;

}

long ArraySymbol::getSize() {
    return mSize;

}

} // namespace caramel::dataStructure::symbolTable

