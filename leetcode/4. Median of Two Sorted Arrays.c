
double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size){
    int overall[2001];
    int idx1 = 0, idx2 = 0, idx = 0;
    while ((idx1 < nums1Size) || (idx2 < nums2Size)){
        if ((idx1 < nums1Size) && (idx2 < nums2Size)){
            if (nums1[idx1] <= nums2[idx2]){
                overall[idx] = nums1[idx1];
                idx1++;
            }
            else{
                overall[idx] = nums2[idx2];
                idx2++;
            }
        }
        else{
            if (idx1 < nums1Size){
                overall[idx] = nums1[idx1];
                idx1++;
            }
            else{
                overall[idx] = nums2[idx2];
                idx2++;
            }
        }
        idx++;
    }
    if ((nums1Size + nums2Size) % 2 == 1){
        return overall[(int) ((nums1Size + nums2Size) / 2)];
    }
    else{
        return (float) (overall[(int) ((nums1Size + nums2Size) / 2)] + overall[(int) ((nums1Size + nums2Size) / 2) - 1]) / 2.0;
    }
}
