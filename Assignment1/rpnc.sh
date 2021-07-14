# Bash Reverse Polish Notation (RPN) Calculator
# Accepts an single argument a RPN, evaluates it and prints the answer to stdout.

# Despina Patronas
# CSE111 Summer 21

# Citation
# https://how-to.fandom.com/wiki/How_to_read_command_line_arguments_in_a_bash_script
# https://tldp.org/LDP/abs/html/declareref.html
# https://ryanstutorials.net/bash-scripting-tutorial/bash-if-statements.php
# https://stackoverflow.com/questions/1469849/how-to-split-one-string-into-multiple-strings-separated-by-at-least-one-space-in
# https://www.tutorialkart.com/bash-shell-scripting/bash-array/
# https://unix.stackexchange.com/questions/232384/argument-string-to-integer-in-bash/232386
# https://linuxconfig.org/how-to-make-decimal-calculations-in-bash-using-bc

set -f                  # Ignore * for multiplcation
declare expr="$1"       # CLI expression
declare -a split_expr   # Delimitted expression
declare -a stack        # Hold the current stack
cursor=0                # Iterates through elements

pop() { # Func to Pop operands
    op2=${stack[--cursor]}
    op1=${stack[--cursor]}
}

for elem in $expr # Delimit the expression by whitespace
do
    split_expr[cursor++]=$elem
done

cursor=0 # reset cursor

# Perform Post Fix Calculations
for op in "${split_expr[@]}"
do
    if [ "$op" == "+" ];
    then
        pop             # Arithmetic (())
        result=$(echo "$op1 + $op2" | bc -l)
        stack[cursor++]="${result}"
    elif [ "$op" == "-" ];
    then
        pop
        result=$(echo "$op1 - $op2" | bc -l)
        stack[cursor++]="${result}"
    elif [ "$op" == "*" ];
    then
        pop
        result=$(echo "$op1 * $op2" | bc -l)
        stack[cursor++]="${result}"   
    elif [ "$op" == "/" ];
    then
        pop
        result=$(echo "$op1 / $op2" | bc -l)
        stack[cursor++]="${result}"
    elif [ "$op" == "^" ];
    then
        pop # Note pow is ** for bash, ^ for bc
        result=$(echo "$op1 ^ $op2" | bc -l)
        stack[cursor++]="${result}"
    else # Operand
        stack[cursor++]=$op # Note this is a string
    fi
done
echo ${stack[0]}
set +f
