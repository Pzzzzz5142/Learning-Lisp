#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#ifdef _WIN32
#include <string.h>
char *readline(const char *prompt)
{
    cout << prompt;
    static string buffer;
    cin.flush();
    getline(cin, buffer);
    static char *c_buffer;
    c_buffer = (char *)malloc(sizeof(char) * buffer.size());
    strcpy(c_buffer, buffer.c_str());
    return c_buffer;
}
void add_history(const char *_)
{
}
#else
#include <editline/readline.h>
#endif

extern "C"
{
#include "mpc.h"
}

int main(int argc, char **argv)
{
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");
    mpca_lang(MPCA_LANG_DEFAULT,
              "\
    number : /-?[0-9]+(:?\\.[0-9]+)?/; \
    operator : '+' | '-' | '*' | '/' | '%'; \
    expr : <number> | '(' <operator> <expr>+ ')';\
    lispy : /^/ <operator> <expr>+ /$/;\
    ",
              Number, Operator, Expr, Lispy);

    cout << "Very first beginning prompt for Lispy" << endl;
    while (1)
    {
        char *input = readline("Haha> ");

        add_history(input);

        mpc_result_t res;
        if (mpc_parse("<stdin>", input, Lispy, &res))
        {
            mpc_ast_print((mpc_ast_t *)res.output);
            mpc_ast_delete((mpc_ast_t *)res.output); // need to delete here
        }
        else
        {
            mpc_err_print(res.error);
            mpc_err_delete(res.error); // need to delete here
        }

        free(input);
    }

    mpc_cleanup(4, Number, Operator, Expr, Lispy);
}