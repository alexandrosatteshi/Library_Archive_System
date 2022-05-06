#include<iostream>
#include<iomanip>
#include<cstring>
#include<Windows.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// CONSTANT VARIABLES ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Const Fixed Variables
const int LINE_SIZE = 120;					//Length of the program's lines displayed
const int BOOK_DISPLAY = 20;				//How far inside the row for books should be
const int BOOK_CUSTOMER = 5;				//Maximum number of books a customer can hold
const int BOOK_WARNING = 3;					//Used as a value for customers holding over that amount of books
const int CUSTOMER_MAX = 100;				//Maximum number of customers
const int BOOK_MAX = 100;					//Maximun number of book
const int WORD_DISPLAYGAP = 8;				//Space between words in print
const double BOOK_LATEAMOUNT = 0.50;		//Late books fee amount
const char TITLE[] = { "Library Archive" };	//Title of the program

//Const cstring Variable sizes
const int VAR_ISBN = 15;
const int VAR_NAME = 25;
const int VAR_ADDRESS = 30;
const int VAR_IDNUMBER = 10;
const int VAR_TITLE = 25;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// STRUCTURES ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Date
struct date {
	int day;
	int month;
	int year;
};

//Books loaned or Reserved by customer
struct loan_reserved {
	char isbn[VAR_ISBN];
	bool how;			//True = borrowed, False = Reserverd
	date due_date;
};

//Customer's Details
struct customer {
	char name[VAR_NAME];
	char addreess[VAR_ADDRESS];
	char id_number[VAR_IDNUMBER];
	loan_reserved books[BOOK_CUSTOMER];
	int loan_books;			//Number of books on loan/reserved
};

//Book's Details
struct book {
	char title[VAR_TITLE];
	char author[VAR_NAME];
	int year;
	char isbn[VAR_ISBN];
	bool onLoan = false;			//True = Book is loaned, False = Book is not loaned
	char borrower[VAR_IDNUMBER];	//The ID Number of the borrower
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// GLOBAL VARIABLES ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

customer cust_info[CUSTOMER_MAX];
book book_info[BOOK_MAX];
int register_cust = 0;		//Number of register customers in the library
int register_books = 0;		//Number of register books in the library
HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// FUNCTION HEADERS ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void line();
void line_book();
void header(const char[]);
bool int_check(int, int, int);
bool question(const char[]);
int number_customers();
bool exist_customer(char[]);
bool exist_book(char[]);
void menu();
int menu_choice();
void shift_right(int);
void clear_arraySpot(int);
int array_where(char[]);
void new_customer();
void new_book();
int locate_book(char[]);
int find_book();
int locate_customer(char[]);
int find_customer();
void reserve_book(int, int);
void input_dateValitade(int&, int&, int&);
void lend_book(int, int);
void borrow_Main();
void reserve_Main();
int find_bookCust(int, int);
void shift_left(int, int, int);
void return_book();
void print_custInfo_Header();
void print_custInfo(int);
void print_custDetails();
int find_CustLoan(int);
void print_custMaxBooks();
void print_bookInfo_Header();
void print_bookInfo(int,int,int);
void print_custMoreBooks();
bool customer_Reserve(int);
void print_bookOnReserve();
long find_overdueDateBook(date, int, int);
long find_overdueDate(date, int);
void print_overdueBooks();
void menu_select(int);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// MAIN FUNCTION ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
	int choice;		//User's choice for menu

	do {
		system("cls");
		header(TITLE);
		menu();

		choice = menu_choice();

		menu_select(choice);
	} while (choice != 11);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// FUNCTIONS /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Print's a line of preconfigured lenght
void line() {
	for (int r = 0; r < LINE_SIZE; r++) {
		cout << "-";
	}
	cout << endl;
}

//Print's a line of preconfigured lenght for book
void line_book() {
	cout << right << setw(20) << "";
	SetConsoleTextAttribute(color, 112);

	for (int r = 0; r < LINE_SIZE - BOOK_DISPLAY; r++) {
		cout << "_";
	}

	SetConsoleTextAttribute(color, 7);
	cout << endl;
}

//Print's a given title
void header(const char str[]) {
	line();
	cout << right << setw((LINE_SIZE / 2) + strlen(str) / 2) << str << endl;
	line();
}

//Checks if an input is an intiger and between given limits
bool int_check(int min, int max, int input) {
	if (!cin.good() || (input < min || input > max)) {
		cin.clear();
		cin.ignore(256, '\n');
		cout << "\aIncorect Value! Give an integer number between " << min << "-" << max << endl << endl;
		return false;
	}
	else {
		return true;
	}
}

//Ask's given question, Return True if yes, False otherwise
bool question(const char str[]) {
	bool check_if;
	char repeat;

	do {
		cout << str << "(Y/N): ";
		cin >> repeat;

		if (repeat != 'y' && repeat != 'Y' && repeat != 'n' && repeat != 'N') {
			cout << "'" << repeat << "'\a is not recognized as a program command." << endl << endl;
			check_if = true;
		}
		else {
			check_if = false;
		}
	} while (check_if);

	if (repeat == 'y' || repeat == 'Y') {
		return true;
	}
	else {
		return false;
	}
}

//Returns the number of customers
int number_customers() {
	return register_cust;
}

//Return True if customer exist, False otherwise
bool exist_customer(char id_number[]) {
	for (int r = 0; r < register_cust; r++) {
		if (strcmp(id_number, cust_info[r].id_number) == 0) {
			return true;
		}
	}
	return false;
}


//Return True if book exist in book_info, False otherwise
bool exist_book(char isbn[]) {
	for (int r = 0; r < register_books; r++) {
		if (strcmp(isbn, book_info[r].isbn) == 0) {
			return true;
		}
	}
	return false;
}

//Print's the menu of the program
void menu() {
	cout << left << setw(LINE_SIZE / 2) << "Options" << "Reports" << endl;
	cout << left << setw(LINE_SIZE / 2) << "1 - Add a new customer." << "6 - Details of customer." << endl;
	cout << left << setw(LINE_SIZE / 2) << "2 - Add a new book." << "7 - Find customer with the maximum number of books on loan." << endl;
	cout << left << setw(LINE_SIZE / 2) << "3 - Borrow book/s." << "8 - Find all customers that are holding more than " << BOOK_WARNING << " books." << endl;
	cout << left << setw(LINE_SIZE / 2) << "4 - Reserve book/s." << "9 - Reserved books." << endl;
	cout << left << setw(LINE_SIZE / 2) << "5 - Return book/s." << "10- Find all overdue books." << endl;
	cout << endl << "11- Exit." << endl;
	line();
}

//Takes the user's choice
int menu_choice() {
	int choice;

	do {
		cout << "What whould you like to do?(1-10): ";
		cin >> choice;
	} while (!int_check(1, 11, choice));

	line();

	return choice;
}

//Moves the customer array details one spot to the right
void shift_right(int array_place) {
	for (int r = register_cust; r >= array_place; r--) {
		cust_info[r + 1] = cust_info[r];
	}
}

//Clears the cust_info array spot where the new customer will be placed
void clear_arraySpot(int person_location) {
	for (int c = 0; c < VAR_NAME; c++) {
		cust_info[person_location].name[c] = '\0';
	}

	for (int c = 0; c < VAR_ADDRESS; c++) {
		cust_info[person_location].addreess[c] = '\0';
	}

	for (int c = 0; c < BOOK_CUSTOMER; c++) {
		cust_info[person_location].books[c].due_date.day = 0;
		cust_info[person_location].books[c].due_date.month = 0;
		cust_info[person_location].books[c].due_date.year = 0;

		cust_info[person_location].books[c].how = false;

		for (int r = 0; r < VAR_ISBN; r++) {
			cust_info[person_location].books[c].isbn[r] = '\0';
		}
	}

	for (int c = 0; c < VAR_IDNUMBER; c++) {
		cust_info[person_location].id_number[c] = '\0';
	}

	cust_info[person_location].loan_books = 0;
}

//Returns the place in which to insert data, Rearanges the array if nesesary to make space
int array_where(char word[]) {
	if (register_cust == 0) {
		return 0;
	}
	else {
		int array_place = 0;
		bool check = false;

		if (strcmp(word, cust_info[array_place].name) < 0) {
			return 0;
		}

		do {
			if (strcmp(word, cust_info[array_place].name) == 0) {
				array_place++;
				check = true;
			}
			else if (strcmp(word, cust_info[array_place].name) > 0 && array_place == (register_cust - 1)) {
				array_place++;
				check = true;
			}
			else if (strcmp(word, cust_info[array_place].name) > 0 && strcmp(word, cust_info[array_place + 1].name) < 0) {
				array_place++;
				check = true;
			}
			else if (strcmp(word, cust_info[array_place].name) > 0) {
				array_place++;
				check = false;
			}
		} while (!check && array_place <= register_cust);

		shift_right(array_place);
		return array_place;
	}
}

//Add a new customer to the array
void new_customer() {
	char id_num[VAR_IDNUMBER];
	char name[VAR_NAME];
	int place;				//Where in the array to insert data

	system("cls");
	header(TITLE);
	header("Add Customer");
	
	cout << "ID Number: ";
	cin >> id_num;

	if (exist_customer(id_num)) {
		cout << "\aThe customer with ID \"" << id_num << "\" already exist in the database." << endl << endl;
	}
	else {
		cin.ignore(256, '\n');

		cout << "Name: ";
		cin.getline(name, VAR_NAME);

		place = array_where(name);
		clear_arraySpot(place);

		for (int c = 0; name[c] != '\0'; c++) {
			cust_info[place].name[c] = name[c];
		}

		cust_info[place].name[strlen(cust_info[place].name)] = '\0';

		for (int c = 0; id_num[c] != '\0'; c++) {
			cust_info[place].id_number[c] = id_num[c];
		}

		cust_info[place].id_number[strlen(cust_info[place].id_number)] = '\0';

		cout << "Address: ";
		cin.getline(cust_info[place].addreess, VAR_ADDRESS);

		register_cust++;
	}
}

//Add a new book to the array
void new_book() {
	char isbn[VAR_ISBN];
	int year;

	system("cls");
	header(TITLE);
	header("Add Book");

	cout << "ISBN: ";
	cin >> isbn;

	if (exist_book(isbn)) {
		cout << endl << "\aThe book with ISBN \"" << isbn << "\" already exist in the database." << endl;
	}
	else {
		for (int c = 0; isbn[c] != '\0'; c++) {
			book_info[register_books].isbn[c] = isbn[c];
		}
		
		cust_info[register_books].name[strlen(cust_info[register_books].name)] = '\0';

		cin.ignore(256, '\n');

		cout << "Title: ";
		cin.getline(book_info[register_books].title, VAR_TITLE);

		cout << "Author: ";
		cin.getline(book_info[register_books].author, VAR_NAME);

		do {
			cout << "Year of publication: ";
			cin >> year;
		} while (!int_check(1700, 2100, year));

		book_info[register_books].year = year;

		register_books++;
	}
}

//Return Book location in book_info array with title or ISBN
int locate_book(char isbn[]) {
	for (int r = 0; r < register_books; r++) {
		if (strcmp(isbn, book_info[r].isbn) == 0 || strcmp(isbn, book_info[r].title) == 0) {
			return r;
		}
	}
	return -1;
}

//Locates a book with ither ISBN or title, Returns the location in the array
int find_book() {
	char isbn[VAR_ISBN + VAR_TITLE];
	int book_location;
	
	cin.ignore(256, '\n');

	cout << "Type either ISBN or Title" << endl;
	cout << "Search: ";
	cin.getline(isbn, VAR_TITLE + VAR_ISBN);

	book_location = locate_book(isbn);

	if (book_location == -1) {
		cout << endl << "\aThe book with ISBN or Title \"" << isbn << "\" does not exist in the database." << endl;
	}

	return book_location;
}

//Return customer location in cust_info array with ID Number
int locate_customer(char id_num[]) {
	for (int r = 0; r < register_cust; r++) {
		if (strcmp(id_num, cust_info[r].id_number) == 0) {
			return r;
		}
	}

	return -1;
}

//Locates a customer with ID Number, Returns the location in the array
int find_customer() {
	bool check = false;
	char id_num[VAR_IDNUMBER];
	int person_location;

	do {
		check = false;
		cin.ignore(256, '\n');

		cout << "Customer ID: ";
		cin >> id_num;

		person_location = locate_customer(id_num);

		if (person_location == -1) {
			cout <<  endl << "\aA customer with ID \"" << id_num << "\" does not exist in the database." << endl;
		}
	} while (person_location == -1);

	cout << "Customer Name: " << cust_info[person_location].name << endl;

	return person_location;
}

//Reserve a book to a customer
void reserve_book(int person_location, int book_location) {
	int bookCust_location;

	bookCust_location = find_bookCust(person_location, book_location);

	if (bookCust_location != -1) {
		if (cust_info[person_location].books[bookCust_location].how) {
			cout << endl << cust_info[person_location].name << "\a has already borrowed that book, and cannot reserve it." << endl;
			return;
		}
		else if (!cust_info[person_location].books[bookCust_location].how) {
			cout << endl << cust_info[person_location].name << "\a has already reserved that book." << endl;
			return;
		}
	}

	if (cust_info[person_location].loan_books == BOOK_CUSTOMER) {
		cout << endl << cust_info[person_location].name << "\a can't reserve any more books" << endl;
	}
	else {
		for (int c = 0; book_info[book_location].isbn[c] != '\0'; c++) {
			cust_info[person_location].books[cust_info[person_location].loan_books].isbn[c] = book_info[book_location].isbn[c];
		}

		cust_info[person_location].books[cust_info[person_location].loan_books].isbn[strlen(book_info[book_location].isbn)] = '\0';

		cust_info[person_location].books[cust_info[person_location].loan_books].how = false;
		cust_info[person_location].loan_books++;

		cout << endl << cust_info[person_location].name << " has successfully reserved the book" << endl;
	}
}

//Input date and validate
void input_dateValitade(int& day, int& month, int& year) {
	cin.ignore(256, '\n');
	bool check;

	do {
		check = false;

		do {
			cout << "Day: ";
			cin >> day;
		} while (!int_check(1, 31, day));

		do {
			cout << "Month: ";
			cin >> month;
		} while (!int_check(1, 12, month));

		if (month % 2 == 0) {
			if (day == 31) {
				cout << "\aThe " << month << " month doesn't have 31 days." << endl << endl;
				check = true;
			}
			else if (month == 2 && day > 29) {
				cout << "\aThe " << month << " month doesn't have more than 29 days." << endl << endl;
				check = true;
			}
		}
	} while (check);

	do {
		cout << "Year: ";
		cin >> year;
	} while (!int_check(2020, 2100, year));
}

//Lend a book to a customer
void lend_book(int person_location, int book_location) {
	if (cust_info[person_location].loan_books == BOOK_CUSTOMER) {
		cout << cust_info[person_location].name << "\a can't loan any more books" << endl << endl;
	}
	else {
		cout << "Enter due date" << endl;
		input_dateValitade(cust_info[person_location].books[cust_info[person_location].loan_books].due_date.day, cust_info[person_location].books[cust_info[person_location].loan_books].due_date.month, cust_info[person_location].books[cust_info[person_location].loan_books].due_date.year);

		cust_info[person_location].books[cust_info[person_location].loan_books].how = true;

		//Copy book ISBN to customer
		for (int c = 0; book_info[book_location].isbn[c] != '\0'; c++) {
			cust_info[person_location].books[cust_info[person_location].loan_books].isbn[c] = book_info[book_location].isbn[c];
		}

		cust_info[person_location].books[cust_info[person_location].loan_books].isbn[strlen(book_info[book_location].isbn)] = '\0';
		cust_info[person_location].loan_books++;
		//Copy customers ID to book
		for (int c = 0; cust_info[person_location].id_number[c] != '\0'; c++) {
			book_info[book_location].borrower[c] = cust_info[person_location].id_number[c];
		}

		book_info[book_location].borrower[strlen(book_info[book_location].borrower)] = '\0';
		book_info[book_location].onLoan = true;

		cout << endl << cust_info[person_location].name << " has successfully borrowed the book \"" << book_info[book_location].title << "\"." << endl;
	}
}

//Main function for lend and reserve functions
void borrow_Main() {
	int book_location, person_location;
	bool check = false, first_loop = true;

	do {
		system("cls");
		header(TITLE);
		header("Borrow Books");

		if (register_cust == 0) {
			cout << "\aNo register customers found in the system!" << endl;
			system("pause");
			return;
		}
		else if (register_books == 0) {
			cout << "\aNo register books found in the system!" << endl;
			system("pause");
			return;
		}

		book_location = find_book();

		if (book_location != -1) {
			if (!book_info[book_location].onLoan) {
				cout << endl << "The Book with title \"" << book_info[book_location].title << "\" is available to borrow/reserve." << endl;
			}
			else {
				cout << endl << "The Book with title \"" << book_info[book_location].title << "\" can only be reserved." << endl;
			}

			if (question("Would the customer like to proceed?")) {
				//Find customer, Execute only in first loop
				if (first_loop) {
					person_location = find_customer();
					first_loop = false;
				}

				if (!book_info[book_location].onLoan) {
					lend_book(person_location, book_location);
				}
				else {
					reserve_book(person_location, book_location);
				}
			}
		}
	} while (question("Would the customer like to borrow another book?"));
}

//Main function for reserve function
void reserve_Main() {
	int book_location, person_location;
	bool first_loop = true;

	do {
		system("cls");
		header(TITLE);
		header("Reserve Books");

		if (register_cust == 0) {
			cout << "\aNo register customers found in the system!" << endl;
			system("pause");
			return;
		}
		else if (register_books == 0) {
			cout << "\aNo register books found in the system!" << endl;
			system("pause");
			return;
		}

		book_location = find_book();

		if (book_location != -1) {
			if (!book_info[book_location].onLoan) {
				cout << endl << "The Book with title \"" << book_info[book_location].title << "\" is available to borrow/reserve." << endl;
			}
			else {
				cout << endl << "The Book with title \"" << book_info[book_location].title << "\" can only be reserved." << endl;
			}

			if (question("Would the customer like to proceed with the reservation?")) {
				//Find customer, Execute only in first loop
				if (first_loop) {
					person_location = find_customer();
					first_loop = false;
				}

				reserve_book(person_location, book_location);
			}
		}
	} while (question("Would the customer like to reserve another book?"));
}

//Returns the location of book in loan/reserve-Customer array
int find_bookCust(int person_location, int book_location) {
	for (int c = 0; c < cust_info[person_location].loan_books; c++) {
		if (strcmp(cust_info[person_location].books[c].isbn, book_info[book_location].isbn) == 0) {
			return c;
		}
	}

	return -1;
}

//Removes book from customer array
void shift_left(int person_location, int book_location, int book_locationCust) {
	for (int c = book_locationCust; c <= cust_info[person_location].loan_books; c++) {
		for (int r = 0; cust_info[person_location].books[book_location].isbn[r] != '\0'; r++) {
			cust_info[person_location].books[c].isbn[r] = cust_info[person_location].books[c + 1].isbn[r];
		}

		cust_info[person_location].books[c].isbn[strlen(cust_info[person_location].books[c + 1].isbn)] = '\0';

		cust_info[person_location].books[c].due_date.day = cust_info[person_location].books[c + 1].due_date.day;
		cust_info[person_location].books[c].due_date.month = cust_info[person_location].books[c + 1].due_date.month;
		cust_info[person_location].books[c].due_date.year = cust_info[person_location].books[c + 1].due_date.year;
		cust_info[person_location].books[c].how = cust_info[person_location].books[c + 1].how;
	}
}

//Return book from customer
void return_book() {
	int book_location, person_location, book_locationCust;
	bool first_loop = true;

	do {
		system("cls");
		header(TITLE);
		header("Return Books");

		if (register_cust == 0) {
			cout << "\aNo register customers found in the system!" << endl;
			system("pause");
			return;
		}
		else if (register_books == 0) {
			cout << "\aNo register books found in the system!" << endl;
			system("pause");
			return;
		}

		if (first_loop) {
			person_location = find_customer();
			first_loop = false;
		}

		book_location = find_book();

		if (book_location != -1) {
			cout << endl << "Book Title: " << book_info[book_location].title << endl;

			book_locationCust = find_bookCust(person_location, book_location);

			if (book_locationCust == -1) {
				cout << endl << cust_info[person_location].name << "\a does not have the book " << book_info[book_location].title << " reserved/loaned." << endl;
			}
			else if (cust_info[person_location].books[book_locationCust].how) {
				shift_left(person_location, book_location, book_locationCust);
				cust_info[person_location].loan_books--;
				book_info[book_location].onLoan = false;

				date current_date;

				cout << endl << "Enter current date" << endl;
				input_dateValitade(current_date.day, current_date.month, current_date.year);

				int days = find_overdueDateBook(current_date, person_location, book_locationCust);

				if (days > 0) {
					cout << "The book is overdue by " << days << " days and the customer is charged an extra " << fixed << setprecision(2) << days * BOOK_LATEAMOUNT << "€";
				}

				cout << endl << "Book successfully removed from loaned" << endl;
			}
			else if (!cust_info[person_location].books[book_locationCust].how) {
				shift_left(person_location, book_location, book_locationCust);
				cust_info[person_location].loan_books--;

				cout << endl << "Book successfully removed from reserved" << endl;
			}
		}
	} while (question("Would the customer like to return another book?"));
}

//Header for details of customer
void print_custInfo_Header() {
	cout << left << setw(VAR_IDNUMBER + WORD_DISPLAYGAP) << "ID Number" << setw(VAR_NAME + WORD_DISPLAYGAP);
	cout << "Name" << setw(VAR_ADDRESS + WORD_DISPLAYGAP) << "Address" << endl;
}

//Print Details of Customer
void print_custInfo(int person_location) {
	cout << left << setw(VAR_IDNUMBER + WORD_DISPLAYGAP) << cust_info[person_location].id_number << setw(VAR_NAME + WORD_DISPLAYGAP);
	cout << cust_info[person_location].name << setw(VAR_ADDRESS + WORD_DISPLAYGAP) << cust_info[person_location].addreess << endl;
}

//Main for prints the customer details
void print_custDetails() {
	int person_location;

	system("cls");
	header(TITLE);
	header("Details of Customer");

	if (register_cust == 0) {
		cout << "\aNo register customers found in the system!" << endl;
		return;
	}
	else if (register_books == 0) {
		cout << "\aNo register books found in the system!" << endl;
		return;
	}

	person_location = find_customer();

	system("cls");
	header(TITLE);
	header("Details of Customer");

	print_custInfo_Header();
	print_custInfo(person_location);

	if (cust_info[person_location].loan_books > 0) {
		print_bookInfo_Header();

		for (int c = 0; c < cust_info[person_location].loan_books; c++) {
			print_bookInfo(person_location, locate_book(cust_info[person_location].books[c].isbn), c);
		}
	}
	cout << endl;
}

//Returns the number of loan books of a customer
int find_CustLoan(int person_location) {
	int loan_book = 0;

	for (int c = 0; c < cust_info[person_location].loan_books; c++) {
		if (cust_info[person_location].books[c].how) {
			loan_book++;
		}
	}

	return loan_book;
}

//Print details of customer with the maximum number of books on loan
void print_custMaxBooks() {
	int max = 0;
	int customer_maxLocation = 0;

	system("cls");
	header(TITLE);
	header("Customer with the maximum number of books");

	if (register_cust == 0) {
		cout << "\aNo register customers found in the system!" << endl;
		return;
	}
	else if (register_books == 0) {
		cout << "\aNo register books found in the system!" << endl;
		return;
	}

	for (int r = 0; r < register_cust; r++) {
		if (find_CustLoan(r) > max) {
			customer_maxLocation = r;
			max = find_CustLoan(r);
		}
	}

	print_custInfo_Header();
	print_custInfo(customer_maxLocation);
	print_bookInfo_Header();

	for (int c = 0; c < cust_info[customer_maxLocation].loan_books; c++) {
		print_bookInfo(customer_maxLocation, locate_book(cust_info[customer_maxLocation].books[c].isbn), c);
	}

	cout << setw(BOOK_DISPLAY) << "" << "Number of Books: " << max << endl;
}

//Header of bookinfo
void print_bookInfo_Header() {
	cout << endl << right << setw(BOOK_DISPLAY) << "";

	SetConsoleTextAttribute(color, 112);

	cout << left << setw(VAR_TITLE + WORD_DISPLAYGAP) << "Title" << setw(VAR_NAME + WORD_DISPLAYGAP) << "Author";
	cout << setw(8 + WORD_DISPLAYGAP / 2) << "Status";
	cout << setw(LINE_SIZE - VAR_TITLE - VAR_NAME - 8 - WORD_DISPLAYGAP * 2 - WORD_DISPLAYGAP / 2 - BOOK_DISPLAY) << "Due Date" << endl;

	SetConsoleTextAttribute(color, 7);
	line_book();
}

//Prints Details of book
void print_bookInfo(int person_location, int book_location, int book_locationCust) {
	cout << right << setw(BOOK_DISPLAY) << "";

	SetConsoleTextAttribute(color, 112);

	cout << left << setw(VAR_TITLE + WORD_DISPLAYGAP) << book_info[book_location].title << setw(VAR_NAME + WORD_DISPLAYGAP) << book_info[book_location].author;
	cout << setw(8 + WORD_DISPLAYGAP / 2) << (cust_info[person_location].books[book_locationCust].how ? "Loaned" : "Reserved");

	if (cust_info[person_location].books[book_locationCust].how) {
		cout << right << setw(2) << cust_info[person_location].books[book_locationCust].due_date.day << "/";
		cout << right << setw(2) << cust_info[person_location].books[book_locationCust].due_date.month << "/";
		cout << right << setw(4) << cust_info[person_location].books[book_locationCust].due_date.year;
		cout << setw(LINE_SIZE - VAR_TITLE - VAR_NAME - 8 - WORD_DISPLAYGAP * 2 - WORD_DISPLAYGAP / 2 - BOOK_DISPLAY - 10) << "" << endl;
	}
	else{
		cout << setw(LINE_SIZE - VAR_TITLE - VAR_NAME - 8 - WORD_DISPLAYGAP * 2 - WORD_DISPLAYGAP / 2 - BOOK_DISPLAY) << "";
	}

	SetConsoleTextAttribute(color, 7);
}

//Print details of customers with more than predefined warning number of books
void print_custMoreBooks() {
	system("cls");
	header(TITLE);
	header("Customers with more than 3 books on loan");

	if (register_cust == 0) {
		cout << "\aNo register customers found in the system!" << endl;
		return;
	}
	else if (register_books == 0) {
		cout << "\aNo register books found in the system!" << endl;
		return;
	}

	print_custInfo_Header();

	for (int r = 0; r < register_cust; r++) {
		if (find_CustLoan(r) > BOOK_WARNING) {
			print_custInfo(r);
			print_bookInfo_Header();
			
			for (int c = 0; c < cust_info[r].loan_books; c++) {
				if (cust_info[r].books[c].how) {
					print_bookInfo(r,locate_book(cust_info[r].books[c].isbn),c);
				}
			}
		}
	}
}

//Return True if the customer has a reserved book
bool customer_Reserve(int person_location) {
	for (int c = 0; c < cust_info[person_location].loan_books; c++) {
		if (!cust_info[person_location].books[c].how) {
			return true;
		}
	}
	return false;
}

//Prints all books that are on reserve
void print_bookOnReserve() {
	system("cls");
	header(TITLE);
	header("Books on Reserve");

	if (register_cust == 0) {
		cout << "\aNo register customers found in the system!" << endl;
		return;
	}
	else if (register_books == 0) {
		cout << "\aNo register books found in the system!" << endl;
		return;
	}

	print_custInfo_Header();

	for (int r = 0; r < register_cust; r++) {
		if (customer_Reserve(r)) {
			print_custInfo(r);
			print_bookInfo_Header();

			for (int c = 0; c < cust_info[r].loan_books; c++) {
				if (!cust_info[r].books[c].how) {
					print_bookInfo(r, locate_book(cust_info[r].books[c].isbn), c);
				}
			}

			cout << endl;
		}
	}
}

//Return the difference between current date and date of cust_info[person_location] of a specific book
long find_overdueDateBook(date current_date, int person_location, int book_locationCust) {
	if (cust_info[person_location].books[book_locationCust].how) {
		long days = 0, num;

		num = current_date.year - cust_info[person_location].books[book_locationCust].due_date.year;
		days += num * 365;

		if (cust_info[person_location].books[book_locationCust].due_date.month % 2 == 0) {
			if (cust_info[person_location].books[book_locationCust].due_date.month == 2) {
				num = current_date.month - cust_info[person_location].books[book_locationCust].due_date.month;
				days += num * 29;
			}
			else {
				num = current_date.month - cust_info[person_location].books[book_locationCust].due_date.month;
				days += num * 30;
			}
		}
		else {
			num = current_date.month - cust_info[person_location].books[book_locationCust].due_date.month;
			days += num * 31;
		}

		days += current_date.day - cust_info[person_location].books[book_locationCust].due_date.day;
		
		return days;
	}
	else {
		return -1;
	}
}

//Return the difference between current date and date of cust_info[person_location].books[].date
long find_overdueDate(date current_date, int person_location) {
	long days = 0, num;

	for (int r = 0; r < cust_info[person_location].loan_books; r++) {
		num = find_overdueDateBook(current_date, person_location, r);

		if (num > 0) {
			days += num;
		}
	}

	return days;
}

//Prints all overdue books
void print_overdueBooks() {
	date current_date;
	long days;

	system("cls");
	header(TITLE);
	header("Overdue Books");

	if (register_cust == 0) {
		cout << "\aNo register customers found in the system!" << endl;
		return;
	}
	else if (register_books == 0) {
		cout << "\aNo register books found in the system!" << endl;
		return;
	}

	cout << "Enter current date" << endl;
	input_dateValitade(current_date.day, current_date.month, current_date.year);

	system("cls");
	header(TITLE);
	header("Overdue Books");
	print_custInfo_Header();

	for (int r = 0; r < register_cust; r++) {
		days = find_overdueDate(current_date, r);

		if (days > 0) {
			print_custInfo(r);
			print_bookInfo_Header();

			for (int c = 0; c < cust_info[r].loan_books; c++) {
				if (find_overdueDateBook(current_date, r, c) > 0) {
					print_bookInfo(r, locate_book(cust_info[r].books[c].isbn), c);
				}
			}

			cout << "Overdue days: " << days << endl;
			cout << "Extra fee charged: " << fixed << setprecision(2) << days * BOOK_LATEAMOUNT << "€" << endl << endl;
		}
	}
}

//Function redirects to appropriate function
void menu_select(int choice) {
	switch(choice){
	case 1:
		do {
			new_customer();
			if (register_books > 0 && question("Would the qustomer like to borrow a book?")) {
				borrow_Main();
			}
		} while (question("Do you want to add another customer?"));
		break;
	case 2:
		do {
			new_book();
		} while (question("Do you want to add another book?"));
		break;
	case 3:
		borrow_Main();
		break;
	case 4:
		reserve_Main();
		break;
	case 5:
		return_book();
		break;
	case 6:
		print_custDetails();
		system("pause");
		break;
	case 7:
		print_custMaxBooks();
		system("pause");
		break;
	case 8:
		print_custMoreBooks();
		system("pause");
		break;
	case 9:
		print_bookOnReserve();
		system("pause");
		break;
	case 10:
		print_overdueBooks();
		system("pause");
		break;
	case 11:
		break;
	}
}