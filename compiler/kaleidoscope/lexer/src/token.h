#pragma once

#include <array>
#include <cctype>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>

enum EToken
{
    tok_eof = -1,

    tok_def    = -2,
    tok_extern = -3,

    tok_identifier = -4,
    tok_number     = -5,
};

inline auto token_str_mapping = std::unordered_map<EToken, std::string>{
#define tk(x) {x, #x}
    tk(tok_eof),
    tk(tok_def),
    tk(tok_extern),
    tk(tok_identifier),
    tk(tok_number),
#undef tk
};

struct Token {
    EToken type;

    std::string IdentifierStr;
    double      NumVal;

    std::string to_string()
    {
        return

            "TokenType: " +
            (token_str_mapping.find(type) == token_str_mapping.end() ? std ::to_string(type)
                                                                     : token_str_mapping[type]) +
            "\t  IdentStr: " + IdentifierStr + "\t NumberVal: " + std::to_string(NumVal) + '\n';
    }
};

static Token cur_token;

extern char read_char();

static int read_token()
{
    static char last_char = ' ';

    // clear spaces
    while (std::isspace(last_char)) {
        last_char = read_char();
    }

    // string
    if (std::isalpha(last_char)) {
        cur_token.IdentifierStr.clear();
        cur_token.IdentifierStr.push_back(last_char);
        while (std::isalnum(last_char = read_char())) {
            cur_token.IdentifierStr.push_back(last_char);
        }

        if (cur_token.IdentifierStr == "def") {
            return tok_def;
        }
        if (cur_token.IdentifierStr == "extern") {
            return tok_extern;
        }
        return tok_identifier;
    }

    // number
    if (std::isdigit(last_char) || last_char == '.') {
        std::string num_str;
        do {
            num_str += last_char;
            last_char = read_char();
        } while (std::isdigit(last_char) || last_char == '.');

        cur_token.NumVal = strtof(num_str.c_str(), nullptr);
        return tok_number;
    }

    // Comment
    if (last_char == '#') {
        do {
            last_char = read_char();
        } while (last_char != EOF && last_char != '\n' && last_char != '\r');

        // read next line
        if (last_char != EOF) {
            return read_token();
        }
    }

    // Don't eat eof
    if (last_char == EOF) {
        return tok_eof;
    }

    // otherwise, return the character and ascii value
    int this_char = last_char;
    last_char     = read_char();
    return this_char;
}
