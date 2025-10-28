
typedef struct {
    int num_accounts;
    long long *balance;
} Bank;

bool checkAccNum(int account, int max_account) {
    return (account > 0) && (account <= max_account);
}

Bank* bankCreate(long long* balance, int balanceSize) {
    Bank *bank = malloc(sizeof(Bank));
    bank->balance = balance;
    bank->num_accounts = balanceSize;
    return bank;
}

bool bankTransfer(Bank* obj, int account1, int account2, long long money) {
    if (checkAccNum(account1, obj->num_accounts) && checkAccNum(account2, obj->num_accounts)) {
        if (obj->balance[account1 - 1] >= money) {
            obj->balance[account1 - 1] -= money;
            obj->balance[account2 - 1] += money;
            return true;
        }
        return false;
    }
    return false;
}

bool bankDeposit(Bank* obj, int account, long long money) {
    if (checkAccNum(account, obj->num_accounts)) {
        obj->balance[account - 1] += money;
        return true;
    }
    return false;
}

bool bankWithdraw(Bank* obj, int account, long long money) {
    if (checkAccNum(account, obj->num_accounts)) {
        if (obj->balance[account - 1] >= money) {
            obj->balance[account - 1] -= money;
            return true;
        }
        return false;
    }
    return false;
}

void bankFree(Bank* obj) {
    free(obj);
}

/**
 * Your Bank struct will be instantiated and called as such:
 * Bank* obj = bankCreate(balance, balanceSize);
 * bool param_1 = bankTransfer(obj, account1, account2, money);

 * bool param_2 = bankDeposit(obj, account, money);

 * bool param_3 = bankWithdraw(obj, account, money);

 * bankFree(obj);
*/
