#ifndef PARSE_H_
#define PARSER_H_

#include <stdint.h>
#include <stddef.h>

struct var_declaration {
    const char *identifier;
    int type;

    struct token *initaliser;
    size_t token_cnt;
};

struct assignment {
    struct var_declaration var;

    struct token *initaliser;
    size_t token_cnt;
};

void parse_expression(struct token **token_list, int *depth);

#endif
