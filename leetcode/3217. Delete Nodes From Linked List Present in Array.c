/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
 int compare(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

struct ListNode* modifiedList(int* nums, int numsSize, struct ListNode* head) {
    struct ListNode *fakehead = malloc(sizeof(struct ListNode));
    struct ListNode *nodeptr, *tmpptr;

    fakehead->next = head;
    nodeptr = fakehead;

    qsort(nums, numsSize, sizeof(int), compare);

    while (nodeptr != NULL) {
        if (
            nodeptr->next 
            && (int *) bsearch(&((nodeptr->next)->val), nums, numsSize, sizeof(int), compare)
        ) {
            tmpptr = nodeptr->next;
            nodeptr->next = nodeptr->next->next;
            free(tmpptr);
        } else {
            nodeptr = nodeptr->next;
        }
    }
    nodeptr = fakehead->next;
    free(fakehead);
    return nodeptr;
}
 /**********************************************************************************************************************/
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
 int compare(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

struct ListNode* modifiedList(int* nums, int numsSize, struct ListNode* head) {
    struct ListNode *fakehead = malloc(sizeof(struct ListNode));
    struct ListNode *nodeptr, *tmpptr;

    fakehead->next = head;
    nodeptr = fakehead;

    qsort(nums, numsSize, sizeof(int), compare);

    while (nodeptr != NULL) {
        while (
            nodeptr->next 
            && (int *) bsearch(&((nodeptr->next)->val), nums, numsSize, sizeof(int), compare)
        ) {
            tmpptr = nodeptr->next;
            nodeptr->next = nodeptr->next->next;
            free(tmpptr);
        }
        nodeptr = nodeptr->next;
    }
    nodeptr = fakehead->next;
    free(fakehead);
    return nodeptr;
}

