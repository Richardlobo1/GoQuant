#include <iostream>
#include "cpp-httplib/httplib.h"        // Include cpp-httplib
#include <rapidjson/document.h>  // Include RapidJSON
#include <string>

using namespace rapidjson;

// Replace with your actual Deribit API keys

const std::string client_id = "client_id ";
const std::string client_secret = "client_secret";

// Function to get access token
std::string getAccessToken() {
    httplib::Client cli("https://test.deribit.com");
    std::string url = "/api/v2/public/auth?client_id=" + client_id + 
                      "&client_secret=" + client_secret + 
                      "&grant_type=client_credentials";

    auto res = cli.Get(url.c_str());
    if (res && res->status == 200) {
        Document doc;
        doc.Parse(res->body.c_str());
        return doc["result"]["access_token"].GetString();
    }
    std::cerr << "Failed to get access token" << std::endl;
    return "";
}

// Function to place an order
void placeOrder(const std::string& access_token, const std::string& instrument_name, 
                double amount, double price) {
    httplib::Client cli("https://test.deribit.com");
    std::string url = "/api/v2/private/buy?instrument_name=" + instrument_name + 
                      "&amount=" + std::to_string(amount) + 
                      "&price=" + std::to_string(price);

    httplib::Headers headers = { { "Authorization", "Bearer " + access_token } };
    auto res = cli.Get(url.c_str(), headers);

    if (res && res->status == 200) {
        Document doc;
        doc.Parse(res->body.c_str());
        std::cout << "Order placed: " << res->body << std::endl;
    } else {
        std::cerr << "Error placing order: " << res.error() << std::endl;
    }
}

// Function to cancel an order
void cancelOrder(const std::string& access_token, const std::string& order_id) {
    httplib::Client cli("https://test.deribit.com");
    std::string url = "/api/v2/private/cancel?order_id=" + order_id;

    httplib::Headers headers = { { "Authorization", "Bearer " + access_token } };
    auto res = cli.Get(url.c_str(), headers);

    if (res && res->status == 200) {
        Document doc;
        doc.Parse(res->body.c_str());
        std::cout << "Order canceled: " << res->body << std::endl;
    } else {
        std::cerr << "Error canceling order: " << res.error() << std::endl;
    }
}

int main() {
    std::string access_token = getAccessToken();
    if (!access_token.empty()) {
        std::string instrument_name = "BTC-PERPETUAL";
        double amount = 1.0;
        double price = 40000.0;

        // Place an order
        placeOrder(access_token, instrument_name, amount, price);

        // Replace with a valid order ID
        std::string order_id = "YOUR_ORDER_ID";
        cancelOrder(access_token, order_id);
    } else {
        std::cerr << "Failed to retrieve access token" << std::endl;
    }
    return 0;
}
