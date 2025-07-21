#include <stdio.h>
#include <stdlib.h>


typedef struct DequeElement {
    double value;
    struct DequeElement *next;
    struct DequeElement *prev;
} tDequeElement;


typedef struct {
    int length;
    tDequeElement *left;
    tDequeElement *right;
} tDeque;


tDeque init_deque() {
    tDeque queue = {.length = 0, .left = NULL, .right = NULL};
    return queue;
}


tDequeElement *create_element(double value) {
    tDequeElement *element = malloc(sizeof(tDequeElement));
    element->value = value;
    element->next = NULL;
    element->prev = NULL;
    return element;
}


void push_left(tDeque *queue, double value) {
    tDequeElement *element = create_element(value);
    if (queue->length == 0) {
        queue->left = element;
        queue->right = element;
    } else {
        queue->left->prev = element;
        element->next = queue->left;
        queue->left = queue->left->prev;
    }
    queue->length++;
}


double pop_left(tDeque *queue) {
    if (queue->length < 1) {
        printf("Queue is empty\n");
        return 0;
    }
    float value = queue->left->value;
    if (queue->length == 1) {
        free(queue->left);
        queue->left = NULL;
        queue->right = NULL;
    } else {
        queue->left = queue->left->next;
        free(queue->left->prev);
        queue->left->prev = NULL;
    }
    queue->length--;
    return value;
}


void push_right(tDeque *queue, double value) {
    tDequeElement *element = create_element(value);
    if (queue->length == 0) {
        queue->left = element;
        queue->right = element;
    } else {
        queue->right->next = element;
        element->prev = queue->right;
        queue->right = queue->right->next;
    }
    queue->length++;
}


double pop_right(tDeque *queue) {
    if (queue->length < 1) {
        printf("Queue is empty\n");
        return 0;
    }
    float value = queue->right->value;
    if (queue->length == 1) {
        free(queue->right);
        queue->left = NULL;
        queue->right = NULL;
    } else {
        queue->right = queue->right->prev;
        free(queue->right->next);
        queue->right->next = NULL;
    }
    queue->length--;
    return value;
}


void clean_deque(tDeque *queue) {
    while (queue->length > 0) pop_left(queue);
}


void print_deque(tDeque queue) {
    printf("Deque contents\n");
    printf("Deque length: %d\n", queue.length);
    tDequeElement *curr;
    curr = queue.left;
    for (int i = 0; i < queue.length; i++) {
        printf("Current: %f ", curr->value);
        if (curr->prev != NULL) {
            printf("Prev: %f ", curr->prev->value);
        }
        if (curr->next != NULL) {
            printf("Next: %f ", curr->next->value);
        }
        curr = curr->next;
        printf("\n");
    }
}


void print_help() {
    printf(
        "Double linked queue\n"
        "Enter h for help, "
        "l to push left, r to push right, "
        "e to pop right, k to pop left, "
        "p to print queue, q to exit\n");
    return;
}


int main()
{
    tDeque queue = init_deque();
    char op = 'h';
    double val;
    do
    {
        switch (op)
        {
            case 'h':
                print_help();
                break;
            case 'l':
                printf("Enter value: ");
                scanf("%lf", &val);
                push_left(&queue, val);
                break;
            case 'r':
                printf("Enter value: ");
                scanf("%lf", &val);
                push_right(&queue, val);
                break;
            case 'k':
                val = pop_left(&queue);
                printf("%f\n", val);
                break;
            case 'e':
                val = pop_right(&queue);
                printf("%f\n", val);
                break;
            case 'p':
                print_deque(queue);
                break;
            default:
                print_help();
        }
        printf("Enter operation: ");
        scanf(" %c", &op);
    }
    while (op != 'q');
    return 0;
}
