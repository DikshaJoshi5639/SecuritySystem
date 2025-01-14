#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>
using namespace std;

class Password {
    string category;
    string description;
    string value;
public:
    Password(string cat, string desc, string val) : category(cat), description(desc), value(val) {}
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    string getValue() const { return value; }
    void setValue(string val) { value = val; }
};


class PasswordManager {
    vector<Password> passwords;
    vector<string> categories = {"Default"};
    string filename = "passwords.txt";

public:
    void generateRandomPassword(string& password, int length) {
        const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
        random_device rd;
        mt19937 generator(rd());
        uniform_int_distribution<> dist(0, chars.size() - 1);
        password.clear();
        for (int i = 0; i < length; ++i) {
            password += chars[dist(generator)];
        }
    }

    void savePasswordsToFile() {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error opening file for saving.\n";
            return;
        }
        for (const auto& p : passwords) {
            file << p.getCategory() << "\n" << p.getDescription() << "\n" << p.getValue() << "\n";
        }
        file.close();
        cout << "Passwords saved successfully to " << filename << ".\n";
    }

    void loadPasswordsFromFile() {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error opening file for loading.\n";
            return;
        }
        passwords.clear();
        string category, description, value;
        while (getline(file, category) && getline(file, description) && getline(file, value)) {
            passwords.emplace_back(category, description, value);
        }
        file.close();
        cout << "Passwords loaded successfully from " << filename << ".\n";
    }

    void addPassword() {
        string category, description, value;
        cout << "Enter category: ";
        getline(cin, category);
        cout << "Enter description: ";
        getline(cin, description);
        int choice;
        cout << "1. Enter password manually\n2. Generate random password\nChoose an option: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            cout << "Enter password: ";
            getline(cin, value);
        } else if (choice == 2) {
            int length;
            cout << "Enter length of the password: ";
            cin >> length;
            cin.ignore();
            generateRandomPassword(value, length);
            cout << "Generated password: " << value << "\n";
        } else {
            cout << "Invalid choice.\n";
            return;
        }
        passwords.emplace_back(category, description, value);
        cout << "Password added successfully.\n";
    }

    void deletePassword() {
        string description;
        cout << "Enter the description of the password to delete: ";
        getline(cin, description);
        auto it = remove_if(passwords.begin(), passwords.end(), [&](const Password& p) {
            return p.getDescription() == description;
        });
        if (it != passwords.end()) {
            passwords.erase(it, passwords.end());
            cout << "Password deleted successfully.\n";
        } else {
            cout << "Password not found.\n";
        }
    }

    void searchPassword() {
        string keyword;
        cout << "Enter a keyword to search: ";
        getline(cin, keyword);
        bool found = false;
        for (const auto& p : passwords) {
            if (p.getCategory().find(keyword) != string::npos || p.getDescription().find(keyword) != string::npos) {
                cout << "Category: " << p.getCategory() << "\nDescription: " << p.getDescription() << "\nPassword: " << p.getValue() << "\n\n";
                found = true;
            }
        }
        if (!found) {
            cout << "No matching passwords found.\n";
        }
    }

    void editPassword() {
        string description;
        cout << "Enter the description of the password to edit: ";
        getline(cin, description);
        for (auto& p : passwords) {
            if (p.getDescription() == description) {
                string newValue;
                cout << "Enter new password: ";
                getline(cin, newValue);
                p.setValue(newValue);
                cout << "Password updated successfully.\n";
                return;
            }
        }
        cout << "Password not found.\n";
    }

    void sortPasswords() {
        sort(passwords.begin(), passwords.end(), [](const Password& a, const Password& b) {
            return a.getCategory() < b.getCategory();
        });
        cout << "Passwords sorted by category.\n";
    }

    void manageCategories() {
        int choice;
        cout << "1. Add a category\n2. Remove a category\nChoose an option: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            string newCategory;
            cout << "Enter new category: ";
            getline(cin, newCategory);
            categories.push_back(newCategory);
            cout << "Category added successfully.\n";
        } else if (choice == 2) {
            string categoryToRemove;
            cout << "Enter category to remove: ";
            getline(cin, categoryToRemove);
            auto it = find(categories.begin(), categories.end(), categoryToRemove);
            if (it != categories.end()) {
                categories.erase(it);
                cout << "Category removed successfully.\n";
            } else {
                cout << "Category not found.\n";
            }
        } else {
            cout << "Invalid choice.\n";
        }
    }

    void displayMenu() {
        cout << "\nPassword Manager Menu:\n";
        cout << "1. Add a password\n";
        cout << "2. Delete a password\n";
        cout << "3. Search for a password\n";
        cout << "4. Edit a password\n";
        cout << "5. Sort passwords\n";
        cout << "6. Manage categories\n";
        cout << "7. Save passwords to file\n";
        cout << "8. Load passwords from file\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
    }

    void run() {
        int choice;
        do {
            displayMenu();
            cin >> choice;
            cin.ignore();
            switch (choice) {
                case 1:
                    addPassword();
                    break;
                case 2:
                    deletePassword();
                    break;
                case 3:
                    searchPassword();
                    break;
                case 4:
                    editPassword();
                    break;
                case 5:
                    sortPasswords();
                    break;
                case 6:
                    manageCategories();
                    break;
                case 7:
                    savePasswordsToFile();
                    break;
                case 8:
                    loadPasswordsFromFile();
                    break;
                case 9:
                    cout << "Exiting Password Manager.\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 9);
    }
};

int main() {
    PasswordManager manager;
    manager.run();
    return 0;
}
