#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Reverse Polish Notation (RPN) Calculator
// Accepts a single argument in RPN, evaluates it and prints the answer to stdout.
// Returns -1 on error, 0 otherwise.

// Despina Patronas
// CSE111 Summer 21

// Citation: 
// Prof. Harrison Lecture 1 - July 21

static double stack[16];
static int cursor = 0;

static void push (double operand) {
    stack[cursor++] = operand;
}

static double pop () {
    return stack[--cursor];
}

int main(int argc, char *argv[]) {
    char * oper = strtok(argv[1], " "); //takes the whole expr
    double op2 = 0;
    while(oper != NULL) {
        switch (oper[0]) {  //check if the token is a operator
            case '+':
                push (pop() + pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '*':
                push(pop() * pop());
                break;
            case '/':
                op2 = pop();
                push(pop() / op2);
                break;
            case '^':
                op2 = pop();
                push( pow(pop(),op2) );
                break;
            default:    // current token is a digit..
                push(atof(oper));
                break;
        }
        oper = strtok(NULL, " ");
    }
    printf("%g\n", stack[0]);
    return 0;
}
