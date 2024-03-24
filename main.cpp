#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <limits>
#include <type_traits> // testing
#include <sstream>

using namespace std;

#define SETW_LEFT(width) setw(width) << left

struct StudentInformation{
    int nodeId; 
    int studentId, yearLevel;
    string fullName, birthday, address, gender, program;
    static int counter;

    StudentInformation* prev = NULL;
    StudentInformation* next = NULL;

    StudentInformation(){
        counter++;
        nodeId = counter;
    }

    void displayInfo(){
        
        cout << setw(12) << nodeId << "| " << SETW_LEFT(11) << studentId << "| " << SETW_LEFT(43) << fullName
                  << "| " << SETW_LEFT(11) << birthday << "| " << SETW_LEFT(8) << gender
                << "| " << SETW_LEFT(15) << program << "| " << SETW_LEFT(11) << yearLevel << "| " << SETW_LEFT(13) << address << '\n';
    }
};

int StudentInformation::counter = 0;

class StudentDataBase{
    public:
    bool isModified = false;
    bool unsavedChanges = false;
    StudentInformation* head = NULL;

    void addStudent(StudentInformation* studentInfo){
        StudentInformation* newStudent = studentInfo;
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

    bool idExisting(int id){
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

    void readDatabase(fstream& DatabaseFile){
        DatabaseFile.open("studentdatabase.txt", ios::in);
        string line;
        
        getline(DatabaseFile, line);
        if(line.find("::") != string::npos){
            StudentInformation::counter = stoi(line.substr(line.find("::") + 2));
        }
        else{
            StudentInformation::counter = 0;
        }

        while(getline(DatabaseFile, line)){
            if(line.find('#') != string::npos){
                StudentInformation* newStudent = new StudentInformation;
                newStudent->nodeId = stoi(line.substr(line.find('#') + 1));
                
                getline(DatabaseFile, line);
                newStudent->studentId = stoi(line);
                getline(DatabaseFile, line);
                newStudent->fullName = line;
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

        DatabaseFile.close();
    }

    void displayHeader(){
        cout << SETW_LEFT(12) << "Database ID" << SETW_LEFT(13) << "| Student ID" << SETW_LEFT(45) << "| Full Name" 
             << SETW_LEFT(13) << "| Birthday" << SETW_LEFT(10) << "| Gender" 
             << SETW_LEFT(17) << "| Degree Program" << SETW_LEFT(13) << "| Year Level" << SETW_LEFT(15) << "| Address" << '\n';
        cout << setfill('=') << setw(140) << "=" << setfill(' ') << '\n';
    }

    void viewRecords(){
        StudentInformation* temp = head;
        if(temp != NULL){
            displayHeader();
            while(temp != NULL){
                temp->displayInfo();
                temp = temp->next;
            }
        }
        else{
            cout << "Database is empty.\n";
        }
    }

    void searchDatabase(string data, int option){
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

    StudentInformation* searchByStudentId(int id){
        if (head == nullptr) {
            cout << "List is empty.\n";
            return NULL;
        }

        StudentInformation* temp = head;
        if(head->studentId == id){
            return head;
        }

        while(temp != NULL && temp->studentId != id){
            temp = temp->next;
        }

        if(temp == NULL){
            cout << "No match found.\n";
            return NULL;
        }
        else if(temp->studentId == id){
            return temp;
        }

        return NULL;
    }

    void deleteRecord(int nodeId){
        int deleteId = nodeId;

        if(head->nodeId == deleteId) {
			StudentInformation* temp = head;
			head = head->next;
			delete temp;
			
			cout << "Record deleted successfully.\n";
		}
        else{
		    StudentInformation* temp = head;

            while (temp->next != nullptr && temp->next->nodeId != deleteId) {
                cout << "test\n";
                temp = temp->next;
            }

            if (temp->next == nullptr) {
                cout << "No record exists.\n";
                return;
            }
            
            StudentInformation* deleteMatch = temp->next;
            temp->next = temp->next->next;
            delete deleteMatch;
            cout << "Record deleted successfully.\n";
        }
    }

    void rewriteDatabase(fstream& DatabaseFile){
        if(isModified == true){
            DatabaseFile.open("studentdatabase.txt", ios::out);

            StudentInformation* temp = head;

            DatabaseFile << "::" << StudentInformation::counter << '\n';
            while (temp != NULL) {
                DatabaseFile << '#' << temp->nodeId << '\n';
                DatabaseFile << temp->studentId << '\n';
                DatabaseFile << temp->fullName << '\n';
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

void pressContinue();
int getOption();
void addRecord(StudentDataBase& studentDataBase);
void deleteRecord(StudentDataBase& studentDataBase);
void searchRecord(StudentDataBase& studentDataBase);
void saveChanges(StudentDataBase& studentDataBase, fstream& DatabaseFile);

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
        cout << "[1] - Add New Record\n"
             << "[2] - Search Record\n"
             << "[3] - Display All Records\n"
             << "[4] - Delete Record\n"
             << "[5] - Save Changes\n"
             << "[6] - Exit\n"
             << "Enter option: ";
        option = getOption();

        switch(option){
            case 1:
                system("cls");
                addRecord(studentDataBase);
                pressContinue();
                break;
            case 2:
                system("cls");
                searchRecord(studentDataBase);
                break;
            case 3:
                system("cls");
                studentDataBase.viewRecords();
                pressContinue();
                break;
            case 4:
                system("cls");
                deleteRecord(studentDataBase);
                pressContinue();
                break;
            case 5:
                system("cls");
                saveChanges(studentDataBase, DatabaseFile);
                pressContinue();
                break;
            case 6:
                if(studentDataBase.unsavedChanges){
                    char confirm;
                    cout << "You have unsaved changes. Closing the program will lose these changes.\n";
                    cout << "Proceed to exit?(Y/n): ";
                    cin >> confirm;
                    cin.ignore();
                    switch(confirm){
                        case 'Y':
                        case 'y':
                            system("cls");
                            cout << "Program exiting...\n\n";
                            cout << "GROUP A\n"
                                 << "Agustin, Keanu\n"
                                 << "Imperial, Jacqueline\n"
                                 << "Joaquin, Arkinne Yuel\n"
                                 << "Llarvers, John Kaiser\n"
                                 << "Saavedra, Clarence Vance\n"
                                 << "Santos, Jericho\n";
                            return 0;
                            break;
                        case 'N':
                        case 'n':
                            system("cls");
                            continue;
                        default:
                            system("cls");
                            cout << "Invalid option.\n";
                            continue;
                    }
                }
                else{
                    system("cls");
                    cout << "Program exiting...\n\n";
                    cout << "GROUP A\n"
                         << "Agustin, Keanu\n"
                         << "Imperial, Jacqueline\n"
                         << "Joaquin, Arkinne Yuel\n"
                         << "Llarvers, John Kaiser\n"
                         << "Saavedra, Clarence Vance\n"
                         << "Santos, Jericho\n";
                    return 0;
                }
            default:
                system("cls");
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input\n";
                pressContinue();
        }
    } while(true);

    return 0;
}

void pressContinue(){
	cout << "Press any key to continue...\n";
    cin.get();
	system("cls");
}

int getOption(){
	std::string inputOption;
	int option;
	do{
		std::getline(std::cin, inputOption);

		std::stringstream ss(inputOption);
		if (ss >> option) {
			char remaining;
			if (ss >> remaining) {
				std::cout << "\x1b[1A";
    			std::cout << "\x1b[2K";
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
		else { // Conversion failed
			std::cout << "\x1b[1A";
    		std::cout << "\x1b[2K";
			std::cerr << "Invalid input. Please enter only an integer.\n"
					  << "Enter again: ";
		}
	}while(true);
}

void addRecord(StudentDataBase& studentDataBase){
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
    cout << "Full Name: ";
    getline(cin, newStudent->fullName);
    cout << "Birthday (mm/dd/yyyy): ";
    getline(cin, newStudent->birthday);
    cout << "Address: ";
    getline(cin, newStudent->address);
    cout << "Gender(M or F): ";
    getline(cin, newStudent->gender);
    cout << "Degree Program: ";
    getline(cin, newStudent->program);
    cout << "Year Level: ";
    cin >> newStudent->yearLevel;
    cin.ignore();
    
    studentDataBase.addStudent(newStudent);
    studentDataBase.isModified = true;
    studentDataBase.unsavedChanges = true;
}

void deleteRecord(StudentDataBase& studentDataBase){

    if(studentDataBase.head == NULL){
        cout << "Database is empty.\n";
    }
    else{
        int nodeId;
        cout << "Enter database ID to delete: ";
        cin >> nodeId;
        cin.ignore();

        studentDataBase.deleteRecord(nodeId);
        studentDataBase.isModified = true;
        studentDataBase.unsavedChanges = true;
    }
}

void searchRecord(StudentDataBase& studentDataBase){
    int option;
    string data;
    int intData;

    do{
        cout << "Choose an option for searching\n"
                  << "[1] - By Student ID\n"
                  << "[2] - By Name\n"
                  << "[3] - By Gender\n"
                  << "[4] - By Degree Program\n"
                  << "[5] - By Year Level\n"
                  << "[6] - Exit\n";
        option = getOption();

        switch (option){
            case 1:
                system("cls");
                cout << "Enter student ID: ";
                cin >> intData;
                cin.ignore();
                data = to_string(intData);
                break;
            case 2:
                system("cls");
                cout << "Enter full name: ";
                getline(cin, data);
                break;
            case 3:
                system("cls");
                cout << "Enter gender: ";
                getline(cin, data);
                break;
            case 4:
                system("cls");
                cout << "Enter degree program: ";
                getline(cin, data);
                break;
            case 5:
                system("cls");
                cout << "Enter year level: ";
                cin >> intData;
                cin.ignore();
                data = to_string(intData);
                break;
            case 6:
                system("cls");
                return;
            default:
                system("cls");
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                break;
        }
        studentDataBase.searchDatabase(data, option);
        pressContinue();
    } while(true);
}

void saveChanges(StudentDataBase& studentDataBase, fstream& DatabaseFile){
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