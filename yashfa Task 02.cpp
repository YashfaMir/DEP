#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm> // Required for std::remove_if

using namespace std;

class Contact {
public:
    string name;
    string phoneNum;

    Contact(string n, string p) : name(n), phoneNum(p) {}
};

class ContactManager {
private:
    vector<Contact> contacts;

    void loadContacts() {
        ifstream infile("Contact.txt");
        if (!infile.is_open()) {
            cerr << "Unable to open file Contact.txt" << endl;
            return;
        }

        string name, phoneNumber;
        while (getline(infile, name) && getline(infile, phoneNumber)) {
            contacts.emplace_back(name, phoneNumber);
        }

        infile.close();
    }

    void saveContactsToFile() {
        ofstream outfile("Contact.txt");
        if (!outfile.is_open()) {
            cerr << "Unable to open file" << endl;
            return;
        }

        for (const auto& contact : contacts) {
            outfile << contact.name << endl;
            outfile << contact.phoneNum << endl;
        }

        outfile.close();
    }

public:
    ContactManager() {
        loadContacts();
    }

    ~ContactManager() {
        saveContactsToFile();
    }

    void addContact(const string& name, const string& phoneNumber) {
        contacts.emplace_back(name, phoneNumber);
        cout << "Contact added successfully!" << endl;
        saveContactsToFile();
    }

    void viewContacts() const {
        if (contacts.empty()) {
            cout << "No contacts available." << endl;
            return;
        }

        cout << "Contacts:" << endl;
        for (const auto& contact : contacts) {
            cout << "Name: " << contact.name << ", Phone Number: " << contact.phoneNum << endl;
        }
    }

    void deleteContact(const string& name) {
        auto it = remove_if(contacts.begin(), contacts.end(), [&](const Contact& contact) {
            return contact.name == name;
        });

        if (it != contacts.end()) {
            contacts.erase(it, contacts.end());
            cout << "Contact deleted!" << endl;
            saveContactsToFile();
        } else {
            cout << "Contact not found." << endl;
        }
    }
};

void showMenu() {
    cout << "\nContact Management System" << endl;
    cout << "1. Add Contact" << endl;
    cout << "2. View Contacts" << endl;
    cout << "3. Delete Contact" << endl;
    cout << "4. Exit" << endl;
    cout << "Choose an option: ";
}

int main() {
    ContactManager manager;
    int choice;
    string name, phoneNumber;

    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            cout << "Enter name: ";
            getline(cin, name);
            cout << "Enter phone number: ";
            getline(cin, phoneNumber);
            manager.addContact(name, phoneNumber);
            break;
        case 2:
            manager.viewContacts();
            break;
        case 3:
            cout << "Enter name to delete: ";
            getline(cin, name);
            manager.deleteContact(name);
            break;
        case 4:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

