#!/usr/bin/env python3
import re
import sys


def grammar_generator(reciepe):
    grammar = open("grammar/" + reciepe, 'r').read()

    separator = '// InjectedTokens\n'

    if len(grammar.split(separator)) == 2:
        tokens_array = grammar.split(separator)[1].split(';\n')
        tokens = {}
        for token in tokens_array:
            if len(token) != 0:
                stripped_token = strip(token, "'")
                token_key = stripped_token.split(':')[0]
                token_value = stripped_token.split(':')[1]
                tokens[token_key] = token_value
        grammar = grammar.split('// InjectedTokens\n')[0]
        old_grammar = grammar
        while True:
            for token in tokens:
                grammar = re.sub(token, tokens[token], grammar)
            if grammar == old_grammar:
                break
            old_grammar = grammar

    caramel = open("grammar/Caramel.g4", 'w')
    caramel.write(grammar)


def strip(str, quote):
    escaped = False
    result = ''
    for c in str:
        if c == ' ' or c == '\t':
            if escaped:
                result += c
        else:
            if c == quote:
                escaped = not escaped
            result += c
    return result


if __name__ == '__main__':
    grammar_generator(sys.argv[1])
