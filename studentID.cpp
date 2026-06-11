#include<iostream>
#include<iomanip>
#include<string>
#include<math.h>
#include<fstream>


using namespace std;

// ===================== GLOBAL VARIABLES =====================
const int max_size = 100; // inital max size for arrays 
int act_size = 0; // actual size of arrays

// ===================== GLOBAL ARRAYS =====================
string ID[max_size];
string Student_Name[max_size];
string Student_Status[max_size];
string Student_Major[max_size];
int Student_Test[max_size];
int Student_Exam[max_size];
int Student_Assignment[max_size];


// ===================== FUNCTION DECLARATIONS =====================
void clear_terminal();
void load_data(); // load data from results.txt to arrays
void display_table(); // will include update function and add function
void add_student();// will lie in display
void comp_grade();
void update(); // will include delete function 
void delete_student();// will lie in update function
void statistics(); 
void search(); // will include update func
void display_menu();
void reset();

// ===================== VALIDATION FUNCTION =====================
void name_valid();

int main(){
    int menu_inpt;// user input
    
    // resultsF.open("result.txt");
    // gradesF.open("grade.txt");
    // deletedF.open("delet.txt");
    // updatedF.open("update");
    // statisticsF.open("statistics");

    // resultsF.close();
    // gradesF.close();
    // deletedF.close();
    // updatedF.close();
    // statisticsF.close();
    
    load_data();

    do{
        cout << "  =============================================\n";
        cout << "Welcome to XMUM Student portal!" << endl;
        cout << "  =============================================\n";

        display_menu(); // display main menu

        cin >> menu_inpt; // input user choice

        switch (menu_inpt) {
            case 1: display_table();             break;
            case 2: add_student();                break;
            case 3: comp_grade();              break;
            case 4: search();             break;
            case 5: delete_student();             break;
            case 6: update();             break;
            case 7: statistics();                break;
            case 8:
                cout << "\n  Exiting program. Goodbye!\n\n";
                return 0;
                break;
            default:
                cout << " Invalid choice. Please enter a number from 1 to 8: ";
                clear_terminal();
                break;
        }
    }while (menu_inpt != 9);

    return 0;
}

void clear_terminal(){ // function to clear terminal
    #if defined(_WIN32) || defined(_WIN64) // check for windows OS
        system("cls"); 
    #else 
        system("clear"); // for other OS
    #endif
}

// ===================== DISPLAY MENU =====================
void display_menu() {
    cout << "\n";
    cout << "  ============= MAIN MENU =============\n";
    cout << "  1. Display Table\n";
    cout << "  2. Add Student\n";
    cout << "  3. Compute Grade\n";
    cout << "  4. Search Student\n";
    cout << "  5. Delete Student\n";
    cout << "  6. Update Student Info\n";
    cout << "  7. Data trends\n";
    cout << "  8. Exit\n";
    cout << "  =====================================\n";
    cout << "  Please enter your choice: ";
}

void load_data(){
    ifstream resultsF;

    resultsF.open("results.txt");

    while(resultsF >> ID[act_size] >> ws){
        getline(resultsF, Student_Name[act_size]);
        getline(resultsF, Student_Status[act_size]);
        getline(resultsF, Student_Major[act_size]);
        resultsF >> Student_Test[act_size] >> Student_Exam[act_size] >> Student_Assignment[act_size];
        act_size ++ ;
    }
    resultsF.close();
}

void display_table(){
    clear_terminal();

    cout << "===============================================================================================\n";
    cout << left 
         << "| " << setw(6)  << "ID" 
         << "| " << setw(20) << "Name" 
         << "| " << setw(11) << "Status" 
         << "| " << setw(20) << "Major" 
         << "| " << setw(6)  << "Test" 
         << "| " << setw(6)  << "Exam" 
         << "| " << setw(11)  << "Assignment" << "|\n";
    cout << "===============================================================================================\n";

    for (int i = 0; i < act_size; i++){
        cout << left 
             << "| " << setw(6)  << ID[i] 
             << "| " << setw(20) << Student_Name[i] 
             << "| " << setw(11) << Student_Status[i] 
             << "| " << setw(20) << Student_Major[i] 
             << "| " << setw(6)  << Student_Test[i] 
             << "| " << setw(6)  << Student_Exam[i] 
             << "| " << setw(11)  << Student_Assignment[i] << "|\n";
    }
    cout << "===============================================================================================\n";

    cout << "\nPress ENTER to proceed.";
    cin.ignore(); // delete the buffer input
    cin.get();
}

void add_student(){

    clear_terminal();

    cin.ignore(); // delete the buffer input

    if (act_size==max_size){ // check for meximum amount of data
        cout << "Reached maximum amount of students!" << "\n Delete a student or reset the database.";
        cin.get();
        return;
    }

    ofstream resultsF;
    resultsF.open("text.txt", ios::app);

    int new_studentid = act_size;

    while(new_studentid==act_size){
    
    cout << "Please enter following student data or -1 for exit." << endl;
    cout << "Student ID(S****):";
    getline(cin, ID[act_size]);

    if(ID[act_size] == "-1"){ // exit 
        break;
    }

    cout << "\nStudent name, surname:";
    getline(cin, Student_Name[act_size]);

    //validation
    cout << "\nStudent status(Full Time/Part Time/N/A):";
    getline(cin, Student_Status[act_size]);

    cout << "\nStudent major:";
    getline(cin, Student_Major[act_size]);

    cout << "\nStudent test marks:";
    cin >> Student_Test[act_size];

    cout << "\nStudent exam marks:";
    cin >> Student_Exam[act_size];

    cout << "\nStudent assignment marks:";
    cin >> Student_Assignment[act_size];

    resultsF << "\n" << ID[act_size] << " " << Student_Name[act_size]
             << "\n" << Student_Status[act_size]
             << "\n" << Student_Major[act_size]
             << "\n" << Student_Test[act_size]
             << "\n" << Student_Exam[act_size]
             << "\n" << Student_Assignment[act_size];
    act_size++;
    }
    resultsF.close();
}

void search(){
    clear_terminal();

    int search_choice;

    cout << "=========================================\n";
    cout << "            SEARCH STUDENT BY            \n";
    cout << "=========================================\n";
    cout << "  1. Search by ID\n";
    cout << "  2. Search by Name\n";
    cout << "  3. Search by Major\n";
    cout << "  4. Search by Status\n";
    cout << "  5. Search by Grade\n";
    cout << "=========================================\n";
    cout << "Please enter your choice (1-5): ";

    cin >> search_choice;
    cin.ignore();

    if (search_choice < 1 || search_choice > 4){
        cout << "";
        return;
    }
    cout << "\nEnter the value:";
    string search_value;
    getline(cin, search_value);

    bool found = false;
    for (int i = 0; i<act_size; i++){
        bool match = false;

        switch (search_choice)
        {
        case 1:
            if (ID[i]==search_value) match = true; break;
        case 2:
            if (Student_Name[i]==search_value) match = true; break;
        case 3:
            if (Student_Major[i]==search_value) match = true; break;
        case 4:
            if (Student_Status[i]==search_value) match = true; break;
        case 5:
           break;
        default:
            break;
        }

        if(match){
            found = true;
            cout << left 
             << "| " << setw(6)  << ID[i] 
             << "| " << setw(20) << Student_Name[i] 
             << "| " << setw(11) << Student_Status[i] 
             << "| " << setw(20) << Student_Major[i] 
             << "| " << setw(6)  << Student_Test[i] 
             << "| " << setw(6)  << Student_Exam[i] 
             << "| " << setw(11)  << Student_Assignment[i] << "|\n";
             if (search_choice == 1)break;
        }
    }

    if (!found) {
        cout << "\n No student found matching \"" << search_value << "\".\n";
    }
    cout << "\nPress Enter to return to the main menu...";
    cin.get();
}

void comp_grade(){}
void update(){}
void delete_student(){}
void statistics(){}
void reset(){};


// ===================== VALIDATION FUNCTION =====================
// int name_valid(string name[], int i){
//     char name_ch[];

//     for(int i = 0; i<name[i+1].length; i++){

//     }
// }
