#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


#define WORD_LENGTH 100


struct tree_node {
    char *word;
    int count;
    struct tree_node *left;
    struct tree_node *right;
};


struct tree_node *add_word(struct tree_node *node, char *word) {
    if (node == NULL) {
        node = (struct tree_node *) malloc(sizeof(struct tree_node));
        node->word = word;
        node->count = 1;
        node->left = node->right = NULL;
    } else {
        int cmp = strcmp(word, node->word);
        if (cmp < 0) {
            node->left = add_word(node->left, word);
        } else if (cmp > 0) {
            node->right = add_word(node->right, word);
        } else {
            node->count++;
        }
    }
    return node;
}


void print_tree(struct tree_node *node) {
    if (node != NULL) {
        print_tree(node->left);
        printf("%s: %d\n", node->word, node->count);
        print_tree(node->right);
    }
}


int count_nodes(struct tree_node *node) {
    if (node == NULL) {
        return 0;
    } else {
        return 1 + count_nodes(node->left) + count_nodes(node->right);
    }
}


void collect_words(struct tree_node **words, struct tree_node *node) {
    static int counter = 0;
    if (node != NULL) {
        collect_words(words, node->left);
        words[counter++] = node;
        collect_words(words, node->right);
    }
}


int compare_counts(const void *left, const void *right) {
    return (*(struct tree_node **) left)->count - (*(struct tree_node **) right)->count;
}


int main(void) {
    int c, curr_length = 0, num_words;
    char buf[WORD_LENGTH], *word;
    struct tree_node *root = NULL, **words;
    do {
        c = getchar();
        if ((isalnum(c) || c == '_' || c == '-') && curr_length < WORD_LENGTH - 1) {
            buf[curr_length++] = tolower(c);
        } else {
            if (curr_length > 0) {
                buf[curr_length++] = '\0';
                word = (char *) malloc(sizeof(char) * (strlen(buf) + 1));
                strcpy(word, buf);
                root = add_word(root, word);
            }
            curr_length = 0;
        }
    } while (c != EOF);

    num_words = count_nodes(root);
    printf("Number of unique words: %d\n", num_words);

    printf("\n\nWords in alphabetic order\n");
    print_tree(root);

    printf("\n\nWords by frequency\n");
    words = (struct tree_node **) malloc(sizeof(struct tree_node *) * num_words);
    collect_words(words, root);
    qsort(words, num_words, sizeof(struct tree_node *), compare_counts);
    for (int i = 0; i < num_words; i++) {
        printf("%s: %d\n", (words[i])->word, words[i]->count);
    }
    return 0;
}
