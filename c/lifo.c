#include <stdio.h>
#include <stdlib.h>

typedef struct stack_element
{
    stack_element* next;
    double value;
};

void print_help()
{
    printf("Демонстрация реализации стека\nВведите h для помощи, a для добавления элемента в стек, d для удаления элемента из стека, g для вывода верхнего элемента стека, l для вывода стека на экран, q или e для выхода из программы\n");
    return;    
}

void add_elem(stack_element **top)
{
    double val;
    stack_element *new_elem;
    new_elem = (stack_element *) malloc(sizeof(stack_element));
    printf("Введите число: ");
    scanf("%lf", &val);
    new_elem->value = val;
    new_elem->next = *top;
    *top = new_elem;
    return;
}

void del_elem(stack_element **top)
{
    stack_element *new_top;
    new_top = (*top)->next;
    free(*top);
    *top = new_top;
    return;
}

void list_elem(stack_element *top)
{
    stack_element *elem;
    elem = top;
    while (elem->next != NULL)
    {
        printf("%lf ", elem->value);
        elem = elem->next;
    }
    printf("\n");
    return;
}

int main()
{
    stack_element *top;
    int length = 0;
    char op;
    double val;
    top = (stack_element *) malloc(sizeof(stack_element));
    top->value = 0;
    top->next = NULL;
    do
    {
        switch (op)
        {
            case 'h':
                print_help();
                break;
            case 'a':
                add_elem(&top);
                length++;
                break;
            case 'd':
                if (length > 0)
                {
                    del_elem(&top);
                    length--;
                }
                else
                    printf("Невозможно удалить элемент.\n");
                break;
            case 'g':
                if (length == 0)
                    printf("Стек пуст\n");
                else
                    printf("%lf\n", top->value);
                break;
            case 'l':
                list_elem(top);
                break;
            default:
                print_help();
        }
        printf("Введите код операции: ");
        scanf(" %c", &op);
    }
    while ((op != 'e') && (op != 'q'));
    return 0;
}
