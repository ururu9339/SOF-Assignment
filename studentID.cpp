#include<iostream>
#include<iomanip>
#include<string>
#include<math.h>
#include<fstream>

using namespace std;

// ===================== GLOBAL VARIABLES =====================
const int max_size = 100; // inital and max size for arrays 
int act_size = max_size; // actual size of arrays

// ===================== GLOBAL ARRAYS =====================
int ID[max_size];
string Student_Name[max_size];
string Student_Status[max_size];
string Student_Major[max_size];
int Student_Test[max_size];
int Student_Exam[max_size];
int Student_Assignment[max_size];


// ===================== FUNCTION DECLARATIONS =====================
void clear_terminal();
void display_table(); // will include update function and add function
void add_student();// will lie in display
void update(); // will include delete function 
void delete_student();// will lie in update function
void statistics(); 
void search(); // will include update func


int main(){
    int menu_inpt;

    fstream resultsF;
    fstream gradesF;
    ofstream deletedF;
    ofstream updatedF;
    ofstream statisticsF;
    
    resultsF.open("result.txt");
    gradesF.open("grade.txt");
    deletedF.open("delet.txt");
    updatedF.open("update");
    statisticsF.open("statistics");

    resultsF.close();
    gradesF.close();
    deletedF.close();
    updatedF.close();
    statisticsF.close();

    do{
        cout << "  =============================================\n";
        cout << "Welcome to XMUM Student portal!" << endl;
        cout << "  =============================================\n";

        displayMenu(); // display main menu

        cin >> menu_inpt; // input user choice

        switch (menu_inpt) {
            case 1: display_table("results.txt"); break;
            case 2: addStudent();                break;
            case 3: computeGrade();              break;
            case 4: searchStudent();             break;
            case 5: deleteStudent();             break;
            case 6: updateStudent();             break;
            case 7: statistics();                break;
            case 8:
                cout << "\n  Exiting program. Goodbye!\n\n";
                break;
            default:
                cout << " Invalid choice. Please enter a number from 1 to 9: ";
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
void displayMenu() {
    cout << "\n";
    cout << "  ============= MAIN MENU =============\n";
    cout << "  1. Display Table (results.txt)\n";
    cout << "  2. Add Student\n";
    cout << "  3. Compute Grade\n";
    cout << "  4. Search Student\n";
    cout << "  5. Delete Student\n";
    cout << "  6. Update Student\n";
    cout << "  7. Statistics\n";
    cout << "  8. Exit\n";
    cout << "  =====================================\n";
    cout << "  Please enter your choice: ";
}
