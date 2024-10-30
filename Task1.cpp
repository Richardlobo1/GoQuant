#include <iostream>
#include <string>
#include "cjson/cJSON.h"
#include <httpClient.h>  // Include libHTTPClient header

// Replace these with your actual Deribit API keys


const std::string client_id = "client_id ";
const std::string client_secret = "client_secret";

// Helper function for HTTP GET request
std::string httpGetRequest(const std::string& url) {
    HC_INIT();
    HCCallHandle call;
    HRESULT hr = HCHttpCallCreate(&call);
    HCHttpCallRequestSetUrl(call, "GET", url.c_str());

    std::string responseBody;
    hr = HCHttpCallPerformAsync(call, nullptr, [](HRESULT, HCCallHandle handle, void*) {
        uint32_t statusCode;
        const char* body;
        HCHttpCallResponseGetStatusCode(handle, &statusCode);
        HCHttpCallResponseGetResponseString(handle, &body);
        responseBody = body;
        HCHttpCallCloseHandle(handle);
    });
    HC_CLEANUP();
    return responseBody;
}

// Helper function for parsing JSON with cJSON
std::string getJsonValue(const std::string& jsonStr, const char* key) {
    cJSON* json = cJSON_Parse(jsonStr.c_str());
    if (!json) return "";

    cJSON* jsonValue = cJSON_GetObjectItem(json, key);
    std::string result = jsonValue ? jsonValue->valuestring : "";
    cJSON_Delete(json);
    return result;
}

// Function to get access token
std::string getAccessToken() {
    std::string url = "https://test.deribit.com/api/v2/public/auth?client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials";
    std::string response = httpGetRequest(url);

    // Parse JSON response
    std::string access_token = getJsonValue(response, "result.access_token");
    return access_token;
}

// Function to place an order
std::string placeOrder(const std::string& access_token, const std::string& instrument_name, double amount, double price) {
    std::string url = "https://test.deribit.com/api/v2/private/buy?instrument_name=" + instrument_name + "&amount=" + std::to_string(amount) + "&price=" + std::to_string(price) + "&Authorization=Bearer " + access_token;
    std::string response = httpGetRequest(url);

    // Parse JSON response to get order_id
    std::string order_id = getJsonValue(response, "result.order_id");
    std::cout << "Order placed. Order ID: " << order_id << std::endl;
    return order_id;
}

// Function to modify an order
void modifyOrder(const std::string& access_token, const std::string& order_id, double amount, double price) {
    std::string url = "https://test.deribit.com/api/v2/private/edit?order_id=" + order_id + "&amount=" + std::to_string(amount) + "&price=" + std::to_string(price) + "&Authorization=Bearer " + access_token;
    std::string response = httpGetRequest(url);

    std::cout << "Order modified. Response: " << response << std::endl;
}

// Function to get the orderbook for an instrument
void getOrderBook(const std::string& instrument_name) {
    std::string url = "https://test.deribit.com/api/v2/public/get_order_book?instrument_name=" + instrument_name;
    std::string response = httpGetRequest(url);

    std::cout << "Order book for " << instrument_name << ": " << response << std::endl;
}

// Function to view current positions
void viewCurrentPositions(const std::string& access_token) {
    std::string url = "https://test.deribit.com/api/v2/private/get_positions?Authorization=Bearer " + access_token;
    std::string response = httpGetRequest(url);

    std::cout << "Current positions: " << response << std::endl;
}

int main() {
    std::string access_token = getAccessToken();
    if (!access_token.empty()) {
        std::string instrument_name = "BTC-PERPETUAL";
        double amount = 1.0;
        double price = 40000.0;

        // Place an order and get the order_id
        std::string order_id = placeOrder(access_token, instrument_name, amount, price);

        if (!order_id.empty()) {
            // Modify the order using the obtained order_id
            modifyOrder(access_token, order_id, amount, price + 500);

            // Get the orderbook for an instrument
            getOrderBook(instrument_name);

            // View current positions
            viewCurrentPositions(access_token);
        } else {
            std::cerr << "Failed to place order and obtain order_id" << std::endl;
        }
    } else {
        std::cerr << "Failed to retrieve access token" << std::endl;
    }

    return 0;
}
