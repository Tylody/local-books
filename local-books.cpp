// pa15.cpp

#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Employee {
private:
    static int employeeCount;
    string fullname;
    int ssn;
    double wage;
    int id;
    string department;
    string dateHired;
    Employee* nextEmployee;
    Employee* prevEmployee;
public:
    Employee(string nameParam = "", int ssnParam = 000000000, double wageParam = 0, int idParam = 0, string departmentParam = "", string dateHiredParam = "") {
        employeeCount++;
        fullname = nameParam;
        ssn = ssnParam;
        wage = wageParam;
        id = idParam;
        department = departmentParam;
        dateHired = dateHiredParam;
        nextEmployee = nullptr;
        prevEmployee = nullptr;
    }
    static Employee* firstEmployee;
    static Employee* lastEmployee;

    static int get_employee_count() { return employeeCount; }
    static void reset_employee_count() { employeeCount = 0; }
    static void decrement_employee_count() { --employeeCount; }
    void setNextEmployee(Employee* next) { nextEmployee = next; }
    void setPrevEmployee(Employee* prev) { prevEmployee = prev; }
    Employee* getNextEmployee() { return nextEmployee; }
    Employee* getPrevEmployee() { return prevEmployee; }

    string getName() { return fullname; }
    int getSSN() { return ssn; }
    double getWage() { return wage; }
    int getID() { return id; }
    string getDepartment() { return department; }
    string getDateHired() { return dateHired; }

    void setName(string param) { fullname = param; }
    void setSSN(int param) { ssn = param; }
    void setWage(double param) { wage = param; };
    void setID(int param) { id = param; }
    void setDepartment(string param) { department = param; }
    void setDateHired(string param) { dateHired = param; }

    void outputEmployeeInformation() { cout << "Employee: " << fullname << endl << "SSN: " << ssn << endl << "Wage: $" << wage << endl << "ID: " << id << endl << "Department: " << department << endl << "Date Hired: " << dateHired << endl; }
};

int Employee::employeeCount = 0;
vector<Employee> employeeStorage(100);
Employee* Employee::firstEmployee = nullptr;
Employee * Employee::lastEmployee = nullptr;

void clear_console() {
    cout << "\x1B[2J\x1B[H";
}
void discard_user_input() {
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
}
string get_user_input(string, int);
string get_user_input();
string get_date();
bool prompt_user_rerun();

bool check_if_employees_exist();
void input_from_text_file();
void manual_input_employees();
void delete_employee();
void edit_employees();
void search_employees();
void create_employee(string, int, double, int, string, string, int);

void display_all_employees();

void export_to_file();


int main()
{
    Employee::reset_employee_count();
    cout << "\nEmployee Information Database\n------------------------\nWould you like to: \n1) Import data from a text file\n2) Manually input data\n\n";
    string input = get_user_input("12", 1);

    char selectedAction = input[0];
    switch (selectedAction) {
    case '1':
        discard_user_input();
        input_from_text_file();
        break;
    case '2':
        manual_input_employees();
        break;
    }

    while (true) {
        clear_console();

        cout << "\nEmployee Information Database\n------------------------\nAvailable Actions: \n1) Add new employees\n2) Delete employees\n3) Edit employee information\n4) Search for employees\n5) Display all employees\n6) Export database to file\n7) Exit Program\n\n";

        string choice = get_user_input("1234567", 1);
        switch (choice[0]) {
        case '7':
            cout << "\nExiting program...\n";
            break;
        case '1':
            manual_input_employees();
            continue;
        case '2':
            delete_employee();
            continue;
        case '3':
            edit_employees();
            continue;
        case '4':
            search_employees();
            continue;
        case '5':
            display_all_employees();
            continue;
        case '6':
            export_to_file();
            continue;
        }
        break;
    }

    cout << "Program made by Tyler Hoang.";
}

void input_from_text_file() {
    clear_console();

    ifstream file;

    while (true) {
        cout << "\nName of text file to input: ";
        string filename = get_user_input();

        file.open(filename);

        if (file)
            break;

        cout << "\nInvalid file! Try again.";
    }

    string name;
    string ssnString;
    string wageString;
    string idString;
    string department;
    string dateHired;

    int ssn;
    double wage;
    int id;
    bool validData;

    int lineNumber = 0;
    getline(file, name);

    while (!file.eof()) {

        if (Employee::get_employee_count() >= 100) {
            cout << "\nError! Max number of employees reached. No new employees will be added.";
            break;
        }

        ++lineNumber;
        validData = true;
        getline(file, name, '\t');
        getline(file, ssnString, '\t');
        getline(file, wageString, '\t');
        getline(file, idString, '\t');
        getline(file, department, '\t');
        getline(file, dateHired,'\n');


        if (ssnString.length() != 9 && ssnString.find_first_not_of("1234567890") != -1)
            validData = false;
        if (wageString.find_first_not_of("1234567890") != -1)
            validData = false;
        if (idString.length() != 3 && idString.find_first_not_of("1234567890") != -1)
            validData = false;

        if (!validData) {
            cout << "\nError! Due to invalid data on line " << lineNumber << ", this employee has not been added to the database.";
            continue;
        }

        ssn = stoi(ssnString);
        wage = stod(wageString);
        id = stoi(idString);

        create_employee(name, ssn, wage, id, department, dateHired, 0);
    }

    file.close();

    cout << "\n\nSucessfully written! Press <Enter> to continue...";

    get_user_input();
}

void manual_input_employees() {
    clear_console();

    string name;
    string ssnString;
    string wageString;
    string idString;
    string department;
    string dateHired;
    string indexString;

    int ssn;
    double wage;
    int id;
    int index;

    while (true) {
        cout << "\nEnter the information of the employee";
        discard_user_input();
        cout << "\nName (or 0 to exit): ";
        name = get_user_input();
        if (name == "0")
            return;
        cout << "SSN: ";
        ssnString = get_user_input("1234567890", 9);
        cout << "Wage: $";
        wageString = get_user_input("1234567890",0);
        cout << "ID: ";
        idString = get_user_input("1234567890", 3);
        discard_user_input();
        cout << "Department: ";
        department = get_user_input();
        cout << "Date Hired (mm-dd-yyyy): ";
        dateHired = get_date();

        cout << "Index for insertion, or 0 to insert at end of list: ";
        indexString = get_user_input("1234567890", 0);

        ssn = stoi(ssnString);
        wage = stod(wageString);
        id = stoi(idString);

        Employee* currentPointer = Employee::firstEmployee;
        Employee* nextPointer = nullptr;
        bool unusedID = true;

        if (currentPointer != nullptr) {
            while (true) {
                if (currentPointer->getID() == id) {
                    cout << "\nError! ID already in use! Cannot create employee entry.\n";
                    unusedID = false;
                    break;
                }
                nextPointer = currentPointer->getNextEmployee();
                if (nextPointer == nullptr)
                    break;
                currentPointer = nextPointer;
            }
        }

        if (!unusedID)
            continue;

        if (indexString.empty()) {
            index = 0;
        }
        else {
            index = stoi(indexString);
        }

        create_employee(name, ssn, wage, id, department, dateHired, index);

        cout << "\nEmployee created!";

        cout << "\nWould you like to create another employee? (y/n): ";
        if (prompt_user_rerun())
            continue;

        discard_user_input();

        cout << "\n\nPress <Enter> to continue...";
        get_user_input();

        break;
    }
}

void create_employee(string name, int ssn, double wage, int id, string department, string dateHired, int index) {

    employeeStorage[Employee::get_employee_count()] = Employee(name, ssn, wage, id, department, dateHired);

    Employee* myEmployeePtr = &employeeStorage[Employee::get_employee_count()];

    if (Employee::get_employee_count() == 1) {
        Employee::firstEmployee = myEmployeePtr;
        Employee::lastEmployee = myEmployeePtr;
        return;
    }

    if (index == 0) {
        Employee::lastEmployee->setNextEmployee(myEmployeePtr);
        employeeStorage[Employee::get_employee_count()].setPrevEmployee(Employee::lastEmployee);
        Employee::lastEmployee = myEmployeePtr;
        return;
    }

    if (index == 1) {
        Employee::firstEmployee->setPrevEmployee(myEmployeePtr);
        Employee::firstEmployee = myEmployeePtr;
        return;
    }

    Employee* currentPointer = Employee::firstEmployee;
    Employee* nextPointer = nullptr;

    for (int i = 1; i < index; ++i) {
        nextPointer = currentPointer->getNextEmployee();
        if (nextPointer == nullptr)
            break;
        currentPointer = nextPointer;
    }
    Employee* previousPointer = currentPointer->getPrevEmployee();
    previousPointer->setNextEmployee(myEmployeePtr);
    currentPointer->setPrevEmployee(myEmployeePtr);
    employeeStorage[Employee::get_employee_count()].setPrevEmployee(previousPointer);
    employeeStorage[Employee::get_employee_count()].setNextEmployee(currentPointer);

    return;
}

void delete_employee() {
    clear_console();

    while (true) {

        if (!check_if_employees_exist())
            return;

        cout << "\nEnter the index of the employee to remove, or 0 to exit: ";
        string indexString = get_user_input("1234567890", 0);
        int index = stoi(indexString);

        if (index == 0)
            break;

        Employee* currentPtr = Employee::firstEmployee;
        Employee* nextPtr = nullptr;
        for (int i = 1; i < index; i++) {
            nextPtr = currentPtr->getNextEmployee();
            if (nextPtr == nullptr)
                break;
            currentPtr = nextPtr;
        }
        
        cout << "\nSelected employee: \n";
        currentPtr->outputEmployeeInformation();

        cout << "\n\nConfirm deletion? (y/n)";
        string input = get_user_input("yn", 1);
        if (input[0] == 'n')
            continue;

        bool previousPtrNonexistant = (currentPtr->getPrevEmployee() == nullptr);
        bool nextPtrNonexistant = (currentPtr->getNextEmployee() == nullptr);

        if (previousPtrNonexistant && nextPtrNonexistant) {
            Employee::firstEmployee = nullptr;
            Employee::lastEmployee = nullptr;
            Employee::reset_employee_count();
        }

        if (previousPtrNonexistant && !nextPtrNonexistant) {
            currentPtr->getNextEmployee()->setPrevEmployee(nullptr);
            Employee::firstEmployee = currentPtr->getNextEmployee();
        }

        if (!previousPtrNonexistant && nextPtrNonexistant) {
            currentPtr->getPrevEmployee()->setNextEmployee(nullptr);
            Employee::lastEmployee = currentPtr->getPrevEmployee();
        }

        if (!previousPtrNonexistant && !nextPtrNonexistant) {
            currentPtr->getPrevEmployee()->setNextEmployee(currentPtr->getNextEmployee());
            currentPtr->getNextEmployee()->setPrevEmployee(currentPtr->getPrevEmployee());
        }


        cout << "\nEmployee at index " << index << " deleted!";

        cout << "\nWould you like to delete another employee? (y/n): ";
        if (prompt_user_rerun()) 
            continue;

        discard_user_input();
        cout << "\n\nPress <Enter> to continue...";
        get_user_input();

        break;
    }
}

void edit_employees() {
    clear_console();

    if (!check_if_employees_exist())
        return;

    string name;
    string ssnString;
    string wageString;
    string idString;
    string department;
    string dateHired;
    string indexString;

    int ssn;
    double wage;
    int id;

    while (true) {
        cout << "\nEnter the index of the employee to edit, or type 0 to exit: ";
        string indexString = get_user_input("1234567890", 0);
        int index = stoi(indexString);

        if (index == 0)
            break;

        Employee* currentPtr = Employee::firstEmployee;
        Employee* nextPtr = nullptr;
        for (int i = 1; i < index; i++) {
            nextPtr = currentPtr->getNextEmployee();
            if (nextPtr == nullptr)
                break;
            currentPtr = nextPtr;
        }

        cout << "\n\nCurrent information:\n";
        currentPtr->outputEmployeeInformation();

        cout << "\n\nEnter the updated information of the employee";
        discard_user_input();
        cout << "\nName (or 0 to exit): ";
        name = get_user_input();
        if (name == "0")
            return;
        cout << "SSN: ";
        ssnString = get_user_input("1234567890", 9);
        cout << "Wage: $";
        wageString = get_user_input("1234567890", 0);
        cout << "ID: ";
        idString = get_user_input("1234567890", 3);
        discard_user_input();
        cout << "Department: ";
        department = get_user_input();
        cout << "Date Hired (mm-dd-yyyy): ";
        dateHired = get_date();

        ssn = stoi(ssnString);
        wage = stod(wageString);
        id = stoi(idString);

        currentPtr->setName(name);
        currentPtr->setSSN(ssn);
        currentPtr->setWage(wage);
        currentPtr->setID(id);
        currentPtr->setDepartment(department);
        currentPtr->setDateHired(dateHired);

        cout << "\nWould you like to edit another employee? (y/n): ";
        if (prompt_user_rerun())
            continue;

        cout << "\n\nPress <Enter> to continue...";
        get_user_input();

        break;
    }
}

void search_employees() {
    clear_console();

    if (!check_if_employees_exist())
        return;

    while (true) {
        cout << "\nWould you like to search by ID or index number? (1 or 2, or type 0 to exit): ";
        string choice = get_user_input("012", 0);

        if (choice[0] == '0') {
            return;
        }

        if (choice[0] == '1') { // search by id
            cout << "\nEnter the id: ";
            string idString = get_user_input("1234567890", 0);
            int id = stoi(idString);

            bool found = false;
            Employee* currentPointer = Employee::firstEmployee;
            Employee* nextPointer = nullptr;

            int i = 0;
            while (true) {
                ++i;
                if (currentPointer->getID() == id) {
                    found = true;
                    break;
                }
                nextPointer = currentPointer->getNextEmployee();
                if (nextPointer == nullptr) {
                    break;
                }
                currentPointer = nextPointer;
            }

            if (found) {
                cout << "\nEmployee found at ID " << i << ": \n";
                currentPointer->outputEmployeeInformation();
            }
            else {
                cout << "\nEmployee not found.";
            }
        }

        if (choice[0] == '2') { // search by index
            cout << "\nEnter the index, or 0 to exit: ";
            string indexString = get_user_input("1234567890", 0);
            int index = stoi(indexString);

            if (indexString == "0")
                continue;

            Employee* currentPointer = Employee::firstEmployee;
            Employee* nextPointer = nullptr;

            for (int i = 1; i < index; ++i) {
                nextPointer = currentPointer->getNextEmployee();
                if (nextPointer == nullptr)
                    break;
                currentPointer = nextPointer;
            }

            cout << "\nEmployee found: ";
            currentPointer->outputEmployeeInformation();
        }

        cout << "\n\nWould you like to search for another employee? (y/n): ";
        if (prompt_user_rerun())
            continue;

        cout << "\n\nPress <Enter> to continue...";
        get_user_input();

        break;
    }
}

void display_all_employees() {
    clear_console();

    if (!check_if_employees_exist())
        return;

    cout << "\nIndex\tName\t\tID\tWage\tSSN\t\tDepartment\tHired Date\n";

    Employee* currentPointer = Employee::firstEmployee;
    int i = 0;
    while (currentPointer != nullptr) {
        i++;
        cout << right << i << '\t';
        cout << left << setw(8) << currentPointer->getName().substr(0, 15) << '\t';
        cout << right << currentPointer->getID() << '\t' << currentPointer->getWage() << '\t' << currentPointer->getSSN() << '\t';
        cout << left << setw(12) << currentPointer->getDepartment().substr(0, 15) << '\t';
        cout << currentPointer->getDateHired() << endl;

        currentPointer = currentPointer->getNextEmployee();
    }
    discard_user_input();
    cout << endl << endl << "Press <Enter> to continue...";
    get_user_input();
}

void export_to_file() {
    clear_console();
    discard_user_input();
    cout << "\nExporting to file...";

    ofstream file;

    file.open("output.txt");

    Employee* currentPointer = Employee::firstEmployee;

    while (currentPointer != nullptr) {
        file << endl << currentPointer->getName() << '\t' << currentPointer->getSSN() << '\t' << currentPointer->getWage() << '\t' << currentPointer->getID() << '\t' << currentPointer->getDepartment() << '\t' << currentPointer->getDateHired();

        currentPointer = currentPointer->getNextEmployee();
    }

    file.close();
    cout << "\n\nSuccessfully exported to output.txt! Press <Enter> to continue...";
    get_user_input();
}

string get_user_input() {
    while (true) {
        string toInput;
        getline(cin, toInput);
        if (toInput.find("\t") != -1) {
            cout << "Invalid input! Try again: ";
            continue;
        }
        return toInput;
    }
}
string get_user_input(string filter, int chars) {
    string toInput;
    while (true) {
        cin >> toInput;
        if (toInput.find_first_not_of(filter) != -1) {
            cout << "Invalid input! Try again: ";
            continue;
        }
        if (chars != 0 && toInput.length() != chars) {
            cout << "Invalid input length! Only input " << chars << " character(s) please." << "Try again: ";
            continue;
        }
        break;
    }
    return toInput;
}
string get_date() {

    string toInput;

    while (true) {
        toInput = get_user_input("1234567890-", 10);

        int firstSlashIndex = toInput.find("-");
        int lastSlashIndex = toInput.rfind("-");

        int secondSlashIndex = toInput.find("-", 3);

        if (firstSlashIndex == secondSlashIndex || secondSlashIndex != lastSlashIndex) {
            cout << "Invalid date! Input a date in format mm/dd/yyyy. Try again: ";
            continue;
        }
        break;
    }

    return toInput;
}
bool prompt_user_rerun() {
    string input = get_user_input("yn", 1);
    if (input[0] == 'y')
        return true;

    return false;

}
bool check_if_employees_exist() {
    if (Employee::get_employee_count() == 0) {
        discard_user_input();
        cout << "Error! No employees exist. \n\nPress <Enter> to continue...";
        get_user_input();
        return false;
    }

    return true;
}