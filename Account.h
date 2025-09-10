//
// Created by Eli Chandler on 8/29/25.
//

#ifndef PFT_ACCOUNT_H
#define PFT_ACCOUNT_H
#include <iostream>
#include "Transaction.h"

class Account{
    protected:
        std::vector<Transaction> transactions;
    public:
        void run();
        Account();
        Transaction newTransaction();
        void clearSave();
        void removeTransaction(int id);
        void printTransactionsByDate(const std::string& date) const;
        void printTransactionsByType(const std::string& type) const;
        void printTransactionsByAmount( double amount) const;
        void viewAllTransactions() const;
        void pickChoice(int choice);
        void printAllIncome() const;
        void printAllExpense() const;
        std::string getValidType() const;
        double getValidAmount() const;
        std::string getDesc() const;
        std::string getValidDate() const;
        bool isValidDate(const std::string& date) const;
        void importFromFile(const std::string& filename);
        void saveToFile();
        void loadFromFile();
};
#endif //PFT_ACCOUNT_H