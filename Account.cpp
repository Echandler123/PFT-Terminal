//
// Created by Eli Chandler on 8/29/25.
//
#include "Account.h"
#include "Transaction.h"
#include <iostream>
#include <ctime>
#include <fstream>

Account::Account() = default;
void Account::run() {
    loadFromFile();
    int choice;
    int choice2;

    std::cout << "Welcome to the Personal Finance Tracker!" << std::endl;
    std::cout << "Press 1 to make a new transaction" "\n" "Press 2 to view existing transactions " "\n" "Press 3 to "
    "import a file containing transactions" "\n" "Press 4 to remove all saved transactions" "\n" "Press 5 to remove a "
    "specific transaction" << std::endl;
    std::cin >> choice;
    while(choice > 0){
        if(choice == 1){
            transactions.push_back(newTransaction());
        }
        else if(choice == 2){
            std::cout << "Press 3 view all transactions." "\n" "Press 4 view all transactions of at least the given "
            "amount" "\n" "Press 5 view all transactions of the given type" "\n" "Press 6 view transactions on the given "
            "date" "\n"<< std::endl;
            std::cin >> choice2;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pickChoice(choice2);
        }
        else if(choice == 3) {
            std::string name;
            std::cout << "What is the name of your file?" << std::endl;
            std::cin >> name;
            importFromFile(name);
        }
        else if(choice == 4) {
            clearSave();
            std::cout << "All saved transactions are now removed" << std::endl;
        }
        else if(choice == 5) {
            int id = -1;
            removeTransaction(id);
        }
        std::cout << "Press 1 to make a new transaction" "\n" "Press 2 to view existing transactions " "\n" "Press 3 to "
        "import a file containing transactions" "\n" "Press 4 to remove all saved transactions" "\n" "Press 5 to remove a "
        "specific transaction" << std::endl;
        std::cout << "Press 0 to exit." << std::endl;
        std::cin >> choice;
    }
    saveToFile();
}
void Account::addTransaction(Transaction tx) {
    transactions.push_back(tx);
}
std::vector<Transaction> Account::getTransactions() const {
    return transactions;
}
void Account::clearSave() {
    std::ofstream file("transactions.txt", std::ios::trunc);
    for (int i =0; i < transactions.size(); i++) {
            transactions.erase(transactions.begin() + i);
    }
    file.close();
}
void Account::importFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if(!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }
    std::string line;
    while(getline(file, line)) {
        if (!line.empty()) {
            Transaction t = Transaction::deserialize(line);
            transactions.push_back(t);
        }
    }
    file.close();
}
void Account::saveToFile() {
    std::ofstream file("transactions.txt");
    if (file.is_open()) {
        for (int i = 0; i < transactions.size(); i++) {
            std::string line = transactions[i].serialize();
            file << line << "\n";
        }
    }
    file.close();
}
void Account::loadFromFile() {
    std::ifstream file("transactions.txt");
    if (!file) {
        std::cerr << "No transactions on record" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Transaction t = Transaction::deserialize(line);
            transactions.push_back(t);
        }
    }
    file.close();
}
Transaction Account::newTransaction() {
    return Transaction(getValidAmount(), getValidDate(), getValidType(), getDesc());;
}
std::string Account::getValidDate() const {
    std::string date;
    while (true) {
        std::cout << "Enter the transaction date (e.g. 04-06-2025): ";
        std::getline(std::cin, date);
        if (isValidDate(date) == true) {
            return date;
        }
        std::cout << "Please enter a valid date" << std::endl;
    }
}
bool Account::isValidDate(const std::string& date) const {

    if (date.size() == 10 && date[2] == '-' && date[5] == '-') {
        std::tm tm = {};
        int year, month, day;
        try {
            month = std::stoi(date.substr(0, 2));
            day = std::stoi(date.substr(3, 2));
            year = std::stoi(date.substr(6,4));
        } catch (...) {
            return false;
        }
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        std::time_t time = std::mktime(&tm);
        return time != -1 && tm.tm_year == year - 1900 && tm.tm_mon == month - 1 && tm.tm_mday == day;

    }
    return false;
}
std::string Account::getDesc() const{
    std::string desc;
    std::cout << "Enter transaction description: ";
    std::getline(std::cin, desc);
    return desc;
}
double Account::getValidAmount() const {
    double amt;
    while (true) {
        std::cout << "Enter transaction amount: ";
        std::cin >> amt;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(amt >= 0) {
            return amt;
        }
        std::cout << "Please enter a valid amount." << std::endl;
    }
}
std::string Account::getValidType() const{
    std::string type;
    while (true) {
        std::cout << "Enter transaction type (e.g. Income/Expense): ";
        std::getline(std::cin, type);
        if(type == "Income" || type == "Expense") {
            return type;
        }
        std::cout << "Please enter a valid transaction type." << std::endl;
    }
}
void Account::removeTransaction(int id) {
    for (int i =0; i < transactions.size(); i++) {
        if (transactions[i].getId()== id) {
            transactions.erase(transactions.begin() + i);
            return;
        }
    }
}

void Account::printTransactionsByDate(const std::string& date) const{
    if (transactions.empty()) {
        std::cout << "No transactions found" << std::endl;
        return;
    }
    for(int i = 0; i < transactions.size(); i ++) {
        if(transactions[i].getDate() == date) {
            transactions[i].printTransaction();
        }
    }
}
void Account::printTransactionsByType(const std::string& type) const {
    if (transactions.empty()) {
        std::cout << "No transactions found" << std::endl;
        return;
    }
    for(int i = 0; i < transactions.size(); i ++) {
        if(transactions[i].getType() == type) {
            transactions[i].printTransaction();
        }
    }
}
void Account::printTransactionsByAmount(double amount) const {
    if (transactions.empty()) {
        std::cout << "No transactions found" << std::endl;
        return;
    }
    for(int i = 0; i < transactions.size(); i ++) {
        if(transactions[i].getAmount() >= amount) {
            transactions[i].printTransaction();
        }
    }
}
void Account::viewAllTransactions() const {
    if (transactions.empty()) {
        std::cout << "No transactions found" << std::endl;
        return;
    }
    if(!transactions.empty()) {
        for(int i = 0; i < transactions.size(); i ++){
            transactions[i].printTransaction();
        }
    }
}
void Account::pickChoice(int choice) {
    switch (choice) {
        case 3: {
            viewAllTransactions();
            break;
        }
        case 4: {
            double amountGiven;
            std::cout << "Please Enter a given amount: ";
            std::cin >> amountGiven;
            printTransactionsByAmount(amountGiven);
            break;
        }
        case 5: {
            std::string typeGiven;
            std::cout << "Please enter a given type (e.g. Income/Expense):";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, typeGiven);
            printTransactionsByType(typeGiven);
            break;
        }
        case 6: {
            std::string dateGiven;
            std::cout << "Please enter a given date (e.g. 04-06-2025):";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, dateGiven);
            printTransactionsByDate( dateGiven);
            break;
        }
        default: {}
    }
}
void Account::printAllIncome() const{
    if (transactions.empty()) {
        std::cout << "No income found" << std::endl;
        return;
    }
    double totalAmount = 0;
    for (int i = 0; i < transactions.size(); i ++) {
        if (transactions[i].getType() == "Income") {
            totalAmount += transactions[i].getAmount();
        }
    }
    std::cout << "Total Income: " << totalAmount << std::endl;
}
void Account::printAllExpense() const{
    if (transactions.empty()) {
        std::cout << "No expenses found" << std::endl;
        return;
    }
    double totalAmount = 0;
    for (int i = 0; i < transactions.size(); i ++) {
        if (transactions[i].getType() == "Expense") {
            totalAmount += transactions[i].getAmount();
        }
    }
    std::cout << "Total Expenses: -" << totalAmount << std::endl;
}

