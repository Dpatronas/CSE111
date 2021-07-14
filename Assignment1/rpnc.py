import sys

# Python3 Reverse Polish Notation (RPN) Calculator
# Accepts an single argument in RPN, evaluates it and prints the answer to stdout.

# Despina Patronas
# CSE111 Summer 21

# Citation: 
# http://net-informations.com/python/file/split.htm

# Stack Functions
def push(operand):
    stack.append(float(operand))

def pop():
    return stack.pop()

# Operator Functions
def plus():
    push(pop() + pop())

def minus():
    op2 = pop()
    push(pop() - op2)

def multi():
    push(pop() * pop())

def divid():
    op2 = pop()
    push(pop() / op2)

def pow():
    op2 = pop()
    push(pop() ** op2)

stack = []
expression = sys.argv[1].split()   # grab CLI

# Perform Post Fix Calculations
for i in expression:
    if   i == "+": plus()
    elif i == "-": minus()
    elif i == "*": multi()
    elif i == "/": divid()
    elif i == "^": pow()
    else: push(i)

print("%g" % stack[0])
