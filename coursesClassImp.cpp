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
	std::fstream inputFile;
	char tempFileName[strSize], tempStr[strSize];
	
	std::cout << "What file would you like to use for input and output? ";
	std::cin.getline(tempFileName, strSize);

	int size = strlen(tempFileName); // added for set size to tempName
	fileStr = new char[size + 1]; // +1 for NULL
	strncpy(fileStr, tempFileName, size); // (destination, source, num)
	fileStr[size] = '\0'; // Explicit NULL termination
	inputFile.open(fileStr, std::ios::in);

	while (!inputFile.is_open()) { // Invalid File Error Check
		std::cout << "File does not exist. Creating a new file for data...\n" << std::endl;
		inputFile.open(fileStr, std::ios::app); // Creates new file
		inputFile.close();

		std::cout << "Successfully created new file.\n" << std::endl;
		return 0;
	}
	inputFile.close();
	inputFile.open(fileStr, std::ios::in | std::ios::out);

	while (!inputFile.eof()) {
		Course * ptr = new Course;
		ptr->next = nullptr;

		inputFile.getline(tempStr, strSize, ',');
		if (strlen(tempStr) == 0) {
			break;
		}
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
	inputFile.close();

	std::cout << "Successfully loaded " << count << " courses..." << std::endl;

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
		std::cout << "Error, invalid input.\n" << std::endl;
		break;
	}
}

// Displays all data from the database onto the screen.
void Transcript::displayData() {
	std::cout << std::endl << std::left << std::setw(nameWidth) << "Course Name"
		<< std::left << std::setw(descWidth) << "Description"
		<< std::left << std::setw(termWidth) << "Term"
		<< std::left << std::setw(gradeWidth) << "Grade" << std::endl;
	std::cout << "------------------------------------"
		<< "------------------------------------" << std::endl;

	Course * p = head;

	while (p != nullptr) {
		std::cout << std::left << std::setw(nameWidth) << p->name
			<< std::left << std::setw(descWidth) << p->description
			<< std::left << std::setw(termWidth) << p->term
			<< "  " << std::setfill(' ') << std::setw(gradeWidth) << p->grade << std::endl;
		p = p->next;
	}

	std::cout << "\n";
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


	std::cout << "\nSuccessfully added " << ptr->name << " to the course list.\n" << std::endl;
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
		std::cout << removedNum
			<< " course has been removed from the list. \n";
	}
	else {
		std::cout << "Course not found. Nothing has been removed from the list. \n";
	}
	std::cout << "\n";

}


// Calculates Grade Point Average of all
// courses inside the database.
int Transcript::calcGPA() {
	char x;
	double points = 0;
	double gpa = 0;

	std::cout << std::fixed << std::showpoint << std::setprecision(3);

	Course * p = head;

	try {
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
		if (count == 0) {
			throw count;
		}
	}
	catch (int x) {
		std::cout << "\nYou currently have " << x << " courses stored "
			<< "in the database, cannot divide by " << x << "!\n" << std::endl;
		return 0;
	}

	gpa /= count;
	std::cout << "\nYour current GPA after completing "
		<< count << " courses is: " << gpa << ".\n" << std::endl;
	return 0;
}

// Writes all data to the original text file.
void Transcript::writeData() {
	std::ofstream outFile;
	outFile.open(fileStr, std::ios::in | std::ios::out);

	try {
		if (!outFile) {
			std::cout << "cannot open file to write new data!";
			exit(0);
		}
		else if (count <= 0) {
			throw count;
		}
	}
	catch (int) {
		outFile.close();
		std::cout << "\nNo data left to write to file, "
			<< "will not save any information to this file.\n";
		std::cout << "Terminating program..." << std::endl;
		system("pause");
		exit(0);
	}
	Course * p = head;

	while (p != nullptr && p->next != nullptr) {
		outFile << p->name << ',' << p->description << ','
			<< p->term << ',' << p->grade << '\n';
		p = p->next;
	}

	if (p != nullptr) {
		outFile << p->name << ',' << p->description << ','
			<< p->term << ',' << p->grade;
	}

	outFile.close();
	std::cout << "Successfully wrote data to the course text file." << std::endl;
	std::cout << "Terminating program..." << std::endl;
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
