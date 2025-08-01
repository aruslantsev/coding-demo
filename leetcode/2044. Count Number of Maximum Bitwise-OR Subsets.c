int count_subsets(int *nums, int nums_size, int index, int current_or, int target_or) {
    if (index == nums_size) return current_or == target_or? 1: 0;
    return (
        count_subsets(nums, nums_size, index + 1, current_or, target_or)
        + count_subsets(nums, nums_size, index + 1, current_or | nums[index], target_or)
    );
}


int countMaxOrSubsets(int* nums, int numsSize) {
    int target_or = 0;
    for (int i = 0; i < numsSize; target_or |= nums[i], i++);
    return count_subsets(nums, numsSize, 0, 0, target_or);
}
