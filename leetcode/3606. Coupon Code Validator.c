/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

struct element {
    char *code;
    char *businessLine;
    bool isActive;
};

int cmp_elem(const void *a, const void *b) {
    struct element elem1 = *((struct element *) a);
    struct element elem2 = *((struct element *) b);
    int ans = 0;
    ans = strcmp(elem1.businessLine, elem2.businessLine);
    if (ans != 0) return ans;
    return strcmp(elem1.code, elem2.code);
}

bool checkstr(char *s) {
    if (strlen(s) == 0) return false;
    while (*s != '\0') {
        if (isalnum(*s) || *s == '_')
            s++;
        else
            return false;
    }
    return true;
}

bool checkbl(char *s) {
    const char *bl[4] = {"electronics", "grocery", "pharmacy", "restaurant"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(s, bl[i]) == 0)
            return true;
    }
    return false;
}

char** validateCoupons(char** code, int codeSize, char** businessLine, int businessLineSize, bool* isActive, int isActiveSize, int* returnSize) {
    int ans_size = 0;
    char **ans = (char **) malloc(sizeof(char *) * codeSize);
    struct element *arr = (struct element *) malloc(sizeof(struct element) * codeSize);
    struct element curr_elem;
    for (int i = 0; i < codeSize; i++) {
        if (isActive[i] && checkstr(code[i]) && checkbl(businessLine[i]))
            arr[ans_size++] = (struct element) {.code=code[i], .businessLine=businessLine[i], .isActive=isActive[i]};
    }
    qsort(arr, ans_size, sizeof(struct element), cmp_elem);
    for (int i = 0; i < ans_size; i++) {
        ans[i] = arr[i].code;
    }
    *returnSize = ans_size;
    return ans;
}
