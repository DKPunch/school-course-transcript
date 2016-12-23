// gpaImpFile.cpp 
// December 12, 2016
// Author: Joseph Schell
// Sources: None

#include <string>
#include "coursesHeader.h"

// Default Constructor
// sets count to 0
// sets head to null
Transcript::Transcript() {
	count = 0;
	head = nullptr;
}

// Destructor
// Ensures deletion of dynamic memory.
Transcript::~Transcript() {
	Course * ptr;
	Course * tmp;
	ptr = head;
	while (ptr != nullptr) {
		tmp = ptr->next;
		deleteCoursePointer(ptr);
		ptr = nullptr;
		ptr = tmp;
	}

	if (fileStr != nullptr) {
		delete fileStr;
		fileStr = nullptr;
	}
}

// Reads data from text file that was input
// by the user into pointer arrays.
int Transcript::loadData() {
	fstream inputFile;
	char tempFileName[strSize], tempStr[strSize];
	char lastChar;
	long long charCount;

	cout << "What file would you like to use for input and output? ";
	cin.getline(tempFileName, strSize);

	int size = strlen(tempFileName); // added for set size to tempName
	fileStr = new char[size + 1]; // +1 for NULL
	strncpy(fileStr, tempFileName, size); // (destination, source, num)
	fileStr[size] = '\0'; // Explicit NULL termination
	inputFile.open(fileStr, ios::in);

	while (!inputFile.is_open()) { // Invalid File Error Check
		cout << "File does not exist. Creating a new file for data...\n" << endl;
		inputFile.open(fileStr, ios::app); // Creates new file
		inputFile.close();

		cout << "Successfully created new file.\n" << endl;
		return 0;
	}
	inputFile.close();

	inputFile.open(fileStr, ios::in | ios::out);
	while (!inputFile.eof()) {
		Course * ptr = new Course;
		ptr->next = nullptr;

		inputFile.getline(tempStr, strSize, ',');
		ptr->name = new char[strlen(tempStr) + 1]; // + 1 for null.
		strncpy(ptr->name, tempStr, strlen(tempStr) + 1);
		inputFile.getline(tempStr, strSize, ',');
		ptr->description = new char[strlen(tempStr) + 1]; // + 1 for null.
		strncpy(ptr->description, tempStr, strlen(tempStr) + 1);
		inputFile.getline(tempStr, strSize, ',');
		ptr->term = new char[strlen(tempStr) + 1]; // + 1 for null.
		strncpy(ptr->term, tempStr, strlen(tempStr) + 1);
		inputFile.getline(tempStr, strSize, '\n');
		ptr->grade = new char[strlen(tempStr) + 1]; // + 1 for null.
		strncpy(ptr->grade, tempStr, strlen(tempStr) + 1);

		addCourseSeq(ptr);
	}
	// read end of file character
	inputFile.getline(tempStr, strSize, '\n');

	// Check if last char in the file is \n. If not, then adds \n.
	inputFile.seekg(0, ios::end);
	charCount = inputFile.tellg();
	if (charCount > 0) {
		inputFile.seekg(-1, ios::end);
		lastChar = inputFile.peek();
		if (lastChar != '\n')
			inputFile.write("\n", 1);
	}
	inputFile.close();

	cout << "Successfully loaded " << count << " courses..." << endl;

	displayData();
	return 0;
}

// Takes reply by user and executes the matching method.
void Transcript::exeChoice(char reply) {
	switch (tolower(reply)) {
	case 'l':
		displayData();
		break;
	case 'a':
		addCourse();
		break;
	case 'd':
		deleteCourse();
		break;
	case 'c':
		calcGPA();
		break;
	case 'q':
		writeData();
		break;
	default:
		cout << "Error, invalid input.\n" << endl;
		break;
	}
}

// Displays all data from the database onto the screen.
void Transcript::displayData() {
	cout << endl << left << setw(nameWidth) << "Course Name"
		<< left << setw(descWidth) << "Description"
		<< left << setw(termWidth) << "Term"
		<< left << setw(gradeWidth) << "Grade" << endl;
	cout << "------------------------------------"
		<< "------------------------------------" << endl;

	Course * p = head;

	while (p != nullptr) {
		cout << left << setw(nameWidth) << p->name
			<< left << setw(descWidth) << p->description
			<< left << setw(termWidth) << p->term
			<< "  " << setfill(' ') << setw(gradeWidth) << p->grade << endl;
		p = p->next;
	}

	cout << "\n";
}

// Takes user input and adds this data as a
// new course to the database.
void Transcript::addCourse() {
	char tempStr[strSize];
	Course * ptr = new Course;
	ptr->next = nullptr;

	readString(tempStr, "What is the name of this course? ");
	ptr->name = new char[strlen(tempStr) + 1];
	strncpy(ptr->name, tempStr, strlen(tempStr) + 1);
	readString(tempStr, "What is the course description? ");
	ptr->description = new char[strlen(tempStr) + 1];
	strncpy(ptr->description, tempStr, strlen(tempStr) + 1);
	readString(tempStr, "Which term did you take this course? ");
	ptr->term = new char[strlen(tempStr) + 1];
	strncpy(ptr->term, tempStr, strlen(tempStr) + 1);
	readString(tempStr, "What grade did you receive for this course? ");
	ptr->grade = new char[strlen(tempStr) + 1];
	strncpy(ptr->grade, tempStr, strlen(tempStr) + 1);


	cout << "\nSuccessfully added " << ptr->name << " to the course list.\n" << endl;
	addCourseSeq(ptr);
}

// Takes user input and looks matching course
// to remove from the database.
void Transcript::deleteCourse() {
	int removedNum = 0;
	char tempStr[strSize];
	Course * ptr;

	readString(tempStr, "What is the name of the course you would like to delete? ");

	ptr = findCourse(tempStr);
	// remove all courses that have the same name
	while (ptr != nullptr) {
		deleteCourseSeq(ptr);
		ptr = findCourse(tempStr);
		removedNum++;
	}

	if (removedNum > 0) {
		cout << removedNum
			<< " course has been removed from the list. \n";
	}
	else {
		cout << "Course not found. Nothing has been removed from the list. \n";
	}
	cout << "\n";

}


// Calculates Grade Point Average of all
// courses inside the database.
void Transcript::calcGPA() {
	char x;
	double points = 0;
	double gpa = 0;

	cout << fixed << showpoint << setprecision(3);

	Course * p = head;

	while (p != nullptr) {

		x = *(p->grade);
		switch (toupper(x)) {
		case 'A':
			points = 4.00;
			break;
		case 'B':
			points = 3.00;
			break;
		case 'C':
			points = 2.00;
			break;
		default:
			points = 0.00;
			break;
		}
		gpa += points;
		p = p->next;
	}
	gpa /= count;
	cout << "\nYour current GPA after completing "
		<< count << " courses is: " << gpa << ".\n" << endl;
}


// Writes all data to the original text file.
void Transcript::writeData() {
	ofstream outFile;
	outFile.open(fileStr, ios::in | ios::out);
	if (!outFile) {
		cout << "cannot open file to write new data!";
		exit(0);
	}

	if (count <= 0) {
		outFile.close();
		cout << "No data left to write to file, will not save changes!!\n";
		cout << "Terminating program..." << endl;
		system("pause");
		exit(0);
	}

	Course * p = head;

	while (p != nullptr & p->next != nullptr) {
		outFile << p->name << ',' << p->description << ','
			<< p->term << ',' << p->grade << '\n';
		p = p->next;
	}

	if (p != nullptr) {
		outFile << p->name << ',' << p->description << ','
			<< p->term << ',' << p->grade;
	}

	outFile.close();
	cout << "Successfully wrote course data to the course report file." << endl;
	cout << "Terminating program..." << endl;
}

// Tries to find a matching course name
// inside of the database.
Course * Transcript::findCourse(char * name) {

	Course * p = head;
	while (p != nullptr) {
		if (strcmp(name, p->name) == 0) {
			return p;
		}
		p = p->next;
	}
	// if nothing is found
	return nullptr;
}

// Adds a new course to the database in alphabetical order.
void Transcript::addCourseSeq(Course * ptr) {
	if (ptr != nullptr) {
		count++;
		if (head == nullptr) {
			head = ptr;
		}
		else {
			if (strcmp(head->name, ptr->name) >= 0) {
				ptr->next = head;
				head = ptr;
			}
			else {
				Course * p = head;
				while (p->next != nullptr
					&& strcmp(p->next->name, ptr->name) <= 0) {
					p = p->next;
				}

				ptr->next = p->next;
				p->next = ptr;
			}
		}
	}
}

// Deletes a course from the database.
void Transcript::deleteCourseSeq(Course * ptr) {
	if (ptr == nullptr || head == nullptr) {
		return;
	}
	if (ptr == head) {
		head = ptr->next;
		count--;
		deleteCoursePointer(ptr);
		ptr = nullptr;
	}
	else {
		Course * p = head;
		while (p->next != nullptr
			&& p->next != ptr) {
			p = p->next;
		}
		if (p->next == ptr) {
			p->next = ptr->next;
			count--;
			deleteCoursePointer(ptr);
			ptr = nullptr;
		}
	}
}