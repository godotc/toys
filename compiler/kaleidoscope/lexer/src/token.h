#pragma once

#include <array>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>
#include <string>

enum EToken
{
    tok_eof = -1,

    tok_def    = -2,
    tok_extern = -3,

    tok_identifier = -4,
    tok_number     = -5,
};

static std::string IdentifierStr;
static double      NumVal;

static int gettok()
{
    static int last_char = ' ';

    while (std::isspace(last_char)) {
        last_char = getchar();
    }

    if (std::isalpha(last_char)) {
        IdentifierStr = last_char;
        while (std::isalnum(last_char = getchar())) {
            IdentifierStr += last_char;
        }

        if (IdentifierStr == "def") {
            return tok_def;
        }
        if (IdentifierStr == "extern") {
            return tok_extern;
        }

        return tok_identifier;
    }

    if (std::isdigit(last_char) || last_char == '.') {
        std::string num_str;
        do {
            num_str += last_char;
            last_char = getchar();
        } while (std::isdigit(last_char) || last_char == '.');

        NumVal = strtof(num_str.c_str(), 0);
        return tok_number;
    }

    if (last_char == '#') {
        // Comment until end of line.
        do {
            last_char = getchar();
        } while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF) {
            return gettok();
        }
    }

    // Don't eat eof
    if (last_char == EOF) {
        return tok_eof;
    }

    // otherwise, return the character and ascii value
    int this_char = last_char;
    last_char     = getchar();
    return this_char;
}
