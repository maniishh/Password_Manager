#include<bits/stdc++.h>
using namespace std;

const string PIN_FILE = "pin.txt";
const string DATA_FILE = "credentials.txt";
const char XOR_KEY = 'K'; 

struct Credential {
    string website;
    string username;
    string password;
};

string xorEncryptDecrypt(const string& input) {
    string output = input;
    for (char& c : output) {
        c ^= XOR_KEY;
    }
    return output;
}

string getHiddenInput() {
    string input;
    char ch;
    while ((ch = getchar()) != '\n') {
        if (ch == '\b') continue;
        cout << '*';
        input += ch;
    }
    cout << endl;
    return input;
}

bool verifyPIN() {
    ifstream pinFile(PIN_FILE);
    string storedPIN;
    if (!pinFile) {
        cout << "Set your master PIN: ";
        string newPIN;
        cin >> newPIN;
        ofstream out(PIN_FILE);
        out << xorEncryptDecrypt(newPIN);
        cout << "Master PIN set successfully.\n";
        return true;
    } else {
        getline(pinFile, storedPIN);
        cout << "Enter master PIN: ";
        string inputPIN;
        cin >> inputPIN;
        return xorEncryptDecrypt(storedPIN) == inputPIN;
    }
}

void addCredential() {
    Credential cred;
    cout << "Website: "; cin >> cred.website;
    cout << "Username: "; cin >> cred.username;
    cout << "Password: "; cin >> cred.password;

    ofstream file(DATA_FILE, ios::app);
    string line = cred.website + '&' + cred.username + '&' + cred.password;
    file << xorEncryptDecrypt(line) << endl;
    cout << "Credential saved.\n";
}

void viewCredentials() {
    ifstream file(DATA_FILE);
    string line;
    cout << "\nStored Credentials:\n";
    cout << left << setw(20) << "Website" << setw(20) << "Username" << "Password\n";
    cout << "----------------------------------------------------------\n";
    while (getline(file, line)) {
        string decrypted = xorEncryptDecrypt(line);
        size_t pos1 = decrypted.find('&');
        size_t pos2 = decrypted.find('&', pos1 + 1);
        if (pos1 == string::npos || pos2 == string::npos) continue;

        string site = decrypted.substr(0, pos1);
        string user = decrypted.substr(pos1 + 1, pos2 - pos1 - 1);
        string pass = decrypted.substr(pos2 + 1);

        cout << left << setw(20) << site << setw(20) << user << pass << endl;
    }
    cout << endl;
}

void deleteCredential() {
    cout << "Enter website to delete: ";
    string target;
    cin >> target;

    ifstream file(DATA_FILE);
    ofstream temp("temp.txt");

    string line;
    bool found = false;

    while (getline(file, line)) {
        string decrypted = xorEncryptDecrypt(line);
        size_t pos = decrypted.find('&');
        string site = decrypted.substr(0, pos);
        if (site != target) {
            temp << line << endl;
        } else {
            found = true;
        }
    }

    file.close();
    temp.close();
    remove(DATA_FILE.c_str());
    rename("temp.txt", DATA_FILE.c_str());

    if (found)
        cout << "Deleted successfully.\n";
    else
        cout << "Website not found.\n";
}

string generatePassword(int length) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    string password;
    srand(time(0));
    for (int i = 0; i < length; i++) {
        password += chars[rand() % chars.length()];
    }
    return password;
}

string checkStrength(const string& password) {
    int score = 0;
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSymbol = false;

    for (char c : password) {
        if (islower(c)) hasLower = true;
        else if (isupper(c)) hasUpper = true;
        else if (isdigit(c)) hasDigit = true;
        else hasSymbol = true;
    }

    if (hasLower) score++;
    if (hasUpper) score++;
    if (hasDigit) score++;
    if (hasSymbol) score++;
    if (password.length() >= 8) score++;

    if (score <= 2) return "Weak";
    if (score == 3) return "Moderate";
    return "Strong";
}

void menu() {
    while (true) {
        cout << "\n=== Password Manager ===\n";
        cout << "1. Add Credential\n";
        cout << "2. View Credentials\n";
        cout << "3. Delete Credential\n";
        cout << "4. Generate Password\n";
        cout << "5. Check Password Strength\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: addCredential(); break;
            case 2: viewCredentials(); break;
            case 3: deleteCredential(); break;
            case 4: {
                int len;
                cout << "Length: ";
                cin >> len;
                cout << "Generated: " << generatePassword(len) << endl;
                break;
            }
            case 5: {
                string pass;
                cout << "Enter password: ";
                cin >> pass;
                cout << "Strength: " << checkStrength(pass) << endl;
                break;
            }
            case 0: exit(0);
            default: cout << "Invalid option.\n";
        }
    }
}

int main() {
    cout << "=== Welcome to CLI Password Manager ===\n";
    if (verifyPIN()) {
        menu();
    } else {
        cout << "Invalid PIN. Access Denied.\n";
    }
    return 0;
}
