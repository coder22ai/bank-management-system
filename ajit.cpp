#include <iostream>
#include <fstream>
#include <string>
#include <cstring> // Added for string operations
using namespace std;

// ---------------- Base Class ----------------
class Transaction {
protected:
    string type;
    float amount;
    string category;

public:
    Transaction() {
        type = "";
        amount = 0.0;
        category = "";
    }

    Transaction(string t, float a, string c) {
        type = t;
        amount = a;
        category = c;
    }

    virtual void display() {
        cout << "Type: " << type << "\nAmount: " << amount << "\nCategory: " << category << endl;
    }

    float getAmount() { return amount; }
    string getType() { return type; }
    string getCategory() { return category; }
};

// ---------------- Derived Class ----------------
class DetailedTransaction : public Transaction {
    string date;
    string note;

public:
    DetailedTransaction() : Transaction() {
        date = "N/A";
        note = "N/A";
    }

    DetailedTransaction(string t, float a, string c, string d, string n)
        : Transaction(t, a, c) {
        date = d;
        note = n;
    }

    void display() override {
        cout << "\n------ Transaction Details ------" << endl;
        cout << "Type: " << type << endl;
        cout << "Amount: " << amount << endl;
        cout << "Category: " << category << endl;
        cout << "Date: " << date << endl;
        cout << "Note: " << note << endl;
    }

    string getDate() { return date; }
    string getNote() { return note; }
};

// ---------------- User Account Class ----------------
class FinanceTracker {
    string userName;
    float balance;
    DetailedTransaction *transactions;
    int count;
    int capacity; // Added to track array capacity
    static int userCount;

public:
    FinanceTracker(string name, float initialBalance = 0.0) {
        userName = name;
        balance = initialBalance;
        count = 0;
        capacity = 100;
        transactions = new DetailedTransaction[capacity];
        userCount++;
    }

    // Copy constructor for proper memory management
    FinanceTracker(const FinanceTracker& other) {
        userName = other.userName;
        balance = other.balance;
        count = other.count;
        capacity = other.capacity;
        transactions = new DetailedTransaction[capacity];
        for (int i = 0; i < count; i++) {
            transactions[i] = other.transactions[i];
        }
        userCount++;
    }

    // Assignment operator
    FinanceTracker& operator=(const FinanceTracker& other) {
        if (this != &other) {
            delete[] transactions;
            userName = other.userName;
            balance = other.balance;
            count = other.count;
            capacity = other.capacity;
            transactions = new DetailedTransaction[capacity];
            for (int i = 0; i < count; i++) {
                transactions[i] = other.transactions[i];
            }
        }
        return *this;
    }

    ~FinanceTracker() {
        delete[] transactions;
    }

    static void showTotalUsers() {
        cout << "\nTotal Users: " << userCount << endl;
    }

    void addTransaction(string type, float amount, string category, string date, string note) {
        try {
            if (amount <= 0)
                throw "Amount must be positive!";

            // Check if array needs resizing
            if (count >= capacity) {
                resizeArray();
            }

            transactions[count] = DetailedTransaction(type, amount, category, date, note);
            if (type == "income")
                balance += amount;
            else if (type == "expense")
                balance -= amount;
            else
                throw "Type must be 'income' or 'expense'!";

            count++;
            cout << "\nTransaction Added Successfully!\n";

        } catch (const char *msg) {
            cout << "Error: " << msg << endl;
        }
    }

    void viewAll() {
        if (count == 0) {
            cout << "\nNo transactions to display." << endl;
            return;
        }
        
        cout << "\n=== " << userName << "'s Transaction History ===" << endl;
        for (int i = 0; i < count; i++)
            transactions[i].display();
        cout << "\nCurrent Balance: " << balance << endl;
    }

    // ✅ Improved File Handling
    void saveToFile() {
        string filename = userName + "_finance.txt";
        ofstream file(filename);

        if (!file.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        file << "---- Personal Finance Tracker ----\n";
        file << "User Name: " << userName << endl;
        file << "Balance: " << balance << endl;
        file << "Total Transactions: " << count << endl;
        file << "===================================\n";

        for (int i = 0; i < count; i++) {
            file << "\nTransaction " << i + 1 << ":\n";
            file << "Type: " << transactions[i].getType() << endl;
            file << "Amount: " << transactions[i].getAmount() << endl;
            file << "Category: " << transactions[i].getCategory() << endl;
            file << "Date: " << transactions[i].getDate() << endl;
            file << "Note: " << transactions[i].getNote() << endl;
            file << "-----------------------------\n";
        }

        file.close();
        cout << "\n✅ All Transaction Details Saved to File: " << filename << endl;
        
        // Verify file was created
        ifstream checkFile(filename);
        if (checkFile.good()) {
            cout << "✅ File verified and created successfully!" << endl;
        } else {
            cout << "❌ File creation failed!" << endl;
        }
        checkFile.close();
    }

    // Helper function to resize array
    void resizeArray() {
        capacity *= 2;
        DetailedTransaction* newTransactions = new DetailedTransaction[capacity];
        for (int i = 0; i < count; i++) {
            newTransactions[i] = transactions[i];
        }
        delete[] transactions;
        transactions = newTransactions;
        cout << "Array resized to capacity: " << capacity << endl;
    }

    string getUserName() const { return userName; }
    float getBalance() const { return balance; }

    // Friend function example
    friend void compareBalance(const FinanceTracker& f1, const FinanceTracker& f2);
};

int FinanceTracker::userCount = 0;

// Friend function - using references to avoid copying
void compareBalance(const FinanceTracker& f1, const FinanceTracker& f2) {
    cout << "\nComparing balances of " << f1.userName << " and " << f2.userName << endl;
    if (f1.balance > f2.balance)
        cout << f1.userName << " has more savings: $" << f1.balance << " vs $" << f2.balance << endl;
    else if (f1.balance < f2.balance)
        cout << f2.userName << " has more savings: $" << f2.balance << " vs $" << f1.balance << endl;
    else
        cout << "Both have equal balance: $" << f1.balance << endl;
}

// Template function
template <class T>
void showMessage(T msg) {
    cout << "\n[Message]: " << msg << endl;
}

// ---------------- Main Function ----------------
int main() {
    string name;
    cout << "Enter your name: ";
    cin >> name;

    FinanceTracker user(name, 1000);

    int choice;
    do {
        cout << "\n------ Personal Finance Tracker ------" << endl;
        cout << "1. Add Transaction\n2. View All Transactions\n3. Save to File\n4. Create Another User\n5. Exit\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string type, category, date, note;
            float amount;

            cout << "Enter type (income/expense): ";
            cin >> type;
            cout << "Enter amount: ";
            cin >> amount;
            cout << "Enter category: ";
            cin >> category;
            cout << "Enter date (dd/mm/yyyy): ";
            cin >> date;
            cout << "Enter note: ";
            cin.ignore();
            getline(cin, note);

            user.addTransaction(type, amount, category, date, note);
        } 
        else if (choice == 2) {
            user.viewAll();
        } 
        else if (choice == 3) {
            user.saveToFile();
        } 
        else if (choice == 4) {
            string anotherName;
            cout << "Enter new user name: ";
            cin >> anotherName;
            FinanceTracker newUser(anotherName, 500);
            
            // Add some sample transactions to new user
            newUser.addTransaction("income", 1000, "salary", "01/01/2024", "Monthly salary");
            newUser.addTransaction("expense", 200, "food", "02/01/2024", "Groceries");
            
            compareBalance(user, newUser);
            FinanceTracker::showTotalUsers();
            
            // Option to save new user's data
            cout << "Do you want to save new user's data to file? (y/n): ";
            char saveChoice;
            cin >> saveChoice;
            if (saveChoice == 'y' || saveChoice == 'Y') {
                newUser.saveToFile();
            }
        }
    } while (choice != 5);

    showMessage("Thank you for using Personal Finance Tracker!");
    return 0;
}