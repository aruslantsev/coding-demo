struct tree_node {
    int val;
    struct tree_node *left;
    struct tree_node *right;
 };

struct tree_node *add_elem(struct tree_node *node, int val) {
    if (node == NULL) {
        node = (struct tree_node *) malloc(sizeof(struct tree_node));
        node->val = val;
        node->left = NULL;
        node->right = NULL;
    } else {
        if (val > node->val) {
            node->right = add_elem(node->right, val);
        } else if (val < node->val) {
            node->left = add_elem(node->left, val);
        }
    }
    return node;
}

int tree_sum(struct tree_node *node) {
    if (node == NULL) return 0;
    return node->val + tree_sum(node->left) + tree_sum(node->right);
}

int maxSum(int* nums, int numsSize) {
    struct tree_node *root = NULL;
    int max_val = -100;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] > 0) {
            root = add_elem(root, nums[i]);
        }
        max_val = nums[i] > max_val? nums[i]: max_val;
    }
    if (root == NULL) {
        return max_val;
    }
    return tree_sum(root);
}
