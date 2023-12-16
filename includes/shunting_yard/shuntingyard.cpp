#include "shuntingyard.h"

void infix_to_postfix(vector<string> &infix, queue<Token *> &postfix_queue)
{
    // For debugging
    cout << "Here is the infix expression: ";
    for (int i = 0; i < infix.size(); i++)
    {
        cout << infix[i] << " ";
    }
    cout << endl;

    stack<Token *> tokens_stack;
    string token;

    // Go through each character of the infix and create a Token
    //  based on its type
    for (int i = 0; i < infix.size(); i++)
    {
        token = infix[i];

        // Adding tokens to the stack and queue
        if (token == "<" || token == ">" || token == "<=" || token == ">=" || token == "=")
        {
            if (!tokens_stack.empty())
            {
                Token *temp = tokens_stack.top();

                // While this token is bigger than whats on the stack, pop the smaller guy to the queue
                while (temp->type() <= RELATIONAL && temp->type() != LPAREN)
                {
                    tokens_stack.pop();
                    postfix_queue.push(temp);

                    if (!tokens_stack.empty())
                    {
                        temp = tokens_stack.top();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            tokens_stack.push(new Relational(token));
        }
        else if (token == "or")
        {
            if (!tokens_stack.empty())
            {
                Token *temp = tokens_stack.top();

                // While this token is bigger than whats on the stack, pop the smaller guy to the queue
                while (temp->type() <= LOGICAL_OR && temp->type() != LPAREN)
                {
                    tokens_stack.pop();
                    postfix_queue.push(temp);

                    if (!tokens_stack.empty())
                    {
                        temp = tokens_stack.top();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            tokens_stack.push(new Logical(token));
        }
        else if (token == "and")
        {
            if (!tokens_stack.empty())
            {
                Token *temp = tokens_stack.top();

                // While this token is bigger than whats on the stack, pop the smaller guy to the queue
                while (temp->type() <= LOGICAL_AND && temp->type() != LPAREN)
                {
                    tokens_stack.pop();
                    postfix_queue.push(temp);

                    if (!tokens_stack.empty()){
                        temp = tokens_stack.top();
                    }
                    else{
                        break;
                    }
                }
            }
            tokens_stack.push(new Logical(token));
        }
        else if (token == "(" || token == ")")
        {
            // Token is a parenthesis
            if (token == "(")
            {
                // This token can sit on any other token in the stack
                //  it can also be sat on top of by any other token
                tokens_stack.push(new Left_parenthesis(token));
            }
            // Token specifically a right parenthesis
            else
            {
                // Stack is not empty and SHOULD contain a matching left parenthesis
                //  pop all the items until you get to the matching left paren
                Token *temp = tokens_stack.top();
                tokens_stack.pop();

                // While this token is bigger than whats on the stack, pop the smaller guy to the queue
                while (temp->type() != LPAREN && !tokens_stack.empty())
                {
                    postfix_queue.push(temp);
                    temp = tokens_stack.top();
                    tokens_stack.pop();
                }
            }
        }
        else
        { // Regular tokens go straight to the queue
            postfix_queue.push(new TokenStr(token));

            cout << postfix_queue.size() << endl;

            Token *temp = postfix_queue.front();
            
            cout << temp->type() << endl;
            cout << temp->token_str() << endl;
            cout << "End of this token" << endl;
        }
    }
    // Infix string is done iterating, pop everything from the stack onto the queue
    while (!tokens_stack.empty())
    {
        Token *token = tokens_stack.top();
        tokens_stack.pop();
        postfix_queue.push(token);
    }
}