#include <iostream>
#include <fstream>
#include <string>
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
    }

    void showAccount() {
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Name: " << name << "\n";
        cout << "Balance: " << balance << "\n";
    }

    void deposit() {
        double amt;
        cout << "Enter amount to deposit: ";
        cin >> amt;
        balance += amt;
        cout << "Amount Deposited Successfully!\n";
    }

    void withdraw() {
        double amt;
        cout << "Enter amount to withdraw: ";
        cin >> amt;
        if (amt > balance)
            cout << "Insufficient Balance!\n";
        else {
            balance -= amt;
            cout << "Amount Withdrawn Successfully!\n";
        }
    }

    int getAccountNumber() {
        return accountNumber;
    }

    void saveToFile() {
        ofstream outFile("accounts.dat", ios::app | ios::binary);
        outFile.write(reinterpret_cast<char*>(this), sizeof(*this));
        outFile.close();
    }

    void updateInFile() {
        fstream file("accounts.dat", ios::in | ios::out | ios::binary);
        Bank temp;
        while(file.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            if(temp.accountNumber == accountNumber) {
                file.seekp(-static_cast<int>(sizeof(temp)), ios::cur);
                file.write(reinterpret_cast<char*>(this), sizeof(*this));
                break;
            }
        }
        file.close();
    }

    static void displayAllAccounts() {
        ifstream inFile("accounts.dat", ios::binary);
        Bank temp;
        cout << "------ All Accounts ------\n";
        while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            temp.showAccount();
            cout << "-------------------------\n";
        }
        inFile.close();
    }

    static void searchAccount(int accNum) {
        ifstream inFile("accounts.dat", ios::binary);
        Bank temp;
        bool found = false;
        while(inFile.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            if(temp.accountNumber == accNum) {
                cout << "Account Found:\n";
                temp.showAccount();
                found = true;
                break;
            }
        }
        if(!found)
            cout << "Account Not Found!\n";
        inFile.close();
    }

    static void deleteAccount(int accNum) {
        ifstream inFile("accounts.dat", ios::binary);
        ofstream outFile("temp.dat", ios::binary);
        Bank temp;
        bool found = false;
        while(inFile.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            if(temp.accountNumber != accNum) {
                outFile.write(reinterpret_cast<char*>(&temp), sizeof(temp));
            } else found = true;
        }
        inFile.close();
        outFile.close();
        remove("accounts.dat");
        rename("temp.dat", "accounts.dat");
        if(found) cout << "Account Deleted Successfully!\n";
        else cout << "Account Not Found!\n";
    }
};

int main() {
    Bank b;
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

        switch(choice) {
            case 1:
                b.createAccount();
                b.saveToFile();
                break;
            case 2:
                Bank::displayAllAccounts();
                break;
            case 3: {
                int acc;
                cout << "Enter Account Number to search: ";
                cin >> acc;
                Bank::searchAccount(acc);
                break;
            }
            case 4: {
                int acc;
                cout << "Enter Account Number to deposit: ";
                cin >> acc;
                Bank::searchAccount(acc); // show current details
                fstream file("accounts.dat", ios::in | ios::out | ios::binary);
                Bank temp;
                while(file.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
                    if(temp.getAccountNumber() == acc) {
                        temp.deposit();
                        file.seekp(-static_cast<int>(sizeof(temp)), ios::cur);
                        file.write(reinterpret_cast<char*>(&temp), sizeof(temp));
                        break;
                    }
                }
                file.close();
                break;
            }
            case 5: {
                int acc;
                cout << "Enter Account Number to withdraw: ";
                cin >> acc;
                fstream file("accounts.dat", ios::in | ios::out | ios::binary);
                Bank temp;
                while(file.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
                    if(temp.getAccountNumber() == acc) {
                        temp.withdraw();
                        file.seekp(-static_cast<int>(sizeof(temp)), ios::cur);
                        file.write(reinterpret_cast<char*>(&temp), sizeof(temp));
                        break;
                    }
                }
                file.close();
                break;
            }
            case 6: {
                int acc;
                cout << "Enter Account Number to delete: ";
                cin >> acc;
                Bank::deleteAccount(acc);
                break;
            }
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid Choice!\n";
        }

    } while(choice != 7);

    return 0;
}
