#include <stdio.h>
#include <parser.h>
#include <lib/vec.h>
#include <lex.h>

size_t eval_rpn(struct token *token_list, int token_cnt) {
    create_stack(size_t, stack);

    for(int i = 0; i < token_cnt; i++) {
        if(token_list[i].value == LITERAL_INT) {
            size_t value = strtol(token_list[i].identifier, NULL, 10);
            stack_push(size_t, stack, value);
        } else if(token_list[i].type == TYPE_OPERATOR) {
            size_t elem1 = stack_pop(size_t, stack);
            size_t elem2 = stack_pop(size_t, stack);

            size_t result = 0;
            switch(token_list[i].value) {
                case OPERATOR_ADD:
                    result = elem1 + elem2;
                    break;
                case OPERATOR_SUB:
                    result = elem2 - elem1;
                    break;
                case OPERATOR_MUL:
                    result = elem1 * elem2;
                    break;
                case OPERATOR_DIV:
                    result = elem2 / elem1;
                    break; 
                default:
                    printf("Unknown operator %s\n", token_list[i].identifier);
            }

            stack_push(size_t, stack, result);
        }
    }

    size_t result = stack_pop(size_t, stack);
    stack_delete(stack); 
    return result;
}

int infix_to_rpn(struct token **token_list, int token_cnt) {
    create_stack(struct token, output_stack);
    create_stack(struct token, operator_stack);

    for(int i = 0; i < token_cnt; i++) {
        if((*token_list)[i].value == LITERAL_INT || (*token_list)[i].type == TYPE_DECLARE) {
            stack_push(struct token, output_stack, (*token_list)[i]);
        } else if((*token_list)[i].type == TYPE_OPERATOR) {
            while(  (operator_stack.data.element_cnt > 0) &&
                    (stack_view(struct token, operator_stack).operator.precedence >= (*token_list)[i].operator.precedence))
                stack_push(struct token, output_stack, stack_pop(struct token, operator_stack));

            stack_push(struct token, operator_stack, (*token_list)[i]);
        } else if((*token_list)[i].value == SEPARATOR_LEFT_BRACKET) {
            stack_push(struct token, operator_stack, (*token_list)[i]);
        } else if((*token_list)[i].value == SEPARATOR_RIGHT_BRACKET) {
            while(stack_view(struct token, operator_stack).value != SEPARATOR_LEFT_BRACKET)
                stack_push(struct token, output_stack, stack_pop(struct token, operator_stack));
            if(stack_view(struct token, operator_stack).value == SEPARATOR_LEFT_BRACKET)
                stack_pop(struct token, operator_stack);
        }
    }

    for(size_t i = 0; i < operator_stack.data.element_cnt; i++) {
        stack_push(struct token, output_stack, stack_pop(struct token, operator_stack));
    }

    stack_delete(operator_stack);

    *token_list = output_stack.data.data;
    return output_stack.data.element_cnt;
}

struct absolute_statement *parse_declare(struct token **token_list, int *depth) {
    struct var_declaration *var = malloc(sizeof(struct var_declaration));
    struct absolute_statement *abs_expr = malloc(sizeof(struct absolute_statement));

    abs_expr->var = var;

    if( token_list[0]->type != TYPE_DECLARE || 
        (token_list[0]->value != TYPE_UINT8 && 
        token_list[0]->value != TYPE_UINT16 &&
        token_list[0]->value != TYPE_UINT32 &&
        token_list[0]->value != TYPE_UINT64)) {
        printf("[ERROR] unrecognised type\n");
        goto error;
    }

    var->type = token_list[0]->value;
    (*token_list)++;

    if(token_list[0]->type == TYPE_IDENTIFIER && token_list[0]->value == TYPE_IDENTIFIER) {
        var->identifier = token_list[0]->identifier; 
        (*token_list)++;
    } else {
        printf("[ERROR] unrecognised identifier\n"); 
        goto error;
    }

    if(token_list[0]->type == TYPE_OPERATOR && token_list[0]->value == OPERATOR_EQUAL) {
        (*token_list)++;
    } else {
        printf("[ERROR] unrecognised operator\n"); 
        goto error;
    }

    int cnt = 0;
    for(; token_list[0]->type != TYPE_SEPARATOR && token_list[0]->value != SEPARATOR_SIMI_COLON; cnt++, (*token_list)++);
    (*token_list)++;

    var->initaliser = *token_list - cnt;
    var->token_cnt = cnt;

    return abs_expr;

error:
    free(var);
    free(abs_expr);

    exit(0);
}

void parse_scope(struct token *token_list, struct absolute_statement **ret, int *ret_cnt) { 
    int depth = 1;
    vec(struct absolute_statement*, statement_list);

    while(depth || (token_list->type != TERMINATOR && token_list->value != TERMINATOR)) {
        struct absolute_statement *abs_expr = NULL;
        parse_expression(&token_list, &depth, &abs_expr);
        if(abs_expr == NULL) {
            break;
        }

        vec_push(struct absolute_statement, statement_list, abs_expr);
    }

    *ret = *statement_list.data;
    *ret_cnt = statement_list.element_cnt;
}

void parse_expression(struct token **token_list, int *depth, struct absolute_statement **ret) {
    printf("Here lol parsing token %s\n", token_list[0]->identifier);
    switch(token_list[0]->type) {
        case TYPE_DECLARE:
            if( token_list[0]->value == TYPE_UINT8 ||
                token_list[0]->value == TYPE_UINT16 ||
                token_list[0]->value == TYPE_UINT32 ||
                token_list[0]->value == TYPE_UINT64) {
                *ret = parse_declare(token_list, depth); 
            }
            else {
                printf("[ERROR] unrecognized type");
                exit(0);
            }
            break;
        case TYPE_KEYWORD:
            switch(token_list[0]->value) { 
                case KEYWORD_IF:
                    break;
                case KEYWORD_WHILE:
                    break;
                case KEYWORD_FOR:
                    break;
                case KEYWORD_ASM:
                    break;
                case KEYWORD_RETURN:
                    break;
                case KEYWORD_BREAK:
                    break;
                case KEYWORD_CONTINUE:
                    break;
            }
            break;
        case TYPE_SEPARATOR:
            switch(token_list[0]->value) {
                case SEPARATOR_RIGHTC_BRACKET:
                    (*depth)--;
                    break;
                case SEPARATOR_LEFTC_BRACKET:
                    (*depth)++;
            }
            break;
        case TERMINATOR:
            if(token_list[0]->value == TERMINATOR)
                return;
            __attribute__((fallthrough));
        default:
            printf("[ERROR] unrecognized parse token\n");
            exit(0);
    }
}
