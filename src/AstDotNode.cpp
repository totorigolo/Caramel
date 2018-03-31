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

#include "AstDotNode.h"


namespace caramel {

std::stringstream AstDotNode::sNodes;
std::stringstream AstDotNode::sEdges;
std::set<size_t> AstDotNode::sAddedNodes;
std::map<size_t, std::set<size_t>> AstDotNode::sAddedEdges;

void caramel::AstDotNode::addNode(size_t id, const std::string &name,
                                  const std::string &shape, const std::string &color) {
    if (sAddedNodes.find(id) != sAddedNodes.end()) return;

    AstDotNode::sNodes << "\tnode" << id << "[label=\"" << name
                       << "\", shape=" + shape + ", fillcolor=" + color + "]\n";

    sAddedNodes.insert(id);
}

void caramel::AstDotNode::addErrorNode(size_t id, const std::string &name, const std::string &errorMessage) {
    if (sAddedNodes.find(id) != sAddedNodes.end()) return;

    AstDotNode::sNodes << "\tnode" << id << "[label=\"" << name << "\", style=filled, fillcolor=red]\n";
    auto errorId = size_t(&errorMessage[0]); // Same error messages will have the same ID
    addEdge(id, errorId, "error");
    addNode(errorId, errorMessage);

    sAddedNodes.insert(id);
}

void caramel::AstDotNode::addEdge(size_t id1, size_t id2) {
    if (sAddedEdges[id1].find(id2) != sAddedEdges[id1].end()) return;

    AstDotNode::sEdges << "\tnode" << id1 << " -> " << "node" << id2 << ";\n";

    sAddedEdges[id1].insert(id2);
}

void caramel::AstDotNode::addEdge(size_t id1, size_t id2, const std::string &label) {
    if (sAddedEdges[id1].find(id2) != sAddedEdges[id1].end()) return;

    AstDotNode::sEdges << "\tnode" << id1 << " -> " << "node" << id2 << " [label=" + label + "];\n";

    sAddedEdges[id1].insert(id2);
}

std::string caramel::AstDotNode::getDotFile() {
    acceptAstDotVisit();
    return "digraph {\n"
           "\tordering=out;\n"
           "\tranksep=.4;\n"
           "\tnode [shape=box, fixedsize=false, fontsize=12, fontname=\"Helvetica\", fontcolor=\"blue\"\n"
           "\t\twidth=.25, height=.25, color=\"black\", fillcolor=\"lightgrey\", style=\"filled, solid\"];\n"
           "\tedge [arrowsize=.5, color=\"black\"]\n\n" + AstDotNode::sNodes.str() + AstDotNode::sEdges.str() + "}\n";
}

size_t caramel::AstDotNode::thisId() const {
    return size_t(this);
}

} // namespace caramel
