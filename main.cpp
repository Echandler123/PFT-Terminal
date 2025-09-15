#define CROW_USE_BOOST
#include "Account.h"
#include "include/crow.h"
int main() {
    crow::SimpleApp app;
    Account acc;
    acc.run();
    return 0;
}
