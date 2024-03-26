#include <iostream> // input/output
#include <fstream> // file handling
#include <string>// string functions
#include <string.h>
#include <iomanip> // text formatting
#include <type_traits> 
#include <sstream> // input type checking
#include <algorithm> // sorting
#include <vector> // match array

using namespace std;
#define SETW_LEFT(width) setw(width) << left // formatting macro, shorten repetitive syntax

void pressContinue(){   // freeze interface, improves UI/UX
	cout << "Press any key to continue...\n";
    cin.get();
	system("cls");
}

int getOption(){    // ensure valid input for int types, error handling
	string inputOption;
	int option;
	do{
		getline(cin, inputOption);
		stringstream ss(inputOption);

		if (ss >> option) {
			char remaining;
			if (ss >> remaining) {
				std::cerr << "Invalid input. Please enter only an integer.\n"
						  << "Enter again: ";
			} 
			else {
				if(option < 1){
					std::cerr << "Invalid input. Please enter only a positive integer.\n"
						  	  << "Enter again: ";
				}
				else{
					return option;
				}
			}
		} 
		else {
			std::cerr << "Invalid input. Please enter only an integer.\n"
					  << "Enter again: ";
		}
	}while(true);
}

bool confirm(){
    string confirm;
    getline(cin, confirm);
    if(confirm == "Y" || confirm == "y"){
        return true;
    }
    else if(confirm == "N" || confirm == "n"){
        return false;
    }
    else{
        cout << "Invalid input.\n";
        return false;
    }
}

struct StudentInformation{  // node structure for student records
    int nodeId; 
    int studentId, yearLevel;
    string firstName, lastName, fullName, birthday, address, gender, program;
    static int counter;

    StudentInformation* next = NULL;

    StudentInformation(){   // increment counter to ensure unique node id
        counter++;
        nodeId = counter;
    }

    void setFullName(){
        fullName = firstName + ' ' + lastName;
    }

    string printToUpper(string data){   // print to uppercase for uniformity
        string temp = data;
        for(char& c: temp){
            c = toupper(c);
        }
        return temp;
    }

    string shorten(string data, int index){     // handle event for text overflow, ensure display format is not destroyed
        string temp;
        if(data.length() > index){
            temp = data.substr(0, index - 4) + "...";
        }
        else{
            temp = data.substr(0, index);
        }
        return temp;
    }

    string shorten(int data, int index){
        string temp = to_string(data);
        if(temp.length() > index){
            temp = temp.substr(0, index - 4) + "...";
        }
        else{
            temp = temp.substr(0, index);
        }
        return temp;
    }

    void displayInfo(){     // print formatting
        cout << setw(12) << nodeId << "| " << SETW_LEFT(11) << shorten(studentId, 11) 
            << "| " << SETW_LEFT(43) << shorten(printToUpper(fullName), 43)
            << "| " << SETW_LEFT(11) << birthday.substr(0, 10) << "| " << SETW_LEFT(8) << printToUpper(gender.substr(0,1))
            << "| " << SETW_LEFT(15) << shorten(printToUpper(program), 15) << "| " << SETW_LEFT(11) << shorten(yearLevel, 1)
            << "| " << SETW_LEFT(43) << shorten(printToUpper(address), 43) << '\n';
    }

    void displayInfoLastNameFirst(){
        string temp = lastName + ' ' + firstName;
        cout << setw(12) << nodeId << "| " << SETW_LEFT(11) << studentId 
            << "| " << SETW_LEFT(43) << shorten(printToUpper(temp), 43)
            << "| " << SETW_LEFT(11) << birthday.substr(0, 10) << "| " << SETW_LEFT(8) << printToUpper(gender.substr(0,1))
            << "| " << SETW_LEFT(15) << shorten(printToUpper(program), 15) << "| " << SETW_LEFT(11) << shorten(yearLevel, 1)
            << "| " << SETW_LEFT(43) << shorten(printToUpper(address), 43) << '\n';
    }
};

int StudentInformation::counter = 0;

bool compareByLastName(const StudentInformation* a, const StudentInformation* b) {  // function for alphabetical sorting
    string tempA = a->lastName, tempB = b->lastName;
    for(char& c: tempA){
        c = tolower(c);
    }
    for(char& c: tempB){
        c = tolower(c);
    }
    return tempA < tempB;
}

class StudentDataBase{  // database class that will contain the nodes
    public:
    bool isModified = false;
    bool unsavedChanges = false;
    StudentInformation* head = NULL;

    void addStudent(StudentInformation* studentInfo){   // adding new nodes
        StudentInformation* newStudent = studentInfo;
        newStudent->setFullName();
        if(head == NULL){
            head = newStudent;
        }
        else{
            StudentInformation* temp = head;
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = newStudent;
        }
    }

    bool idExisting(int id){    // duplicate student id not allowed
        if(head == NULL){
            return false;
        }
        StudentInformation* temp = head;
        while(temp != NULL){
            if(temp->studentId == id){
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    void readDatabase(fstream& DatabaseFile){   // read data from text file database
        DatabaseFile.open("studentdatabase.txt", ios::in);
        string line;
        
        while(getline(DatabaseFile, line)){     // create new node and fill with data from text file
            if(line.find('#') != string::npos){
                StudentInformation* newStudent = new StudentInformation;
                newStudent->nodeId = stoi(line.substr(line.find('#') + 1));
                
                getline(DatabaseFile, line);
                newStudent->studentId = stoi(line);
                getline(DatabaseFile, line);
                newStudent->firstName = line;
                getline(DatabaseFile, line);
                newStudent->lastName = line;
                getline(DatabaseFile, line);
                newStudent->birthday = line;
                getline(DatabaseFile, line);
                newStudent->address = line;
                getline(DatabaseFile, line);
                newStudent->gender = line;
                getline(DatabaseFile, line);
                newStudent->program = line;
                getline(DatabaseFile, line);
                newStudent->yearLevel = stoi(line);

                addStudent(newStudent);
            }
        }

        DatabaseFile.clear();
        DatabaseFile.seekg(0, ios::beg);

        getline(DatabaseFile, line);
        if(line.find("::") != string::npos){
            StudentInformation::counter = stoi(line.substr(line.find("::") + 2));
        }
        else{
            StudentInformation::counter = 0;
        }

        DatabaseFile.close();
    }

    void displayHeader(){
        system("cls");
        cout << "Maximize window/Fullscreen for better experience.\n\n";
        cout << SETW_LEFT(12) << "Database ID" << SETW_LEFT(13) << "| Student ID" << SETW_LEFT(45) << "| Full Name" 
             << SETW_LEFT(13) << "| Birthday" << SETW_LEFT(10) << "| Gender" 
             << SETW_LEFT(17) << "| Degree Program" << SETW_LEFT(13) << "| Year Level" << SETW_LEFT(15) << "| Address" << '\n';
        cout << setfill('=') << setw(160) << "=" << setfill(' ') << '\n';
    }

    void viewRecords(){
        StudentInformation* temp = head;
        if(temp == NULL){
            cout << "Database is empty.\n";
            return;
        }

        string yes;
        cout << "Enter (y) if you want to display alphabetically. Enter any for default\n";
        getline(cin, yes);

        if(yes == "Y" || yes == "y"){
            vector<StudentInformation*> sortingArray;
            while(temp != NULL){
                sortingArray.push_back(temp);
                temp = temp->next;
            }
            sort(sortingArray.begin(), sortingArray.end(), compareByLastName);
            displayHeader();
            for(StudentInformation* student: sortingArray){
                student->displayInfoLastNameFirst();
            }
        }
        else{
            displayHeader();
            while(temp != NULL){
                temp->displayInfo();
                temp = temp->next;
            }
        }
    }

    void searchDatabase(string data, int option){   // search database depending on option provided by user
        vector<StudentInformation*> matchArray;

        if (head == nullptr) {
            cout << "Database is empty.\n";
            return;
        }

        StudentInformation* temp = head;
        string holder;
        int intData;
        for(char& c: data){
            c = tolower(c);
        }

        switch(option){
            case 1:
                intData = stoi(data);
                while(temp != NULL){
                    if(temp->studentId == intData){
                        matchArray.push_back(temp);
                    }
                    temp = temp->next;
                }
                break;
            case 2:
                while(temp != NULL){
                    holder = temp->fullName;
                    for(char& c: holder){
                        c = tolower(c);
                    }

                    if(holder == data){
                        matchArray.push_back(temp);
                    }
                    temp = temp->next;
                }
                break;
            case 3:
                while(temp != NULL){
                    holder = temp->gender;
                    for(char& c: holder){
                        c = tolower(c);
                    }

                    if(holder == data){
                        matchArray.push_back(temp);
                    }
                    temp = temp->next;
                }
                break;
            case 4:
                while(temp != NULL){
                    holder = temp->program;
                    for(char& c: holder){
                        c = tolower(c);
                    }

                    if(holder == data){
                        matchArray.push_back(temp);
                    }
                    temp = temp->next;
                }
                break;
            case 5:
                intData = stoi(data);
                while(temp != NULL){
                    if(temp->yearLevel == intData){
                        matchArray.push_back(temp);
                    }
                    temp = temp->next;
                }
                break;
            case 6:
                while(temp != NULL){
                    holder = temp->firstName;
                    for(char& c: holder){
                        c = tolower(c);
                    }

                    if(holder == data){
                        matchArray.push_back(temp);
                    }
                    temp = temp->next;
                }
                break;
            case 7:
                while(temp != NULL){
                    holder = temp->lastName;
                    for(char& c: holder){
                        c = tolower(c);
                    }

                    if(holder == data){
                        matchArray.push_back(temp);
                    }
                    temp = temp->next;
                }
                break;
            default:
                return;
        }

        if(matchArray.empty()){
            std::cout << "No match found.\n";
        }
        else{
            displayHeader();
            for(StudentInformation* student: matchArray){
                student->displayInfo();
            }
        }

        return;
    }

    void deleteRecordDatabaseId(int nodeId){
        int deleteId = nodeId;
        StudentInformation* temp = head;

        if(head->nodeId == deleteId) {
            displayHeader();
            temp->displayInfo();
            cout << "\nConfirm deletion of this record? (Y/n): ";
            if(confirm()){
                head = head->next;
			    delete temp;
			    cout << "Record deleted successfully.\n";
            }
		}
        else{
            while (temp->next != nullptr && temp->next->nodeId != deleteId) {
                temp = temp->next;
            }

            if (temp->next == nullptr) {
                cout << "No record exists.\n";
                return;
            }
            
            displayHeader();
            temp->displayInfo();
            cout << "\nConfirm deletion of this record? (Y/n): ";
            if(confirm()){
                StudentInformation* deleteMatch = temp->next;
                temp->next = temp->next->next;
                delete deleteMatch;
                cout << "Record deleted successfully.\n";
            }
        }
    }

    void deleteRecordStudentId(int studentId){
        int deleteId = studentId;
        StudentInformation* temp = head;

        if(head->studentId == deleteId) {
            displayHeader();
            temp->displayInfo();
            cout << "\nConfirm deletion of this record? (Y/n): ";
            if(confirm()){
                head = head->next;
                delete temp;
                
                cout << "Record deleted successfully.\n";
            }
		}
        else{
            while (temp->next != nullptr && temp->next->studentId != deleteId) {
                temp = temp->next;
            }

            if (temp->next == nullptr) {
                cout << "No record exists.\n";
                return;
            }
            
            displayHeader();
            temp->displayInfo();
            cout << "\nConfirm deletion of this record? (Y/n): ";
            if(confirm()){
                StudentInformation* deleteMatch = temp->next;
                temp->next = temp->next->next;
                delete deleteMatch;
                cout << "Record deleted successfully.\n";
            }
        }
    }

    void rewriteDatabase(fstream& DatabaseFile){    // rewrites database text file with local program database
        if(isModified == true){
            DatabaseFile.open("studentdatabase.txt", ios::out);

            StudentInformation* temp = head;

            DatabaseFile << "::" << StudentInformation::counter << '\n';
            while (temp != NULL) {
                DatabaseFile << '#' << temp->nodeId << '\n';
                DatabaseFile << temp->studentId << '\n';
                DatabaseFile << temp->firstName << '\n';
                DatabaseFile << temp->lastName << '\n';
                DatabaseFile << temp->birthday << '\n';
                DatabaseFile << temp->address << '\n';
                DatabaseFile << temp->gender << '\n';
                DatabaseFile << temp->program << '\n';
                DatabaseFile << temp->yearLevel << '\n';
                DatabaseFile << '\n';

                temp = temp->next;
            }
            cout << "Changes applied. Database updated successfully.\n";
        }
        else{
            cout << "Database is not modified. No changes to save.\n";
        }
        DatabaseFile.close();
    }
};

void addRecord(StudentDataBase& studentDataBase);
void deleteRecord(StudentDataBase& studentDataBase);
void searchRecord(StudentDataBase& studentDataBase);
void saveChanges(StudentDataBase& studentDataBase, fstream& DatabaseFile);
bool confirmExit(StudentDataBase studentDataBase);

int main(){
    fstream DatabaseFile("studentdatabase.txt", ios::in);
    if(!DatabaseFile.is_open()){
        DatabaseFile.open("studentdatabase.txt", ios::out);
    }
    else{
        DatabaseFile.close();
    }

    StudentDataBase studentDataBase;
    studentDataBase.readDatabase(DatabaseFile);
    int option;
    
    do{
        system("cls");
        cout << "[1] - Add New Record\n"
             << "[2] - Search Record\n"
             << "[3] - Display All Records\n"
             << "[4] - Delete Record\n"
             << "[5] - Save Changes\n"
             << "[6] - Exit\n"
             << "Enter option: ";
        option = getOption();
        
        system("cls");
        switch(option){
            case 1:
                addRecord(studentDataBase);
                pressContinue();
                break;
            case 2:
                searchRecord(studentDataBase);
                break;
            case 3:
                studentDataBase.viewRecords();
                pressContinue();
                break;
            case 4:
                deleteRecord(studentDataBase);
                break;
            case 5:
                saveChanges(studentDataBase, DatabaseFile);
                pressContinue();
                break;
            case 6:
                if(confirmExit(studentDataBase)){
                    return 0;
                }
                break;
            default:
                system("cls");
                cout << "Invalid input\n";
                pressContinue();
        }
    } while(true);

    return 0;
}

void addRecord(StudentDataBase& studentDataBase){   // interface for adding
    StudentInformation* newStudent = new StudentInformation;

    cout << "Enter the following information\n";
    do{
        cout << "Student ID Number: ";
        newStudent->studentId = getOption();
        if(studentDataBase.idExisting(newStudent->studentId)){
            cout << "Student ID already exists. Please check and try again.\n";
        }
        else{
            break;
        }
    } while(true);
    cout << "First Name: ";
    getline(cin, newStudent->firstName);
    cout << "Last Name: ";
    getline(cin, newStudent->lastName);
    cout << "Birthday (mm/dd/yyyy): ";
    getline(cin, newStudent->birthday);
    cout << "Address: ";
    getline(cin, newStudent->address);
    do{
        cout << "Gender(M or F): ";
        getline(cin, newStudent->gender);
        if(newStudent->gender == "M" || newStudent->gender == "m" || newStudent->gender == "F" || newStudent->gender == "F"){
            break;
        }
        else{
            cout << "Invalid input.\n";
        }
    } while(true);
    cout << "Degree Program: ";
    getline(cin, newStudent->program);
    cout << "Year Level: ";
    newStudent->yearLevel = getOption();
    
    studentDataBase.addStudent(newStudent);
    studentDataBase.isModified = true;
    studentDataBase.unsavedChanges = true;
}

void deleteRecord(StudentDataBase& studentDataBase){    // interface for deleting
    int id, option;

    if(studentDataBase.head == NULL){
        cout << "Database is empty.\n";
        pressContinue();
        return;
    }
    do{
        cout << "[1] - By Database ID\n"
             << "[2] - By Student ID\n"
             << "[3] - Back\n"
             << "Option: ";
        option = getOption();

        system("cls");
        switch(option){
            case 1:
                cout << "Enter database ID to delete: ";
                id = getOption();
                studentDataBase.deleteRecordDatabaseId(id);
                break;
            case 2:
                cout << "Enter Student ID to delete: ";
                id = getOption();
                studentDataBase.deleteRecordStudentId(id);
                break;
            case 3:
                return;
            default:
                cout << "Invalid input.\n";
        }
        
        if(option == 1 || option == 2){
            studentDataBase.isModified = true;
            studentDataBase.unsavedChanges = true;
            pressContinue();
        }
    } while(true);
}

void searchRecord(StudentDataBase& studentDataBase){    // interface for search options
    int option, option2;
    string data;
    int intData;
    
    if(studentDataBase.head == nullptr){
        cout << "Database is empty.\n";
        pressContinue();
        return;
    }

    do{
        cout << "Choose an option for searching\n"
                  << "[1] - By Student ID\n"
                  << "[2] - By Name\n"
                  << "[3] - By Gender\n"
                  << "[4] - By Degree Program\n"
                  << "[5] - By Year Level\n"
                  << "[6] - Back\n"
                  << "Option: ";
        option = getOption();

        system("cls");
        switch (option){
            case 1:
                cout << "Enter student ID: ";
                cin >> intData;
                cin.ignore();
                data = to_string(intData);
                break;
            case 2:
                cout << "[1] - By First Name\n"
                     << "[2] - By Last Name\n"
                     << "[3] - By Full Name\n"
                     << "[4] - Back\n"
                     << "Option: ";
                option2 = getOption();
                system("cls");
                switch(option2){
                    case 1:
                        cout << "Enter first name: ";
                        getline(cin, data);
                        option = 6;
                        break;
                    case 2: 
                        cout << "Enter last name: ";
                        getline(cin, data);
                        option = 7;
                        break;
                    case 3:
                        cout << "Enter full name: ";
                        getline(cin, data);
                        break;
                    case 4:
                        return;
                    default:
                        option = 0;
                        cout << "Invalid input.\n";
                }
                break;
            case 3:
                cout << "Enter gender: ";
                getline(cin, data);
                break;
            case 4:
                cout << "Enter degree program: ";
                getline(cin, data);
                break;
            case 5:
                cout << "Enter year level: ";
                cin >> intData;
                cin.ignore();
                data = to_string(intData);
                break;
            case 6:
                return;
            default:
                option = 0;
                cout << "Invalid input.\n";
                break;
        }
        
        if(option == 0){
            continue;
        } 
        else{
            studentDataBase.searchDatabase(data, option);
            pressContinue(); 
        }
    } while(true);
}

void saveChanges(StudentDataBase& studentDataBase, fstream& DatabaseFile){  // confirmation for saving changes
    string choice;
    cout << "Saving changes will finalize the changes made in this program, rewriting the database file.\n";
    cout << "Proceed? (Y/n): ";

    do{
        getline(cin, choice);
        if(choice == "Y" || choice == "y"){
            studentDataBase.rewriteDatabase(DatabaseFile);
            studentDataBase.unsavedChanges = false;
            studentDataBase.isModified = false;
            break;
        }
        else if (choice == "N" || choice == "n"){
            break;
        }
        else{
            cout << "Invalid choice.\n";
            cout << "Please enter again (Y/n): ";
        }
    } while(true);
}

bool confirmExit(StudentDataBase studentDataBase){  // confirmation for exiting program
    if(studentDataBase.unsavedChanges){
        cout << "You have unsaved changes. Closing the program will lose these changes.\n";
        cout << "Proceed to exit?(Y/n): ";
        if(confirm()){
            system("cls");
            cout << "Program exiting...\n\n";
            cout << "GROUP A\n"
                    << "Agustin, Keanu\n"
                    << "Imperial, Jacqueline\n"
                    << "Joaquin, Arkinne Yuel\n"
                    << "Llarvers, John Kaiser\n"
                    << "Saavedra, Clarence Vance\n"
                    << "Santos, Jericho\n";
            return true;
        }
        else{
            return false;
        }
    }
    else{
        cout << "Program exiting...\n\n";
        cout << "GROUP A\n"
                << "Agustin, Keanu\n"
                << "Imperial, Jacqueline\n"
                << "Joaquin, Arkinne Yuel\n"
                << "Llarvers, John Kaiser\n"
                << "Saavedra, Clarence Vance\n"
                << "Santos, Jericho\n";
        return true;
    }
}