#include <stdio.h>
#include <lib/vec.h>
#include <lex.h>

size_t eval_rpn(struct token *token_list, int token_cnt) {
    create_stack(size_t, stack);

    for(int i = 0; i < token_cnt; i++) {
        if(token_list[i].value == LITERAL_INT) {
            size_t value = strtol(token_list[i].identifier, (void*)NULL, 10);
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

void parse_expression(struct token *token_list, int token_cnt) {

}
