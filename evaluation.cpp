#include <cstdio>
#include <iostream>
#include <string>
#include <string.h>
#include <memory.h>
using namespace std;

#ifdef _WIN32
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

union ret_value
{
    int int_val;
    float float_val;
    ret_value()
    {
    }
    ret_value(int a)
    {
        int_val = a;
    }
    ret_value(float a)
    {
        float_val = a;
    }
};

/*
struct num
{
    ret_value val;
    int tp;
    num() { tp = -1; }
    num(char *c)
    {
        int tp = 1, neg = 0; //0 for int, >=1 for float
        int i_v = 0;
        if (c[0] == '-')
            neg = 1;
        for (int i = neg; c[i]; i++)
        {
            if (c[i] == '.')
            {
                if (tp == 0)
                    tp = 1;
                continue;
            }
            if (tp >= 1)
                tp *= 10;
            i_v = i_v * 10 + c[i] - '0';
        }
        if (neg == 1)
            neg = -1;
        else
            neg = 1;
        if (tp > 0)
            val.float_val = neg * i_v / tp;
        else
            val.int_val = neg * i_v;
        this->tp = tp > 0;
    }
    num(int a)
    {
        tp = 0;
        val.int_val = a;
    }
    num(float a)
    {
        tp = 1;
        val.float_val = a;
    }
    num(const num& a)
    {
        val=a.val;
        tp=a.tp;
    }
};*/

typedef int num;

num operate_it(const num &a, const num &b, char *op)
{
    if (strcmp(op, "+") == 0)
        return a + b;
    else if (strcmp(op, "-") == 0)
        return a - b;
    else if (strcmp(op, "*") == 0)
        return a * b;
    else if (strcmp(op, "/") == 0)
        return a / b;
    else if (strcmp(op, "%") == 0)
        return a % b;
    else if (strcmp(op, "^") == 0)
    {
        num ans = a;
        for (int i = 0; i < b; i++)
        {
            ans *= a;
        }
        return ans;
    }
    else if (strcmp(op, "min") == 0)
        return a < b ? a : b;
    else if (strcmp(op, "max") == 0)
        return a < b ? b : a;
    else
        throw;
}

num solve_expr(mpc_ast_t *node)
{
    if (strstr(node->tag, "number"))
        return atoi(node->contents);

    bool minus = strcmp(node->children[1]->contents, "-") == 0;
    num res = minus ? 0 : solve_expr(node->children[2]);

    for (int i = minus ? 2 : 3; i < node->children_num - 1; i++)
        res = operate_it(res, solve_expr(node->children[i]), node->children[1]->contents);

    return res;
}

num solve_lispy(mpc_ast_t *node)
{
    if (strstr(node->children[1]->tag, "number"))
        return atoi(node->children[1]->contents);

    return solve_expr(node);
}

int main(int argc, char **argv)
{
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");
    mpca_lang(MPCA_LANG_DEFAULT,
              "\
    number : /-?[0-9]+/; \
    operator : '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\"; \
    expr : <number> | '(' <operator> <expr>+ ')'; \
    lispy : /^/ <number> /$/ | /^/ <operator> <expr>+ /$/; \
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
            //mpc_ast_print((mpc_ast_t *)res.output);
            cout << solve_lispy((mpc_ast_t *)res.output) << endl;
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