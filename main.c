#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_EXPRESSION_LENGTH 1024 // yeah

typedef struct
{
    double data[MAX_EXPRESSION_LENGTH];
    int top;
} Stack; // Shunting Yard stack

// range of x-values to plot
const double x_min = -10;
const double x_max = 10;
const double step = 0.1;

void push(Stack* stack, double item)
{
    if(stack->top < MAX_EXPRESSION_LENGTH - 1)
        stack->data[++(stack->top)] = item;

    else
    {
        printf("Stack overflow\n");
        exit(1);
    }
}

double pop(Stack* stack)
{
    if(stack->top >= 0) return stack->data[(stack->top)--];

    else
    {
        printf("Stack underflow\n");
        exit(1);
    }
}

// peek the top element of the stack without popping it
double top(Stack* stack)
{
    if(stack->top >= 0) return stack->data[stack->top];
    else return /*idk*/ 0; // empty stack
}

int is_operator(double c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int is_digit(double c)
{
    if(c >= '0' && c <= '9') return 1;
    return 0;
}

int get_precedence(double operator)
{
    if(operator == '+' || operator == '-') return 1;
    if(operator == '*' || operator == '/') return 2;

    return 0;
}

// use Shunting Yard to convert infix expression to postfix
void shunt(char* infix, char* postfix)
{
    Stack stack;
    stack.top = -1;

    int infix_length = strlen(infix); printf("Infix length: %d\n", infix_length);
    int postfix_index = 0;

    for(int i = 0; i < infix_length; i++)
    {
        printf("Iteration %d\n", i);

        char current = infix[i];

        if(current == ' ') continue; // just skip spaces

        if(is_digit(current)) 
            postfix[postfix_index++] = current;
        
        else if(is_operator(current))
        {
            while(stack.top != -1 && get_precedence(top(&stack)) >= get_precedence(current))
                postfix[postfix_index++] = pop(&stack);

            push(&stack, current);
        }

        else if(current == '(')
            push(&stack, current);

        else if(current == ')') // pop until its opening parenthesis is found
        {
            while(stack.top != -1 && top(&stack) != '(')
                postfix[postfix_index++] = pop(&stack);

            if (stack.top == -1 || top(&stack) != '(')
            {
                printf("Mismatched parentheses\n");
                exit(1);
            }

            pop(&stack); // pop and discard the opening parenthesis
        }
    }

    // pop any remaining operators from the stack and add them to the postfix expression
    while(stack.top != -1)
    {
        if(top(&stack) == '(')
        {
            printf("Mismatched parentheses\n");
            exit(1);
        }
        
        postfix[postfix_index++] = pop(&stack);
    }

    postfix[postfix_index] = 0; // null term it
}

evaluate_postfix(char* postfix)
{
    Stack operand_stack;
    operand_stack.top = -1;
    int postfix_length = strlen(postfix);

    for(int i = 0; i < postfix_length; i++)
    {
        char current = postfix[i];

        if(is_digit(current))
        {
            double number = current - '0';
            push(&operand_stack, number);
        }
    }
}

double evaluate_expression(char* expression, double x)
{
    char postfix_expression[MAX_EXPRESSION_LENGTH] = {0};
    shunt(expression, postfix_expression);
    printf("Postfix expression: %s\n", postfix_expression);

    return evaluate_postfix(postfix_expression);
}

int main()
{
    char input[MAX_EXPRESSION_LENGTH] = {0};
    printf("Enter a function: ");
    fgets(input, MAX_EXPRESSION_LENGTH, stdin);
    input[strlen(input)-1] = 0;

    evaluate_expression(input, 0);

    return 0;
}