#include <stdio.h>
#include <stdlib.h>

typedef struct queue_element
{
    queue_element* next;
    double value;
};

void print_help()
{
    printf("Демонстрация реализации очереди\nВведите h для помощи, a для добавления элемента в очередь, d для удаления элемента из очереди, g для вывода верхнего элемента очереди, l для вывода очереди на экран, q или e для выхода из программы\n");
    return;    
}

void add_elem(queue_element **last)
{
    double val;
    queue_element *new_elem;
    new_elem = (queue_element *) malloc(sizeof(queue_element));
    printf("Введите число: ");
    scanf("%lf", &val);
    new_elem->value = val;
    (*last)->next = new_elem;
    *last = new_elem;
    (*last)->next = NULL;
    return;
}

void del_elem(queue_element **top)
{
    queue_element *new_top;
    new_top = (*top)->next;
    free(*top);
    *top = new_top;
    return;
}

void list_elem(queue_element *top)
{
    queue_element *elem;
    elem = top;
    while (elem->next != NULL)
    {
        printf("%lf ", elem->value);
        elem = elem->next;
    }
    printf("%lf ", elem->value);
    printf("\n");
    return;
}

int main()
{
    queue_element *top, *last;
    int length = 0;
    char op;
    double val;
    top = (queue_element *) malloc(sizeof(queue_element));
    last = (queue_element *) malloc(sizeof(queue_element));
    top->value = 0;
    top->next = last;
    last->next = NULL;
    last->value = 0;
    do
    {
        switch (op)
        {
            case 'h':
                print_help();
                break;
            case 'a':
                add_elem(&last);
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
