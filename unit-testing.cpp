/*
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

*/