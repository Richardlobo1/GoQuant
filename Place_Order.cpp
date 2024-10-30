/*#include <iostream>
#include "json/include/nlohmann/json.hpp"
#include <nlohmann/json.hpp>
#include <curl/curl.h>

#include <string>

using json = nlohmann::json;

// Replace these with your actual Deribit API keys
const std::string client_id = "dA1xPrZH";
const std::string client_secret = "O1om1mkN7HTe5zf-jSr6GGxvwbgg0LBcc3Msp-O4iyA";

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to get access token
std::string getAccessToken() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://test.deribit.com/api/v2/public/auth?client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        auto json_response = json::parse(readBuffer);
        return json_response["result"]["access_token"];
    }
    return "";
}

// Function to place an order Place_Order.c++
void placeOrder(const std::string& access_token, const std::string& instrument_name, double amount, double price) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://test.deribit.com/api/v2/private/buy?instrument_name=" + instrument_name + "&amount=" + std::to_string(amount) + "&price=" + std::to_string(price);
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            auto json_response = json::parse(readBuffer);
            std::cout << "Order placed: " << json_response.dump(4) << std::endl;
        } else {
            std::cerr << "Error placing order: " << curl_easy_strerror(res) << std::endl;
        }
    }
}
void cancelOrder(const std::string& access_token, const std::string& order_id) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/private/cancel?order_id=" + order_id;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            auto json_response = json::parse(readBuffer);
            std::cout << "Order canceled: " << json_response.dump(4) << std::endl;
        } else {
            std::cerr << "Error canceling order: " << curl_easy_strerror(res) << std::endl;
        }
    }
}
int main() {
    std::string access_token = getAccessToken();
    if (!access_token.empty()) {
        std::string instrument_name = "BTC-PERPETUAL";  // Specify the instrument, e.g., BTC-PERPETUAL
        double amount = 1.0;  // Specify order amount
        double price = 40000.0;  // Specify order price
        
        // Place an order
        placeOrder(access_token, instrument_name, amount, price);
        
        // Assuming you have the order ID to cancel (you can obtain this from the order response)
        std::string order_id = "YOUR_ORDER_ID";  // Replace with the actual order ID
        cancelOrder(access_token, order_id);
    } else {
        std::cerr << "Failed to retrieve access token" << std::endl;
    }
    return 0;
}

#include <iostream>
#include <string>
#include "cjson/cJSON.h"
//#include <cjson/cJSON.h>  // Include cJSON
#include <curl/curl.h>     // For HTTP requests

// Replace these with your Deribit API keys
const std::string client_id = "dA1xPrZH";
const std::string client_secret = "O1om1mkN7HTe5zf-jSr6GGxvwbgg0LBcc3Msp-O4iyA";

// Callback to write response data into a string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to parse JSON using cJSON
std::string parseAccessToken(const std::string& json_str) {
    cJSON* root = cJSON_Parse(json_str.c_str());
    if (root == nullptr) {
        std::cerr << "Error parsing JSON response" << std::endl;
        return "";
    }

    cJSON* result = cJSON_GetObjectItem(root, "result");
    cJSON* access_token = cJSON_GetObjectItem(result, "access_token");

    std::string token = access_token ? access_token->valuestring : "";
    cJSON_Delete(root);  // Clean up
    return token;
}

// Function to get the access token
std::string getAccessToken() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/public/auth?client_id=" + client_id +
                          "&client_secret=" + client_secret + "&grant_type=client_credentials";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Failed to retrieve access token: " << curl_easy_strerror(res) << std::endl;
            return "";
        }

        return parseAccessToken(readBuffer);
    }
    return "";
}

// Function to place an order
void placeOrder(const std::string& access_token, const std::string& instrument_name, 
                double amount, double price) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/private/buy?instrument_name=" + instrument_name +
                          "&amount=" + std::to_string(amount) + "&price=" + std::to_string(price);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            std::cout << "Order placed: " << readBuffer << std::endl;
        } else {
            std::cerr << "Error placing order: " << curl_easy_strerror(res) << std::endl;
        }
    }
}

// Function to cancel an order
void cancelOrder(const std::string& access_token, const std::string& order_id) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/private/cancel?order_id=" + order_id;

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            std::cout << "Order canceled: " << readBuffer << std::endl;
        } else {
            std::cerr << "Error canceling order: " << curl_easy_strerror(res) << std::endl;
        }
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
*/
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

// Replace these with your actual Deribit API keys
const std::string client_id = "client_id ";
const std::string client_secret = "client_secret";
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to get access token
std::string getAccessToken() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/public/auth?client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        auto json_response = json::parse(readBuffer);
        return json_response["result"]["access_token"];
    }
    return "";
}

// Function to place an order
// Modified placeOrder function to return the order_id
std::string placeOrder(const std::string& access_token, const std::string& instrument_name, double amount, double price) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/private/buy?instrument_name=" + instrument_name + "&amount=" + std::to_string(amount) + "&price=" + std::to_string(price);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            auto json_response = json::parse(readBuffer);
            std::cout << "Order placed: " << json_response.dump(4) << std::endl;

            // Extract and return the order_id from the response
            if (json_response.contains("result") && json_response["result"].contains("order_id")) {
                return json_response["result"]["order_id"];
            }
        } else {
            std::cerr << "Error placing order: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return "";  // Return an empty string if there was an error
}


//cancel order

void cancelOrder(const std::string& access_token, const std::string& order_id) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/private/cancel?order_id=" + order_id;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            auto json_response = json::parse(readBuffer);
            std::cout << "Order canceled: " << json_response.dump(4) << std::endl;
        } else {
            std::cerr << "Error canceling order: " << curl_easy_strerror(res) << std::endl;
        }
    }
}
// Function to modify an order
void modifyOrder(const std::string& access_token, const std::string& order_id, double amount, double price) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/private/edit?order_id=" + order_id + "&amount=" + std::to_string(amount) + "&price=" + std::to_string(price);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            auto json_response = json::parse(readBuffer);
            std::cout << "Order modified: " << json_response.dump(4) << std::endl;
        } else {
            std::cerr << "Error modifying order: " << curl_easy_strerror(res) << std::endl;
        }
    }
}

// Function to get the orderbook for an instrument
void getOrderBook(const std::string& access_token, const std::string& instrument_name) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/public/get_order_book?instrument_name=" + instrument_name;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            auto json_response = json::parse(readBuffer);
            std::cout << "Order book for " << instrument_name << ": " << json_response.dump(4) << std::endl;
        } else {
            std::cerr << "Error getting orderbook: " << curl_easy_strerror(res) << std::endl;
        }
    }
}

// Function to view current positions
void viewCurrentPositions(const std::string& access_token) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/private/get_positions";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            auto json_response = json::parse(readBuffer);
            std::cout << "Current positions: " << json_response.dump(4) << std::endl;
        } else {
            std::cerr << "Error viewing positions: " << curl_easy_strerror(res) << std::endl;
        }
    }
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
            getOrderBook(access_token, instrument_name);

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
