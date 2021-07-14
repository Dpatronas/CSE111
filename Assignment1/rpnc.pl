#!/usr/bin/perl -w
use strict;
use warnings;

# Perl Reverse Polish Notation (RPN) Calculator
# Accepts an single argument in RPN, evaluates it and prints the answer to stdout.

# Despina Patronas
# CSE111 Summer 21

# Citation: 
# https://www.cyberciti.biz/faq/howto-pass-perl-command-line-arguments/
# https://www.learn-perl.org/en/Arrays
# https://www.perl.com/article/121/2014/10/24/Perl-s-versatile-split-function/

my @stack;  # Keep track of Stack
my @ops;    # Hold the operations

foreach my $expr(@ARGV) {
    @ops = split ' ', $expr;
}

foreach my $op(@ops) {
    # Operators
    if ($op eq "+") {
        push(@stack, (pop(@stack) + pop(@stack)) );
    }
    elsif ($op eq "-") {
        my $var2 = pop(@stack);
        push(@stack, (pop(@stack) - $var2) );
    }
    elsif ($op eq "*") {
        push(@stack, (pop(@stack) * pop(@stack)) );
    }
    elsif ($op eq "/") {
        my $var2 = pop(@stack);
        push(@stack, (pop(@stack) / $var2) );       
    }
    elsif ($op eq "^") {
        my $var2 = pop(@stack);
        push(@stack, (pop(@stack) ** $var2) );  
    }
    else { # Push operand
        push(@stack, $op)
    }
}
print "$stack[0]\n";
