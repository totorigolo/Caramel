#include <iostream>

#include <antlr4-runtime.h>
#include <CaramelLexer.h>
#include <CaramelParser.h>
#include "listeners/DotExportListener.h"

using namespace antlr4;
using namespace antlrcpp;
using namespace std;

int main() {

//    ANTLRInputStream input("2+(6*4)");
//    HelloLexer lexer(&input);
//    CommonTokenStream tokens(&lexer);
//
//    tokens.fill();
//
//    for (auto token : tokens.getTokens()) {
//        std::cout << token->toString() << std::endl;
//    }
//
//    HelloParser parser(&tokens);
//    tree::ParseTree* tree = parser.expr();
//
//    std::cout << tree->toStringTree(&parser) << std::endl << std::endl;
//
//    CalcVisitor visitor;
//    std::cout << "Résultat: " << (int) visitor.visit(tree) << std::endl;


    string path;
    cout << "Indiquez le chemin vers le fichier :" << endl;

    getline(cin, path);

//    string data;
    ifstream fileinput(path);

//    fileinput >> data;

    ANTLRInputStream input(fileinput);
    CaramelLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();

    for (auto const & token: tokens.getTokens()) {
        cout << token->toString() << endl;
    }

    CaramelParser parser(&tokens);
    tree::ParseTree* tree = parser.r();

    cout << tree->toStringTree(&parser) << endl;

    Listeners::DotExportListener dotExport(&parser);
    tree::ParseTreeWalker::DEFAULT.walk(&dotExport, tree);

    ofstream out;
    out.open("out.dot");
    out << dotExport.getDotFile();
    out.close();

    return 0;
}