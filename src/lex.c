#include <lex.h>
#include <string.h>
#include <lib/vec.h>
#include <ctype.h>

static struct token token_list[] = {
    { .type = TYPE_KEYWORD, .identifier = "if", .value = KEYWORD_IF },
    { .type = TYPE_KEYWORD, .identifier = "while", .value = KEYWORD_WHILE },
    { .type = TYPE_KEYWORD, .identifier = "for", .value = KEYWORD_FOR },
    { .type = TYPE_KEYWORD, .identifier = "asm", .value = KEYWORD_ASM },
    { .type = TYPE_KEYWORD, .identifier = "return", .value = KEYWORD_RETURN },
    { .type = TYPE_KEYWORD, .identifier = "break", .value = KEYWORD_BREAK },
    { .type = TYPE_KEYWORD, .identifier = "continue", .value = KEYWORD_CONTINUE },
    { .type = TYPE_OPERATOR, .identifier = "+", .value = OPERATOR_ADD, .operator.precedence = 2, .operator.associativity = OPERATOR_ASSOC_LEFT },
    { .type = TYPE_OPERATOR, .identifier = "-", .value = OPERATOR_SUB, .operator.precedence = 2, .operator.associativity = OPERATOR_ASSOC_LEFT },
    { .type = TYPE_OPERATOR, .identifier = "*", .value = OPERATOR_MUL, .operator.precedence = 3, .operator.associativity = OPERATOR_ASSOC_LEFT },
    { .type = TYPE_OPERATOR, .identifier = "/", .value = OPERATOR_DIV, .operator.precedence = 3, .operator.associativity = OPERATOR_ASSOC_LEFT },
    { .type = TYPE_OPERATOR, .identifier = "=", .value = OPERATOR_EQUAL },
    { .type = TYPE_OPERATOR, .identifier = "!=", .value = OPERATOR_NOT_EQUAL },
    { .type = TYPE_OPERATOR, .identifier = ">", .value = OPERATOR_GREATER },
    { .type = TYPE_OPERATOR, .identifier = "<", .value = OPERATOR_LESS },
    { .type = TYPE_SEPARATOR, .identifier = "(", .value = SEPARATOR_LEFT_BRACKET },
    { .type = TYPE_SEPARATOR, .identifier = ")", .value = SEPARATOR_RIGHT_BRACKET },
    { .type = TYPE_SEPARATOR, .identifier = "{", .value = SEPARATOR_LEFTC_BRACKET },
    { .type = TYPE_SEPARATOR, .identifier = "}", .value = SEPARATOR_RIGHTC_BRACKET },
    { .type = TYPE_SEPARATOR, .identifier = ";", .value = SEPARATOR_SIMI_COLON },
    { .type = TYPE_SEPARATOR, .identifier = ":", .value = SEPARATOR_COLON },
    { .type = TYPE_DECLARE, .identifier = "uint8 ", .value = TYPE_UINT8 },
    { .type = TYPE_DECLARE, .identifier = "uint16 ", .value = TYPE_UINT16 },
    { .type = TYPE_DECLARE, .identifier = "uint32 ", .value = TYPE_UINT32 },
    { .type = TYPE_DECLARE, .identifier = "uint64 ", .value = TYPE_UINT64 },
    { .type = TYPE_TYPE, .identifier = "uint8", .value = TYPE_UINT8 },
    { .type = TYPE_TYPE, .identifier = "uint16", .value = TYPE_UINT16 },
    { .type = TYPE_TYPE, .identifier = "uint32", .value = TYPE_UINT32 },
    { .type = TYPE_TYPE, .identifier = "uint64", .value = TYPE_UINT64 }
};

static int find_next_token(char *input) {
    char breakpoint = *input;
    const char *str = input;

    if(breakpoint == '\'' || breakpoint == '"') {
        for(str++; *str != breakpoint; str++);
    }

    for(str = input; *str; str++) {
        for(size_t j = 0; j < sizeof(token_list) / sizeof(struct token); j++) {
            if(strncmp(token_list[j].identifier, str, strlen(token_list[j].identifier)) == 0) {
                return (int)(size_t)str - (size_t)input;
            }
        }
    }

    return (int)(size_t)str - (size_t)input;
}

static int token_literal_int(char **input, int cnt, struct token *token) {
    struct token new_token = { .identifier = *input, .type = TYPE_LITERAL, .value = LITERAL_INT };

    for(int i = 0; i < cnt; i++) {
        if(!isdigit((*input)[i])) {
            return -1;
        }
    }

    *token = new_token;

    return 0;
}

static int token_literal_str(char **input, int cnt, struct token *token) {
    struct token new_token = { .identifier = *input + 1, .type = TYPE_LITERAL, .value = LITERAL_STR };

    if((*input)[0] == '"' && (*input)[cnt - 1] == '"') {
        (*input)[cnt - 1] = '\0';
        *token = new_token;
        return 0;
    }
    
    return -1;
}

static int token_literal_char(char **input, int cnt, struct token *token) {
    struct token new_token = { .identifier = *input + 1, .type = TYPE_LITERAL, .value = LITERAL_CHAR };

    if((*input)[0] == '\'' && (*input)[cnt - 1] == '\'') {
        (*input)[cnt - 1] = '\0';
        *token = new_token;
        return 0;
    }

    return -1;
}

static int scan_token(char **input, struct token *ret) {
    for(size_t i = 0; i < sizeof(token_list) / sizeof(struct token); i++) {
        if(strncmp(token_list[i].identifier, *input, strlen(token_list[i].identifier)) == 0) {
            *input += strlen(token_list[i].identifier);
            *ret = token_list[i];
            return 0;
        }
    }

    int cnt = find_next_token(*input);
    if(cnt == -1)
        return -1;

    char *identifier = malloc(cnt + 1);
    strncpy(identifier, *input, cnt);
    identifier[cnt] = '\0';
    *input += cnt;
    
    struct token new_token =  { .type = TYPE_LITERAL, .value = LITERAL_INT };

    if(token_literal_int(&identifier, cnt, &new_token) == 0) {
        goto end;      
    } else if(token_literal_str(&identifier, cnt, &new_token) == 0) {
        goto end;
    } else if(token_literal_char(&identifier, cnt, &new_token) == 0) {
        goto end;
    }

    new_token.type = TYPE_IDENTIFIER;
    new_token.value = TYPE_IDENTIFIER;
    new_token.identifier = identifier;

end:
    *ret = new_token;    
    return 0;
}

int tokenize(char *input, struct token **ret) {
    vec(struct token, token_list);

    while(*input) {
        struct token new_token;
        if(scan_token(&input, &new_token) == -1)
            break;

        vec_push(struct token, token_list, new_token);
    }

    struct token terminator = { .type = TERMINATOR, .value = TERMINATOR };
    vec_push(struct token, token_list, terminator);

    *ret = token_list.data;
    return token_list.element_cnt;
}
