#include <iostream>
// #include <stdio.h>
// #include <stdlib.h>

using namespace std;

class stack
{
public:
    stack();
    void push(double);
    double pop();
    void init();
    void list();
    int size();
private:
    struct element
    {
        element *next;
        double value;
    };
    int length;
    element *top;
};

stack::stack()
{
    length = 0;
    top = new element; // = (element *) malloc(sizeof(element));
    top->value = 0;
    top->next = NULL;
    return;
}

void stack::push(double val)
{
    element *new_elem;
    new_elem = new element; // = (element *) malloc(sizeof(element));
    new_elem->value = val;
    new_elem->next = top;
    top = new_elem;
    length++;
    return;
}

double stack::pop()
{
    element *new_top;
    double val = 0;
    if (length > 0)
    {
        val = top->value;
        new_top = top->next;
        delete top; // free(top);
        top = new_top;
        length--;
    }
    return val;
}

void stack::list()
{
    element *elem;
    elem = top;
    // cout << "------------------------" << endl; // printf("------------------------\n");
    while (elem->next != NULL)
    {
        cout << elem->value << endl; // printf("%lf\n", elem->value);
        elem = elem->next;
    }
    // cout << "------------------------" << endl; // printf("------------------------\n");
    return;
}

void stack::init()
{
    while (length > 0)
    {
        pop();
    }
    return;
}

int stack::size()
{
    return length;
}

