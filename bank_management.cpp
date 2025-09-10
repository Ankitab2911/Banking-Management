#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Bank {
    int accountNumber;
    string name;
    double balance;

public:
    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Initial Balance: ";
        cin >> balance;
        cout << "Account Created Successfully!\n";
        saveToFile();
    }

    void showAccount() const {
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Name: " << name << "\n";
        cout << "Balance: " << balance << "\n";
    }

    void deposit(double amt) {
        balance += amt;
    }

    void withdraw(double amt) {
        if (amt > balance)
            cout << "Insufficient Balance!\n";
        else
            balance -= amt;
    }

    int getAccountNumber() const { return accountNumber; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    // Save account to file
    void saveToFile() {
        ofstream outFile("accounts.txt", ios::app);
        outFile << accountNumber << "," << name << "," << balance << "\n";
        outFile.close();
    }

    // Update entire file after modification
    static void updateFile(const vector<Bank>& accounts) {
        ofstream outFile("accounts.txt");
        for (const auto &acc : accounts) {
            outFile << acc.accountNumber << "," << acc.name << "," << acc.balance << "\n";
        }
        outFile.close();
    }

    // Load all accounts from file
    static vector<Bank> loadFromFile() {
        vector<Bank> accounts;
        ifstream inFile("accounts.txt");
        Bank temp;
        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            size_t pos1 = line.find(',');
            size_t pos2 = line.rfind(',');
            temp.accountNumber = stoi(line.substr(0, pos1));
            temp.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            temp.balance = stod(line.substr(pos2 + 1));
            accounts.push_back(temp);
        }
        inFile.close();
        return accounts;
    }
};

int main() {
    int choice;
    do {
        cout << "\n====== Bank Management System ======\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Search Account\n";
        cout << "4. Deposit Money\n";
        cout << "5. Withdraw Money\n";
        cout << "6. Delete Account\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        vector<Bank> accounts = Bank::loadFromFile();
        bool found = false;

        switch (choice) {
            case 1: {
                Bank b;
                b.createAccount();
                break;
            }
            case 2: {
                for (auto &acc : accounts) {
                    acc.showAccount();
                    cout << "-------------------------\n";
                }
                break;
            }
            case 3: {
                int accNo;
                cout << "Enter Account Number: ";
                cin >> accNo;
                for (auto &acc : accounts) {
                    if (acc.getAccountNumber() == accNo) {
                        acc.showAccount();
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account Not Found!\n";
                break;
            }
            case 4: {
                int accNo;
                double amt;
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter Amount to Deposit: ";
                cin >> amt;
                for (auto &acc : accounts) {
                    if (acc.getAccountNumber() == accNo) {
                        acc.deposit(amt);
                        cout << "Deposit Successful!\n";
                        found = true;
                        break;
                    }
                }
                if (found) Bank::updateFile(accounts);
                else cout << "Account Not Found!\n";
                break;
            }
            case 5: {
                int accNo;
                double amt;
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter Amount to Withdraw: ";
                cin >> amt;
                for (auto &acc : accounts) {
                    if (acc.getAccountNumber() == accNo) {
                        acc.withdraw(amt);
                        cout << "Withdrawal Processed!\n";
                        found = true;
                        break;
                    }
                }
                if (found) Bank::updateFile(accounts);
                else cout << "Account Not Found!\n";
                break;
            }
            case 6: {
                int accNo;
                cout << "Enter Account Number to Delete: ";
                cin >> accNo;
                for (auto it = accounts.begin(); it != accounts.end(); ++it) {
                    if (it->getAccountNumber() == accNo) {
                        accounts.erase(it);
                        cout << "Account Deleted Successfully!\n";
                        found = true;
                        break;
                    }
                }
                if (found) Bank::updateFile(accounts);
                else cout << "Account Not Found!\n";
                break;
            }
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid Choice!\n";
        }
    } while (choice != 7);

    return 0;
}
