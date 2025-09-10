//
// Created by Eli Chandler on 8/28/25.
//

#ifndef PFT_TRANSACTION_H
#define PFT_TRANSACTION_H
#include <string>
#include <iostream>
#include <vector>
class Transaction {
protected:
    static int nextId;
    int id;
    double amount;
    std::string date;
    std::string type;
    std::string description;
    static const std::string DELIMITER;
public:
    Transaction(double amt, const std::string& d, const std::string& t, const std::string& desc);
    int getId() const;
    double getAmount() const;
    std::string getDate() const;
    std::string getType() const;
    std::string getDescription() const;
    void setDescription(const std::string& desc);
    void setDate(const std::string& d);
    void setType(const std::string& t);
    void printTransaction() const;
    std::string serialize() const;
    static Transaction deserialize(std::string line);
};
#endif //PFT_TRANSACTION_H