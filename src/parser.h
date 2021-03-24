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
    struct var_declaration *var;

    struct token *initaliser;
    size_t token_cnt;
};

struct absolute_statement {
    struct var_declaration *var;
    struct assignment *assignment;
};

void parse_expression(struct token **token_list, int *depth, struct absolute_statement **ret, struct absolute_statement *statement_list, int statemnet_cnt);
void ssa_init(struct absolute_statement **list, int *statement_cnt);

#endif
