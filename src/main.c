#include <lex.h>
#include <parser.h>
#include <stdio.h>

int main() {
    struct token *token_list = NULL;
//    char *input = "(2*(3+1)+420)/2";
    char *input = "uint32 bruh=420-69;uint64 lmao=69-420;}";
    int cnt = tokenize(input, &token_list);
    printf("%d\n", cnt); 
    for(int i = 0; i < cnt; i++) {
        printf("[%d] [identifier] %s [type] %d\n", i, token_list[i].identifier, token_list[i].value);
    }

/*    int b = infix_to_rpn(&token_list, cnt);
    for(int i = 0; i < b; i++)
        printf("%s ", token_list[i].identifier);

    printf("Result: %d", eval_rpn(token_list, cnt));*/

    struct absolute_statement *statements = NULL;
    int statement_cnt = 0;
    parse_scope(token_list, &statements, &statement_cnt);

    for(int i = 0; i < statement_cnt; i++) {
        if(statements[i].var) {
            printf("Variable: %s %d %ld\n", statements[i].var->identifier, statements[i].var->type, statements[i].var->token_cnt);
        } else if(statements[i].assignment) { 

        }
    }
        printf("%lx\n", (size_t)statements);
}
