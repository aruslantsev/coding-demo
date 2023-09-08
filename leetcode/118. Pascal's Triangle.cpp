class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> result;
        vector<int> prev_row;
        prev_row.push_back(1);
        result.push_back(prev_row);
        for (int i = 1; i < numRows; i++) {
            vector<int> new_row;
            new_row.push_back(1);
            for (int j = 0; j < prev_row.size() - 1; j++)
                new_row.push_back(prev_row[j] + prev_row[j + 1]);
            new_row.push_back(1);
            result.push_back(new_row);
            prev_row = new_row;
        }
        return result;
    }
};
