#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> 
using namespace std;

// Simple custom hash function for learning
string simpleHash(const string &password) {
	unsigned int hash = 0;
	for (char c : password) {
		hash = (hash * 131) + c; // prime-based rolling hash
	}
	return to_string(hash);
}

// Password masking function
string getMaskedPassword() {
	string password;
	char ch;
	while ((ch = _getch()) != '\r') { // Enter key stops input
		if (ch == '\b') { // Handle backspace
			if (!password.empty()) {
				cout << "\b \b";
				password.pop_back();
			}
		}
		else {
			password.push_back(ch);
			cout << '*';
		}
	}
	cout << endl;
	return password;
}

// Check if username already exists
bool usernameExists(const string &username) {
	ifstream file("users.txt");
	string fileUser, filePass;
	while (file >> fileUser >> filePass) {
		if (fileUser == username) {
			return true;
		}
	}
	return false;
}

// Registration process
void registerUser() {
	string username, password, confirmPassword;

	cout << "\n=== User Registration ===\n";

	while (true) {
		cout << "Enter username: ";
		cin >> username;

		if (usernameExists(username)) {
			cout << "Error: Username already exists. Try again.\n";
			continue; // Go back and ask again
		}

		cout << "Enter password: ";
		password = getMaskedPassword();

		cout << "Confirm password: ";
		confirmPassword = getMaskedPassword();

		if (password != confirmPassword) {
			cout << "Error: Passwords do not match. Try again.\n";
			continue; // Retry without going back to main menu
		}

		if (password.empty()) {
			cout << "Error: Password cannot be empty. Try again.\n";
			continue;
		}

		// Store username and hashed password
		ofstream file("users.txt", ios::app);
		file << username << " " << simpleHash(password) << "\n";
		file.close();

		cout << "Registration successful! You can now log in.\n";
		break; // Exit registration loop
	}
}

// Login process
void loginUser() {
	string username, password;
	int attempts = 3;

	cout << "\n=== User Login ===\n";
	while (attempts > 0) {
		cout << "Enter username: ";
		cin >> username;
		cout << "Enter password: ";
		password = getMaskedPassword();

		ifstream file("users.txt");
		string fileUser, filePass;
		bool found = false;

		while (file >> fileUser >> filePass) {
			if (fileUser == username && filePass == simpleHash(password)) {
				cout << "Login successful. Welcome, " << username << "!\n";
				found = true;
				break;
			}
		}
		file.close();

		if (found) return;

		attempts--;
		cout << "Invalid username or password. Attempts left: " << attempts << "\n";
	}

	cout << "Too many failed attempts. Account locked temporarily.\n";
}

int main() {
	int choice;

	do {
		cout << "\n===== LOGIN & REGISTRATION SYSTEM =====\n";
		cout << "1. Register\n";
		cout << "2. Login\n";
		cout << "3. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			registerUser();
			break;
		case 2:
			loginUser();
			break;
		case 3:
			cout << "Exiting program. Goodbye!\n";
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}

	} while (choice != 3);

	system("pause");
	return 0;
}
