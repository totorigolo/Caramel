//
// Created by aurore on 03/04/18.
//

#include "GTEInstruction.h"
#include "../IRVisitor.h"

namespace caramel::ir {

GTEInstruction::GTEInstruction(
        std::string const &returnName,
        const std::shared_ptr<caramel::ir::BasicBlock> &parentBlock,
        const caramel::ast::PrimaryType::Ptr &type
) : IR(returnName,
       Operation::cmp_ge, parentBlock,
       type
) {}

void GTEInstruction::accept(std::shared_ptr<IRVisitor> const &visitor, std::ostream &os) {
    visitor->visitGTE(this, os);
}


}