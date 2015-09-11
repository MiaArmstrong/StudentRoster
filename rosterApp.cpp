#include <iostream>
#include <fstream>
using namespace std;

void pause();

int getInt();
float getFloat();
void getString(char str[], int capacity);

enum Year { FRESHMAN, SOPHOMORE, JUNIOR, SENIOR };
Year readYear();
void printYear(Year aYear);

const int MAX_CHAR = 101;
const int CAPACITY = 10;

struct Student
{
	char	ID[MAX_CHAR];
	float	gpa;
	Year	year;
};

void printStudent(const Student& aStudent);
void readStudent(Student& aStudent);
void readID(char id[]);

void displayMenu();
char readInCommand();
void executeCommand(char command, Student list[], int& size);


void addToDB(const Student& aStudent, Student list[], int& size);
void listAll(const Student list[], int size);
bool search(const char key[], const Student list[], int size, Student& match);
void saveToFile(const char fileName[], const Student list[], int size);
void loadFromFile(const char fileName[], Student list[], int& size);

int main()
{
	char		command;
	Student		studentList[CAPACITY];
	int			size = 0;
	char		fileName[] = "data.txt";

	loadFromFile(fileName, studentList, size);

	displayMenu();
	command = readInCommand();
	while (command != 'q')
	{
		executeCommand(command, studentList, size);

		displayMenu();
		command = readInCommand();
	}

	cout << endl << "Thank you for using student roster!" << endl << endl;
	saveToFile(fileName, studentList, size);

	pause();
	return 0;
}

void loadFromFile(const char fileName[], Student list[], int& size)
{
	ifstream in;
	in.open(fileName);
	if(!in)
	{
		cerr << "Fail to open " << fileName << " for reading!" << endl;
		exit(1);
	}

	Student		aStudent;
	char		id[MAX_CHAR];
	float		gpa;
	Year		year;
	int			temp;

	in.get(id, MAX_CHAR, ';');
	while(!in.eof())
	{
		in.ignore(100, ';');
		in >> gpa;
		in.ignore(100, ';');
		in >> temp;
		year = static_cast<Year>(temp);
		in.ignore(100, '\n');

		aStudent.gpa = gpa;
		strcpy(aStudent.ID, id);
		aStudent.year = year;

		addToDB(aStudent, list, size);

		in.get(id, MAX_CHAR, ';');
	}
	in.close();

}

void saveToFile(const char fileName[], const Student list[], int size)
{
	ofstream out;

	out.open(fileName);
	if(!out)
	{
		cerr << "Fail to open " << fileName << " for writing!" << endl;
		exit(1);
	}
	
	int index;
	for(index=0; index<size; index++)
	{
		out << list[index].ID << ';' << list[index].gpa << ';' << list[index].year 
			<< endl;
	}
	out.close();
}
void executeCommand(char command, Student list[], int& size)
{
	Student aStudent;
	char id[MAX_CHAR];

	switch (command)
	{
	case 'a': 
		readStudent(aStudent);
		addToDB(aStudent, list, size);
		break;
	case 'l': 
		listAll(list, size);
		break;
	case 's': 
		cout << "Please enter the student ID you are searching for: ";
		readID(id);
		if(search(id, list, size, aStudent))
		{
			cout << "Match: ";
			printStudent(aStudent);
			cout << endl << endl;
		}
		else
		{
			cout << "No match found!" << endl << endl;
		}
		break;
	case 'r': cout << "remove is invoked" << endl;
		break;
	default:
		cout << "illegal command!" << endl;
		break;
	}
}

bool search(const char key[], const Student list[], int size, Student& match)
{
	int index;
	for(index=0; index<size; index++)
	{
		if(strcmp(list[index].ID, key) == 0)
		{
			match = list[index];
			break;
		}
	}
	if(index == size)
		return false;
	else
		return true;
}

void addToDB(const Student& aStudent, Student list[], int& size)
{
	list[size] = aStudent; //be careful about '=', it will crash later
	size++;
}

void listAll(const Student list[], int size)
{
	int index;
	for(index=0; index<size; index++)
	{
		printStudent(list[index]);
		cout << endl;
	}
}
/*
This function returns a lowercase char for a command.
return: a lower case char
*/
char readInCommand()
{
	char command;

	cout << "Please enter your command: (a, l, ..) ";
	cin >> command;
	cin.ignore(100, '\n');

	return tolower(command);
}

void displayMenu()
{
	cout << endl << "Welcome to student roster!" << endl
		<< "\ta. add a stduent" << endl
		<< "\tl. list all students" << endl
		<< "\tr. remove a student" << endl
		<< "\ts. search a student" << endl
		<< "\tq. quit" << endl << endl;
}

void printStudent(const Student& aStudent)
{
	cout << aStudent.ID << '\t' << aStudent.gpa << '\t';
	printYear(aStudent.year);
}

void readStudent(Student& aStudent)
{
	cout << "Please enter student id: ";
	readID(aStudent.ID);

	cout << "Please enter student gpa: ";
	aStudent.gpa = getFloat();

	cout << "Please enter student year: (0 for freshman...) ";
	aStudent.year = readYear();
}

void readID(char id[])
{
	getString(id, MAX_CHAR);
}
/* This function reads in a Year value.
return: a Year value
*/
Year readYear()
{
	int temp = getInt();
	while (temp < FRESHMAN || temp > SENIOR)
	{
		cout << "Illegal Year! Try again: ";
		temp = getInt();
	}
	return static_cast <Year> (temp);
}

/* This function prints a Year value.
in: a Year
*/
void printYear(Year aYear)
{
	char yearString[MAX_CHAR];
	switch(aYear)
	{
	case FRESHMAN: 
		strcpy(yearString, "freshman");
		break;
	case SOPHOMORE:
		strcpy(yearString, "sophomore");
		break;
	case JUNIOR:
		strcpy(yearString, "junior");
		break;
	case SENIOR:
		strcpy(yearString, "senior");
		break;
	}
	cout << yearString;
}

void pause()
{
	char ch;
	cout << "Please press any key to continue ..." << endl;
	cin >> ch;
	cin.ignore(100, '\n');
}

/*
This function reads in a c-string from standard input.
in/out: str
return: none
*/
void getString(char str[], int capacity)
{
	cin.get(str, capacity, '\n');
	while(!cin)
	{
		cin.clear();
		cin.ignore(100, '\n');
		cout << "Please enter a string: ";
		cin.get(str, capacity, '\n');
	}
	cin.ignore(100, '\n');		
}
/*
This function reads in an integer from standard input.
in: none
out: none
return: an integer
*/
int getInt()
{
	int value;

	cin >> value;
	while(!cin)
	{
		cin.clear();
		cin.ignore(100, '\n');
		cout << "It has to be an legal integer! Please try again: ";
		cin >> value;
	}
	cin.ignore(100, '\n');

	return value;
}

float getFloat()
{
	float value;

	cin >> value;
	while(!cin)
	{
		cin.clear();
		cin.ignore(100, '\n');
		cout << "It has to be an legal floating point value! Please try again: ";
		cin >> value;
	}
	cin.ignore(100, '\n');

	return value;
}
