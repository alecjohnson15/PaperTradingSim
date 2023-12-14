#include <iostream>
#include <fstream>
#include <vector>
#include "stocks.h" // Code to get Market Pricing

using namespace std;

class Asset {
public:
    string ticker;
    double price;
    int shares;


    Asset() : price(0.0), shares(0) {}
    Asset(const string& ticker, double price, int shares) : ticker(ticker), price(price), shares(shares) {}
};

class Assets {
public:
    vector<Asset> assets;

    
    void View() const {
    if (assets.empty()) {
        cout << "You have no assets.\n";
    } else {
        double totalPurchaseValue = 0.0;
        double totalCurrentValue = 0.0;

        cout << "Your assets:\n";
        for (const auto& asset : assets) {
            double currentPrice = AVPrice(asset.ticker);
            double purchacePrice = asset.price;
            double currentValue = currentPrice * asset.shares;
            double priceChange = currentValue - (purchacePrice * asset.shares);

            cout << "- " << asset.ticker << " (Shares: " << asset.shares
                << ", Purchase Price: $" << purchacePrice
                << ", Current Price: $" << currentPrice
                << ", Total Value: $" << currentValue
                << ",  Ticker Change: $" << (currentPrice - purchacePrice)<< ")\n";

            totalPurchaseValue += purchacePrice * asset.shares;
            totalCurrentValue += currentValue;
        }

        double totalBalance = totalCurrentValue - totalPurchaseValue;

        cout << "\nYour Total Spending: $" << totalPurchaseValue << "\n";
        cout << "Total Current Value: $" << totalCurrentValue << "\n";
        cout << "Profit/Loss: $" << totalBalance << "\n";
    }
}

void Purchase() {
    string newTicker;
    double newPrice = 0.0;
    int Shares;

    cout << "Enter the asset's ticker symbol: ";
    cin >> newTicker;

    cout << "Enter the number of shares to purchase: ";
    cin >> Shares;


    newPrice = AVPrice(newTicker);

   
    bool Exists = false;
    for (auto& asset : assets) {
        if (asset.ticker == newTicker) {
            asset.shares += Shares;
            asset.price = newPrice;
            Exists = true;
            break;
        }
    }

    if (!Exists) {
        assets.push_back(Asset(newTicker, newPrice, Shares));
    }

    cout << "Asset purchased successfully!\n";
}



void Load(const std::string& username) {
    assets.clear(); 

    std::ifstream file(username + "_assets.txt");
    if (file.is_open()) {
        std::string ticker;
        double price;
        int shares;

        while (file >> ticker >> price >> shares) {
            assets.push_back(Asset(ticker, price, shares));
        }

        file.close();
    }
}



void Save(const string& username) const {
    ofstream file(username + "_assets.txt");
    if (file.is_open()) {
        for (const auto& asset : assets) {
            file << asset.ticker << " " << asset.price << " " << asset.shares << endl;
        }

        file.close();
    }
}

};



struct User {
    string username;
    string password;
    Assets userAssets;
};
void createAccount() {
    User New;

    cout << "Enter a username: ";
    cin >> New.username;

    cout << "Enter a password: ";
    cin >> New.password;

    ofstream file("user_info.txt", ios::app);
    if (file.is_open()) {
        file << New.username << " " << New.password << endl;
        file.close();
        cout << "Account created successfully!\n";
    } else {
        cerr << "Unable to open file.\n";
    }

};



int main() {
    char choice;

    cout << "Do you have an account? (y/n): ";
    cin >> choice;

    if (choice == 'n' || choice == 'N') {
        createAccount();
    } else if (choice == 'y' || choice == 'Y') {
        User user;
        bool found = false;

        cout << "Enter your username: ";
        cin >> user.username;

        cout << "Enter your password: ";
        cin >> user.password;

        ifstream file("user_info.txt");
        if (file.is_open()) {
            string currentUsername, password;

            while (file >> currentUsername >> password) {
                if (currentUsername == user.username && password == user.password) {
                    found = true;
                    break;
                }
            }

            file.close();
        } else {
            cerr << "Unable to open file.\n";
            return 1;
        }

        if (found) {
            cout << "Login successful! Welcome, " << user.username << "!\n";

            
            user.userAssets.Load(user.username);

            char assetChoice;
            cout << "Do you want to view assets (v) or purchase assets (p)? ";
            cin >> assetChoice;

            if (assetChoice == 'v' || assetChoice == 'V') {
                user.userAssets.View();
            } else if (assetChoice == 'p' || assetChoice == 'P') {
                user.userAssets.Purchase();
                user.userAssets.Save(user.username);
            } else {
                cout << "Invalid choice.\n";
            }
        } else {
            cout << "Login failed. Please check your username and password.\n";
            return 1;
        }
    } else {
        cout << "Invalid choice.\n";
        return 1;
    }

    return 0;
}

