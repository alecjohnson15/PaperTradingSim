#include <iostream>
#include <string>
#include <curl/curl.h>
#include "stocks.h"

const std::string api = "C1PD8ADVXJ0EMYPC";

size_t WriteCallback(void* contents, size_t size, size_t i, std::string* output) {
    size_t totalSize = size * i;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string Extract(const std::string& response) {
    size_t start = response.find("\"05. price\"");
    if (start != std::string::npos) {
        start = response.find(":", start);
        if (start != std::string::npos) {
            start = response.find_first_of("0123456789", start);
            if (start != std::string::npos) {
                size_t end = response.find_first_not_of("0123456789.", start);
                return (end != std::string::npos) ? response.substr(start, end - start) : "";
            }
        }
    }
    std::cerr << "Cannot find the price value in the response." << std::endl;
    return "";
}

double AVPrice(const std::string& symbol) {
    CURL* curl = curl_easy_init();

    if (curl) {
        std::string url = "https://www.alphavantage.co/query";
        std::string function = "GLOBAL_QUOTE";
        std::string requestUrl = url + "?function=" + function + "&symbol=" + symbol + "&api=" + api;

        std::string responseData;

        curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Error: Unable to perform HTTP request " << curl_easy_strerror(res) << std::endl;
        } else {
            std::string priceString = Extract(responseData);

            if (!priceString.empty()) {
                try {
                    return std::stod(priceString);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error " << e.what() << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error" << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Error" << std::endl;
                }
            } else {
                std::cerr << "Error: Alpha Vantage could not return Price" << std::endl;
            }
        }

        std::cout << "Alpha Vantage Response: " << responseData << std::endl;

        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Error" << std::endl;
    }

    return 0.0; 
}
