#pragma once

#include "Common.h"

#include <antlr4-runtime.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>


using namespace antlr4;
using namespace std;

namespace Listeners {

class DotExportListener : public tree::ParseTreeListener {
protected:
    Parser *parser;
    stringstream nodes;
    stringstream edges;

    static string cleanStr(string const &before) {
        string after;
        after.reserve(before.length() + 4);
        for (size_t i = 0UL; i < before.size(); i++) {
            if (before[i] == '"' && (i == 0 || before[i - 1] != '\\')) {
                after += '\\';
            }
            after += before[i];
        }
        return after;
    }

public:
    explicit DotExportListener(Parser *parser) : parser(parser) {}

    void enterEveryRule(ParserRuleContext *ctx) override {
        for (auto i = 0UL; i < ctx->children.size(); i++) {
            if (ctx->children[i]->getText() != " ") {
                edges << "\tnode" << ctx << " -> " << "node" << ctx->children[i] << ";\n";
            }
        }
        nodes << "\tnode" << ctx << "[label=\"" << tree::Trees::getNodeText(ctx, parser) << "\"];\n";
    }

    void visitTerminal(antlr4::tree::TerminalNode *node) override {
        if (cleanStr(node->toString()) != " ") {
            nodes << "\tnode" << node << "[label=\"" << cleanStr(node->toString()) << "\"]\n";
        }
    }

    string getDotFile() {
        return "digraph {\n"
                       "\tordering=out;\n"
                       "\tranksep=.4;\n"
                       "\tnode [shape=box, fixedsize=false, fontsize=12, fontname=\"Helvetica\", fontcolor=\"blue\"\n"
                       "\t\twidth=.25, height=.25, color=\"black\", fillcolor=\"lightgrey\", style=\"filled, solid\"];\n"
                       "\tedge [arrowsize=.5, color=\"black\"]\n\n" + nodes.str() + edges.str() + "}\n";
    }

    void visitErrorNode(tree::ErrorNode *node) override {
        CARAMEL_UNUSED(node);
    }

    void exitEveryRule(ParserRuleContext *ctx) override {
        CARAMEL_UNUSED(ctx);
    }
};

} // namespace Listeners



