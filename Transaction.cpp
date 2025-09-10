//
// Created by Eli Chandler on 8/28/25.
//

#include "Transaction.h"
#include <iostream>
int Transaction::nextId = 1;
const std::string Transaction::DELIMITER  = "      |       ";
Transaction::Transaction(double amt, const std::string& d, const std::string& t, const std::string& desc)
: id(nextId++), amount(amt), date(d), type(t), description(desc) {
    std::cout << "Transaction added: " << description << std::endl;
}
std::string Transaction::serialize() const {
    return std::to_string(amount) + DELIMITER + date + DELIMITER + type + DELIMITER + description;
}
Transaction Transaction::deserialize(std::string line) {
    size_t pos1 = line.find(DELIMITER);
    size_t pos2 = line.find(DELIMITER, pos1 + DELIMITER.size());
    size_t pos3 = line.find(DELIMITER, pos2 + DELIMITER.size());
    double amount = std::stod(line.substr(0, pos1));
    std::string date = line.substr(pos1 + DELIMITER.size(), pos2 - (pos1 + DELIMITER.size()));
    std::string type = line.substr(pos2 + DELIMITER.size(), pos3 - (pos2 + DELIMITER.size()));
    std::string description = line.substr(pos3 + DELIMITER.size());
    return Transaction(amount, date, type, description);
}
int Transaction::getId() const {
    return id;
}

void Transaction::printTransaction() const{
    std::cout << "      |       " << type << "      |       " << description << "       |       " << amount << "        |       " << date << "        |       " << std::endl;
}
double Transaction::getAmount() const {
    return amount;
}
std::string Transaction::getDate() const {
    return date;
}
std::string Transaction::getType() const {
    return type;
}
std::string Transaction::getDescription() const {
    return description;
}
void Transaction::setDescription(const std::string &desc) {
    description = desc;
}
void Transaction::setDate(const std::string &d) {
    date = d;
}
void Transaction::setType(const std::string &t) {
    type = t;
}
