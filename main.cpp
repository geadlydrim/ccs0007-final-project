#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <fstream>

#define SETW_LEFT(width) std::setw(width) << std::left

struct StudentInformation{
    int nodeId;
    int studentId, yearLevel;
    std::string fullName, birthday, address, gender, program;
    static int counter;

    StudentInformation* prev = NULL;
    StudentInformation* next = NULL;

    StudentInformation(){
        counter++;
        nodeId = counter;
    }

    void displayInfo(){
        
        std::cout << std::setw(12) << nodeId << "| " << SETW_LEFT(11) << studentId << "| " << SETW_LEFT(43) << fullName
                  << "| " << SETW_LEFT(13) << birthday << "| " << SETW_LEFT(8) << gender
                << "| " << SETW_LEFT(15) << program << "| " << SETW_LEFT(11) << yearLevel << "| " << SETW_LEFT(13) << address << '\n';
    }
};

int StudentInformation::counter = 0;

class StudentDataBase{
    StudentInformation* head = NULL;

    public:
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

    void viewRecords(){
        std::cout << SETW_LEFT(12) << "Database ID" << SETW_LEFT(13) << "| Student ID" << SETW_LEFT(45) << "| Full Name" 
                  << SETW_LEFT(15) << "| Birthday" << SETW_LEFT(10) << "| Gender" 
                  << SETW_LEFT(17) << "| Degree Program" << SETW_LEFT(13) << "| Year Level" << SETW_LEFT(15) << "| Address" << '\n';
        std::cout << std::setfill('=') << std::setw(140) << "=" << std::setfill(' ') << '\n';
        StudentInformation* temp = head;
        while(temp != NULL){
            temp->displayInfo();
            temp = temp->next;
        }
    }

    void deleteRecord(int nodeId){
        int deleteId = nodeId;
		if (head == NULL) {
			std::cout << "Database is empty.\n";
		}
		else if (head->nodeId == deleteId) {
			StudentInformation* temp = head;
			head = head->next;
			delete temp;
			
			std::cout << "Record deleted successfully.\n";
		}
        else{
		    StudentInformation* temp = head;

            while (temp->next->nodeId != deleteId) {
                temp = temp->next;
                if (temp->next == NULL) {
                    std::cout << "Item is not in cart.\n";
                    return;
                }
            }
            
            StudentInformation* deleteMatch = temp->next;
            temp->next = temp->next->next;
            delete deleteMatch;
            std::cout << "Record deleted successfully.\n";
        }
    }
};

void pressContinue();
void addRecord(StudentDataBase& studentDataBase);
void deleteRecord(StudentDataBase& studentDataBase);
void readDatabase(std::fstream& DatabaseFile, StudentDataBase& studentDatabase);

int main(){

    std::fstream DatabaseFile;

    if(!DatabaseFile){
        DatabaseFile.open("studentdatabase.txt", std::ios::out);
        DatabaseFile.close();
    }

    StudentDataBase studentDataBase;
    readDatabase(DatabaseFile, studentDataBase);
    int option;
    
    do{
        std::cout << "[1] - Add New Record\n"
                  << "[2] - Search Record\n"
                  << "[3] - Display All Records\n"
                  << "[4] - Display Specific Records\n"
                  << "[5] - Delete Record\n"
                  << "[6] - Exit\n"
                  << "Enter option: ";
        std::cin >> option;
        std::cin.ignore();

        switch(option){
            case 1:
                system("cls");
                addRecord(studentDataBase);
                pressContinue();
                break;
            case 3:
                system("cls");
                studentDataBase.viewRecords();
                pressContinue();
                break;
            case 5:
                system("cls");
                deleteRecord(studentDataBase);
                pressContinue();
                break;
            case 6:
                return 0;
            default:
                system("cls");
                std::cin.clear();
                std::cin.ignore(100, '\n');
                std::cout << "Invalid input\n";
                pressContinue();
        }
    } while(true);

    return 0;
}

void pressContinue(){
	std::cout << "Press any key to continue...\n";
    std::cin.get();
	system("cls");
}

void addRecord(StudentDataBase& studentDataBase){
    StudentInformation* newStudent = new StudentInformation;

    std::cout << "Enter the following information\n";
    std::cout << "Student ID Number: ";
    std::cin >> newStudent->studentId;
    std::cin.ignore();
    std::cout << "Full Name: ";
    std::getline(std::cin, newStudent->fullName);
    std::cout << "Birthday: ";
    std::getline(std::cin, newStudent->birthday);
    std::cout << "Address: ";
    std::getline(std::cin, newStudent->address);
    std::cout << "Gender: ";
    std::getline(std::cin, newStudent->gender);
    std::cout << "Degree Program: ";
    std::getline(std::cin, newStudent->program);
    std::cout << "Year Level: ";
    std::cin >> newStudent->yearLevel;
    std::cin.ignore();
    
    studentDataBase.addStudent(newStudent);
}

void deleteRecord(StudentDataBase& studentDataBase){
    int nodeId;
    std::cout << "Enter database ID to delete: ";
    std::cin >> nodeId;
    std::cin.ignore();

    studentDataBase.deleteRecord(nodeId);
}

void readDatabase(std::fstream& DatabaseFile, StudentDataBase& studentDatabase){
    DatabaseFile.open("studentdatabase.txt", std::ios::in);
    std::string line;

    while(std::getline(DatabaseFile, line)){
        if(line.find('#') != std::string::npos){
            StudentInformation* newStudent = new StudentInformation;
            newStudent->nodeId = std::stoi(line.substr(line.find('#') + 1));
            std::getline(DatabaseFile, line);
            newStudent->studentId = std::stoi(line);
            std::getline(DatabaseFile, line);
            newStudent->fullName = line;
            std::getline(DatabaseFile, line);
            newStudent->birthday = line;
            std::getline(DatabaseFile, line);
            newStudent->address = line;
            std::getline(DatabaseFile, line);
            newStudent->gender = line;
            std::getline(DatabaseFile, line);
            newStudent->program = line;
            std::getline(DatabaseFile, line);
            newStudent->yearLevel = std::stoi(line);

            studentDatabase.addStudent(newStudent);
        }
    }

}