#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

char* allocateArray(const char* text) {
	if (text == nullptr)return nullptr;
	int size = strlen(text) + 1;
	char* newText = new char[size];
	strcpy_s(newText, size, text);
	return newText;
}
enum bookType { NOVEL, COMEDY, SCIENCE, BIOGRAPHY, OTHER };
ostream& operator<<(ostream& os, bookType type) {
	switch (type)
	{
	case NOVEL:
		os << "NOVEL";
		break;
	case COMEDY:
		os << "COMEDY";
		break;
	case SCIENCE:
		os << "SCIENCE";
		break;
	case BIOGRAPHY:
		os << "BIOGRAPHY";
		break;
	case OTHER:
		os << "OTHER";
		break;
	default:
		os << "OTHER";
		break;
	}
	return os;
}
int getRandomNumber(int from, int to) {
	return rand() % (to - from + 1) + from;
}
string generateISBN() {
	srand(time(0));
	string isbn = "978";
	for (int i = 0; i < 9; i++) {
		isbn += to_string(getRandomNumber(0, 9));
	}
	int sum = 0;
	for (int i = 0; i < 12; i++) {
		int digit = isbn[i] - '0';
		if (i % 2 == 0)sum += digit;
		else sum += digit * 3;
	}
	int checkDigit = (10 - (sum % 10)) % 10;
	isbn += to_string(checkDigit);

	return allocateArray(isbn.c_str());
}
bool validateISBN(const string& isbn) {
	regex isbn13("97[89][-\\s]?\\d{1,5}[-\\s]?\\d{1,7}[-\\s]?\\d{1,7}[-\\s]?\\d");
	regex isbn10("\\d{1,5}[-\\s]?\\d{1,7}[-\\s]?\\d{1,7}[-\\s]?[\\dX]");
	return regex_match(isbn, isbn13) || regex_match(isbn, isbn10);
}
class Book {
private:
	string _title;
	string _author;
	string _isbn;
	bool _isAvailable;
	bookType _type;
public:
	Book(string t = "", string a = "", string i = "", bookType tp = OTHER) 
		: _title(t), _author(a), _isbn(i), _isAvailable(true), _type(tp) {}

	//Getters
	string getTitle() const { return _title; }
	string getAuthor() const { return _author; }
	string getISBN() const { return _isbn; }
	bool available() const { return _isAvailable; }
	bookType getType() const { return _type; }

	//Methods
	void borrowBook() { _isAvailable = false; }
	void returnBook() { _isAvailable = true; }

	void display() const {
		cout << "Title: " << getTitle() << "\n";
		cout << " | Author: " << getAuthor() << "\n";
		cout << " | ISBN: " << getISBN() << "\n";
		cout << " | Type: " << getType() << "\n";
		cout << " | Status: " << (available() ? "Available" : "Rented") << "\n";
		cout << endl;
	}
	string serialize() const {
		return _title + "|" + _author + "|" + _isbn + "|" + 
			(_isAvailable ? "1" : "0") + "|" + to_string(_type);
	}
	static Book deserialize(const string& line) {
		size_t pos1 = line.find("|");
		size_t pos2 = line.find("|", pos1 + 1);
		size_t pos3 = line.find("|", pos2 + 1);
		size_t pos4 = line.find("|", pos3 + 1);

		string title = line.substr(0, pos1);
		string author = line.substr(pos1 + 1, pos2 - pos1 - 1);
		string isbn = line.substr(pos2 + 1, pos3 - pos2 - 1);
		bool status = (line.substr(pos3 + 1, pos4 - pos3 - 1) == "1");
		int type = stoi(line.substr(pos4 + 1));
		Book b(title, author, isbn, (bookType)type);
		if (!status) b.borrowBook();
		return b;
	}
};
class Library {
private:
	vector<Book> books;
public:
	void addBook(const Book& b) {
		books.push_back(b);
	}
	void showBooks() const {
		if (books.empty()) {
			cout << "Library is empty." << endl;
			return;
		}
		for (const auto& b : books)
			b.display();
	}
	void searchByTitle(const string& title) const {
		for (const auto& b : books) {
			if (b.getTitle() == title) {
				b.display();
				return;
			}
		}
		cout << "Book not found." << endl;
	}
	void searchByType(bookType type) const {
		bool found = false;
		for (const auto& b : books) {
			if (b.getType() == type) {
				b.display();
				found = true;
			}
		}
		if(!found)
		cout << "There are no books of that type." << endl;
	}
	void borrowBook(const string& isbn) {
		for (auto& b : books) {
			if (b.getISBN() == isbn) {
				if (b.available()) {
					b.borrowBook();
					cout << "You have successfully rented a book.";
				}
				else
					cout << "The book has already been rented.";
				return;
			}
		}
		cout << "Book not found." << endl;
	}
	void returnBook(const string& isbn) {
		for (auto& b : books) {
			if (b.getISBN() == isbn) {
				if (!b.available()) {
					b.returnBook();
					cout << "You have successfully returned a book.";
				}
				else
					cout << "The book is already available.";
				return;
			}
		}
		cout << "Book not found." << endl;
	}
	void saveToFile(const string& fileName) {
		ofstream file(fileName);
		for (const auto& b : books) {
			file << b.serialize() << endl;
		}
		file.close();
	}
	void loadFromFile(const string& fileName) {
		ifstream file(fileName);
		string line;
		while (getline(file, line))
			books.push_back(Book::deserialize(line));
		file.close();
	}
};



int main()
{
	srand(time(0));
	Library lib;
	lib.loadFromFile("library.txt");
	int choice;
	do
	{
		cout << "\n--- Library Managment System ---\n";
		cout << "1. Add a book\n";
		cout << "2. Show all books\n";
		cout << "3. Search for a book by title\n";
		cout << "4. Search for a book by type\n";
		cout << "5. Rent a book\n";
		cout << "6. Return a book\n";
		cout << "7. Exit\n";
		cout << "Entry: ";
		cin >> choice;
		cin.ignore();
		if (choice == 1) {
			string title, author, isbn;
			int type;
			cout << "Enter title: "; getline(cin, title);
			cout << "Enter author: "; getline(cin, author);
			cout << "Enter ISBN or press ENTER for automatic generation: "; getline(cin, isbn);
			if (isbn.empty()) {
				isbn = generateISBN();
				cout << "Automatically generated ISBN: " << isbn << endl;
			}
			else if (!validateISBN(isbn)) {
				cout << "ISBN not valid. " << endl;
				continue;
			}
			cout << "Choose book type: \n";
			cout << "1. NOVEL\n2. COMEDY\n3. SCIENCE\n4. BIOGRAPHY\n5. OTHER\nEntry:";
			cin >> type;
			cin.ignore();
			lib.addBook(Book(title, author, isbn, (bookType)(type - 1)));
		}
		else if (choice == 2)
				lib.showBooks();
		else if (choice == 3) {
			string t;
			cout << "Enter title: "; getline(cin, t);
			lib.searchByTitle(t);
		}
		else if (choice == 4) {
			int type;
			cout << "Choose book type: \n";
			cout << "1. NOVEL\n2. COMEDY\n3. SCIENCE\n4. BIOGRAPHY\n5. OTHER\nEntry:";
			cin >> type;
			cin.ignore();
			lib.searchByType((bookType)(type - 1));
		}
		else if (choice == 5) {
			string isbn;
			cout << "Enter the rental book ISBN: "; getline(cin, isbn);
			lib.borrowBook(isbn);
		}
		else if (choice == 6) {
			string isbn;
			cout << "Enter the book ISBN for return: "; getline(cin, isbn);
			lib.returnBook(isbn);
		}

	} while (choice != 7);
	lib.saveToFile("library.txt");
	cout << "Data saved. End od program.";

	return 0;
}