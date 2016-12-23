// coursesHeader.h
// December 12, 2016
// Author: Joseph Schell
// Sources: None

#ifndef _COURSES_
#define _COURSES_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

using namespace std;

// Global Constants
const int strSize = 200;
const int nameWidth = 16;
const int descWidth = 30;
const int termWidth = 18;
const int gradeWidth = 12;

struct Course {
	char * name;
	char * description;
	char * term;
	char * grade;
	Course * next;
};

class Transcript {
public:
	Transcript(); // Default Constructor
	~Transcript(); // Destructor
	char * fileStr; // Array to store input file name.
	int loadData();
	void exeChoice(char reply);
	void displayData();
	void addCourse();
	void deleteCourse();
	void calcGPA();
	void writeData();
private:
	Course * findCourse(char * name);
	void addCourseSeq(Course * ptr);
	void deleteCourseSeq(Course * ptr);
	Course * head;
	int count;
};

// Standard Function Prototypes
char readChoice();
void readString(char inputStr[], const char prompt[]);
void deleteCoursePointer(Course * ptr);

#endif
