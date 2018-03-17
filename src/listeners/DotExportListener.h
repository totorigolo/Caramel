#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <antlr4-runtime.h>

using namespace antlr4;
using namespace std;

namespace Listeners {

class DotExportListener : public tree::ParseTreeListener {
protected:
    Parser *parser;
    stringstream nodes;
    stringstream edges;

    static string cleanStr(string name) {
        auto pos = 0;
        while ((pos = name.find("\"", pos)) != string::npos) {
            name.replace(pos, 1, "\\\"");
            pos += 2;
        }
        return name;
    }

public:
    explicit DotExportListener(Parser *parser) : parser(parser) {}

    void enterEveryRule(ParserRuleContext *ctx) override {
        for (auto i = 0; i < ctx->children.size(); i++) {
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

    void visitErrorNode(tree::ErrorNode *node) override {} // this method needs to be implemented
    void exitEveryRule(ParserRuleContext *ctx) override {} // this method needs to be implemented
};

} // namespace Listeners



