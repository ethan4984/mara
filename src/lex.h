#ifndef LEX_H_
#define LEX_H_

#include <stdint.h>
#include <stddef.h>

enum {
    TYPE_IDENTIFIER,
    TYPE_KEYWORD,
    TYPE_SEPARATOR,
    TYPE_OPERATOR,
    TYPE_LITERAL,
    TYPE_IDENT,
    TYPE_DECLARE,
    TYPE_TYPE,

    KEYWORD_IF,
    KEYWORD_WHILE,
    KEYWORD_FOR,
    KEYWORD_ASM,
    KEYWORD_RETURN,
    KEYWORD_BREAK,
    KEYWORD_CONTINUE,

    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,

    SEPARATOR_LEFT_BRACKET,
    SEPARATOR_RIGHT_BRACKET,
    SEPARATOR_LEFTC_BRACKET,
    SEPARATOR_RIGHTC_BRACKET,
    SEPARATOR_SIMI_COLON, 
    SEPARATOR_COLON, 

    LITERAL_CHAR,
    LITERAL_INT,
    LITERAL_STR,
    LITERAL_FLOAT,

    OPERATOR_ADD,
    OPERATOR_SUB,
    OPERATOR_MUL,
    OPERATOR_DIV,
    OPERATOR_EQUAL,
    OPERATOR_NOT_EQUAL,
    OPERATOR_GREATER,
    OPERATOR_LESS,

    OPERATOR_ASSOC_RIGHT,
    OPERATOR_ASSOC_LEFT
};

struct token {
    const char *identifier;
    int type;
    int value;
    struct {
        int precedence;
        int associativity;
    } operator;
};

int tokenize(char *input, struct token **ret);

#endif
