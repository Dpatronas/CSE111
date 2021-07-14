# Awk Reverse Polish Notation (RPN) Calculator
# Accepts an single argument in RPN, evaluates it and prints the answer to stdout.

# Despina Patronas
# CSE111 Summer 21

# Citation:
# https://www.gnu.org/software/gawk/manual/html_node/ARGC-and-ARGV.html
# https://www.grymoire.com/Unix/Awk.html#uh-1
# http://kirste.userpage.fu-berlin.de/chemnet/use/info/gawk/gawk_12.html

# Stack functions
function push(op) {
    stack[cursor++] = op;
}

function pop() {
    return stack[--cursor];
}

BEGIN {
    expr[""] = 0;
    stack[""] = 0;
    cursor = 0;

    split(ARGV[1], expr, " ");  # Take out the elements of expr into an array

    for ( i in expr) {
        if (expr[i] == "+") {
            push(pop() + pop());
        }
        else if (expr[i] == "-") {
            op2 = pop();
            push(pop() - op2);
        }
        else if (expr[i] == "*") {
            push(pop() * pop());
        }
        else if (expr[i] == "/") {
            op2 = pop();
            push(pop() / op2);
        }
        else if (expr[i] == "^") {
            op2 = pop();
            push(pop() ** op2);
        }
        else {
            push(strtonum(expr[i]));
        }
    }
    printf "%g", stack[0];
}
