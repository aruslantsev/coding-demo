#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define CMDLENGTH 20

typedef struct stack_element
{
    stack_element* next;
    double value;
};

void print_help()
{
    printf("Вычисления с использованием обратной польской записи\nВведите h для помощи, q или e для выхода из программы\ninit для приведения стека в исходное состояние\npush число для добавления числа в стек, pop для удаления элемента из стека,\nsave для сохранения вершины стека без удаления элемента, load для возврата сохраненного значения в стек,\ntop для вывода верхнего элемента стека, list для вывода стека на экран,\nadd для сложения, sub для вычитания, mul для умножения, div для деления,\ninv для смены знака, abs для вычисления абсолютного значения,\nsqroot для извлечения квадратного корня, pow2 для вычисления 2^x, log2 для вычисления логарифма\nrnd для округления до целого,\nxchg для того, чтобы поменять верхние 2 элемента местами\nsin для вычисления синуса, cos для вычисления косинуса, tan для вычисления тангенса,\nasin для вычисления арксинуса, acos для вычисления арккосинуса, atan для вычисления арктангенса,\nsinh для вычисления гиперболического синуса, cosh для вычисления гиперболического косинуса,\nasinh для вычисления гиперболического арксинуса, acohs для вычисления гиперболического арккосинуса\ncmp для сравнения: возвращает в стек 0 если числа равны, 1 если первое число больше, -1 если второе число больше,\ntest возвращает 0 если число равно 0, 1 если число больше 0, -1 если число меньше 0\n");
}

void push_elem(stack_element **top, double val, int *length)
{
    stack_element *new_elem;
    new_elem = (stack_element *) malloc(sizeof(stack_element));
    new_elem->value = val;
    new_elem->next = *top;
    *top = new_elem;
    (*length)++;
    return;
}

double pop_elem(stack_element **top, int *length)
{
    stack_element *new_top;
    double val;
    val = (*top)->value;
    new_top = (*top)->next;
    free(*top);
    *top = new_top;
    (*length)--;
    return val;
}

void list_elem(stack_element *top)
{
    stack_element *elem;
    elem = top;
    printf("------------------------\n");
    while (elem->next != NULL)
    {
        printf("%lf\n", elem->value);
        elem = elem->next;
    }
    printf("------------------------\n");
    return;
}

int main()
{
    stack_element *top;
    int length = 0;
    bool saved = 0;
    char op[CMDLENGTH];
    double val = 0, a, b, s;
    top = (stack_element *) malloc(sizeof(stack_element));
    top->value = 0;
    top->next = NULL;
    print_help();
    do
    {
        if (!strcmp(op, "h"))
            print_help();
        
        if (!strcmp(op, "init")) 
        while (length > 0)
        {
            pop_elem(&top, &length);
        }
        
        if (!strcmp(op, "push"))
        {
            scanf("%lf", &val);
            push_elem(&top, val, &length);            
            list_elem(top);
        }
        
        if (!strcmp(op, "pop")) 
        {
            if (length > 0)
            {
                pop_elem(&top, &length);
                list_elem(top);
            }
            else
                printf("Стек пуст.\n");
        }
        
        if (!strcmp(op, "save")) 
        {
            if (length > 0)
            {
                s = pop_elem(&top, &length);
                push_elem(&top, s, &length);
                saved = 1;
            }
            else
                printf("Стек пуст.\n");
        }
        
        if (!strcmp(op, "load"))
            if (saved)
            {
                push_elem(&top, s, &length);   
                list_elem(top);
            }
            else
                printf("Нет сохраненного значения.\n");
        
        
        if (!strcmp(op, "top")) 
            if (length == 0)
                printf("Стек пуст\n");
            else
                printf("%lf\n", top->value);
            
        if (!strcmp(op, "xchg"))
            if (length >= 2)
            {
            a = pop_elem(&top, &length);
            b = pop_elem(&top, &length);
            push_elem(&top, a, &length);
            push_elem(&top, b, &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "comp"))
            if (length >= 2)
            {
            a = pop_elem(&top, &length);
            b = pop_elem(&top, &length);
            push_elem(&top, b, &length);
            push_elem(&top, a, &length);
            if (a > b)
                push_elem(&top, 1, &length);
            if (a < b)
                push_elem(&top, -1, &length);
            if (a == b)
                push_elem(&top, 0, &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "test"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, a, &length);
            if (a > 0)
                push_elem(&top, 1, &length);
            if (a < 0)
                push_elem(&top, -1, &length);
            if (a == 0)
                push_elem(&top, 0, &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "add"))
            if (length >= 2)
            {
            a = pop_elem(&top, &length);
            b = pop_elem(&top, &length);
            push_elem(&top, a + b, &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "sub"))
            if (length >= 2)
            {
            a = pop_elem(&top, &length);
            b = pop_elem(&top, &length);
            push_elem(&top, a - b, &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "mul"))
            if (length >= 2)
            {
            a = pop_elem(&top, &length);
            b = pop_elem(&top, &length);
            push_elem(&top, a * b, &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "div"))
            if (length >= 2)
            {
            a = pop_elem(&top, &length);
            b = pop_elem(&top, &length);
            push_elem(&top, a / b, &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "inv"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, -a, &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "abs"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, abs(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "rnd"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, floor(a + 0.5), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "sqroot"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, sqrt(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "rnd"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, floor(a + 0.5), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
        
        if (!strcmp(op, "sin"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, sin(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "cos"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, cos(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "tan"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, tan(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "asin"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, asin(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "acos"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, acos(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "atan"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, atan(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "sinh"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, sinh(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "cosh"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, cosh(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "asinh"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, asinh(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "acosh"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, acosh(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "log2"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, log2(a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "pow2"))
            if (length >= 1)
            {
            a = pop_elem(&top, &length);
            push_elem(&top, pow(2,a), &length);
            list_elem(top);
            }
            else
                printf("Недостаточно чисел в стеке\n");
            
        if (!strcmp(op, "list"))
            list_elem(top);
        
        printf("-> ");
        scanf("%s", op);
    }
    while ((strcmp(op, "q")) && (strcmp(op, "e")));
    return 0;
}
