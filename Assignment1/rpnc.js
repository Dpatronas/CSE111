// Node.js / JavaScript Reverse Polish Notation (RPN) Calculator
// Accepts an single argument in RPN, evaluates it and prints the answer to stdout.

// Despina Patronas
// CSE111 Summer 21

// Citation: 
// Split string: https://nodejs.org/en/knowledge/command-line/how-to-parse-command-line-arguments/
// General:      https://www.w3schools.com/jsref/jsref_foreach.asp

var expression = process.argv.slice(2); //Grab CLI
var stack = [];

var expr = expression.toString().split(' ');
expr.pop();             // remove empty index at the end
expr.forEach(check);    // Iterate through all elements of the list

var op2;                // Hold second argument popped off list
function check(op) {
    switch (op) {
        case "+":
            stack.push( stack.pop() + stack.pop() );
            break;
        case "-":
            op2 = stack.pop();
            stack.push( stack.pop() - op2);
            break;
        case "*": 
            stack.push( stack.pop() * stack.pop());            
            break;
        case "/": 
            op2 = stack.pop();
            stack.push( stack.pop() / op2);
            break;
        case "^": 
            op2 = stack.pop();
            stack.push( Math.pow(stack.pop(), op2) );
            break;
        default:
            stack.push(parseInt(op));
    }
}
console.log(stack[0]);
