// coursesMain.cpp
// School course database using pointers and linked lists.
// December 12, 2016
// Author: Joseph Schell
// Sources: None

#include "coursesHeader.h"

int main() {
	Transcript list;
	char reply;

	cout << "Welcome to the Course Transcript and GPA program." << endl;
	list.loadData();

	do {    // Welcome Screen
		cout << "What would you like to do:\n"
			<< "L: Shows a complete transcript of courses taken.\n"
			<< "A: Add a course to the list.\n"
			<< "D: Delete a course from the list.\n"
			<< "C: Calculate GPA.\n"
			<< "Q: Quit program." << endl;
		cout << "--> ";

		reply = readChoice();
		list.exeChoice(reply);
	} while (toupper(reply) != 'Q');

	system("pause");
	return 0;
}

// Reads user input into a char variable.
char readChoice() {
	char choice;

	cin >> choice;
	cin.ignore(100, '\n');
	return choice;
}

// Verifies user inputs c-string.
void readString(char inputStr[], const char prompt[]) {
	cout << prompt;
	cin.getline(inputStr, strSize, '\n');
	while (!cin) {
		cin.clear();
		cin.ignore(100, '\n');
	}
}

// Deletes dynamic memory data.
void deleteCoursePointer(Course * ptr) {
	if (ptr != nullptr) {
		delete[] ptr->name;
		delete[] ptr->description;
		delete[] ptr->term;
		delete[] ptr->grade;
		delete ptr;
	}
}
