#define CROW_USE_BOOST
#include "Account.h"
#include "include/crow.h"
#include <fstream>
#include <sstream>

std::string loadFile(const std::string& htmlFile) {
    std::ifstream file(htmlFile);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
int main() {

    crow::SimpleApp app;
    Account acc;
    acc.loadFromFile();
    //acc.run();

    CROW_ROUTE(app, "/")([](){
        return loadFile("../frontEnd/index.html");
    });
    CROW_ROUTE(app, "/transactions")([&](crow::response& res){
        crow::json::wvalue jsonTransactions;
        const auto& transactions = acc.getTransactions();
        for (size_t i = 0; i < transactions.size(); ++i) {
            const Transaction& t = transactions[i];
            jsonTransactions[i]["date"] = t.getDate();
            jsonTransactions[i]["description"] = t.getDescription();
            jsonTransactions[i]["amount"] = t.getAmount();
            jsonTransactions[i]["type"] = t.getType();
        }
        res.write(jsonTransactions.dump());
        std::cout << "Sending " << transactions.size() << " transactions to frontend.\n";
        res.end();
    });
    CROW_ROUTE(app, "/newtransaction").methods(crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){
    // Parse JSON sent by frontend
        auto body = crow::json::load(req.body);
        if (!body) {
            res.code = 400; // Bad request
            res.write("Invalid JSON");
            res.end();
            return;
        }

        // Extract transaction fields
        double amount = body["amount"].d();
        std::string date = body["date"].s();
        std::string type = body["type"].s();
        std::string description = body["description"].s();

        // Create new transaction and add it to Account
        Transaction t(amount, date, type, description);
        acc.addTransaction(t);
        acc.saveToFile();
        std::cout << "Transaction added. Total now: " << acc.getTransactions().size() << std::endl;
        // Respond with success
        res.code = 200;
        res.write("Transaction added");
        res.end();
    });
    // Clear all transactions
    CROW_ROUTE(app, "/cleartransactions").methods(crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){
        acc.clearSave();  // Your existing method to clear vector and file
        res.code = 200;
        res.write("All transactions cleared");
        res.end();
    });


    CROW_ROUTE(app, "/<path>")([](std::string path){
        return loadFile("../frontEnd/" + path);
    });



    app.port(18080).multithreaded().run();

    return 0;
}

