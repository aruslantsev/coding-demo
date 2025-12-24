#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct tree_node {
    int val;
    struct tree_node *left;
    struct tree_node *right;
};

typedef struct tree_node treeNode;

treeNode *addNode(treeNode *, int);
void showTreeLine(treeNode *);
void showTree(treeNode *, int);

int main(void) {
    srand(time(NULL));
    treeNode *root = NULL;
    int val;
    root = addNode(root, 10);
    for (int i = 0; i < 13; i++) {
        val = rand() % 20;
        printf("%d ", val);
        root = addNode(root, val);
    }
    puts("");
    showTreeLine(root);
    puts("");
    showTree(root, 0);
    puts("");
    return 0;
}

treeNode *addNode(treeNode *root, int val) {
    if (root == NULL) {
        root = (treeNode *) malloc(sizeof(treeNode));
        if (root == NULL) {
            puts("Memory allocation failed");
            exit(1);
        }
        root->val = val;
        root->left = NULL;
        root->right = NULL;
    } else {
        if (val < root->val) {
            root->left = addNode(root->left, val);
        } else if (val > root->val) {
            root->right = addNode(root->right, val);
        } else {
            printf("DUP%d ", val);
        }
    }
    return root;
}

void showTreeLine(treeNode *root) {
    if (root != NULL) {
        showTreeLine(root->right);
        printf("%d ", root->val);
        showTreeLine(root->left);
    }
}

void showTree(treeNode *root, int depth) {
    if (root != NULL) {
        showTree(root->right, depth + 1);
        for (int i = 0; i < depth; i++) printf("%8s", "");
        printf("%5d\n", root->val);
        showTree(root->left, depth + 1);
    }
}
