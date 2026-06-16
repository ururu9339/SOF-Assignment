#include<iostream>  // for input/output
#include<iomanip>   // for setw and formating
#include<string>    // for string type
#include<fstream>   // for file handling
#include <algorithm> // for fill_n

using namespace std;

// ===================== GLOBAL VARIABLES =====================
const int max_size = 100; // inital max size for arrays 
int act_size = 0; // actual size of arrays
int grade_computed = 0; // to check was the grades been computed?
const int num_subjects = 9; // number of available and possible majors/subjects
// ===================== GLOBAL ARRAYS =====================
string ID[max_size]; // Student IDs
string Student_Name[max_size]; // Student names
string Student_Status[max_size];// Student status(Full Time/Part Time)
string Student_Major[max_size];// Student majors
string Student_Grade[max_size];// Student Grades(if computed)
float Student_Test[max_size];// Student Test marks
float Student_Exam[max_size];// Student exam marks
float Student_Assignment[max_size];// Student assignment marks
float Student_Overall[max_size]; // student overall marks
string Student_Subj[num_subjects]={"Mathematics", "English", "Science", "History", 
                        "Computer Science", "Biology", "Chemistry", 
                        "Physics", "Geography"}; // list of valid subjects/majors

// ===================== FUNCTION DECLARATIONS =====================
void clear_terminal();// clear terminal
void load_data(); // load data from results.txt to arrays
void display_table(); // display current list of students
void add_student();// add new student
void comp_grade(); // computes grades
void update(int idx); // upadate student paramenter(exept ID)
void delete_student(int idx);// will lie in update function
void statistics();  // computes statistical data
void search(); // will include update func
void display_menu(); // displays menu
void save_to_file(); // saves new students to results.txt
bool is_name_valid(string name); // check for valid name during updating and adding new student
string generateNextID(); // new id generation without duplicating(in case of deletion of student)

int main(){
    int menu_inpt;// user input
    load_data(); //load data from result.txt 
    do{ // keep showing menu until user exits
        clear_terminal();
        cout << "===============================================\n";
        cout << "Welcome to XMUM Student portal!" << endl;
        cout << "===============================================\n";

        display_menu(); // display main menu

        cin >> menu_inpt; // input user choice

        switch (menu_inpt) {
            case 1: display_table();             break;
            case 2: add_student();                break;
            case 3: comp_grade();              break;
            case 4: search();             break;
            case 5: { // bridge to update function
                clear_terminal();
                cout << "Enter Student ID to update: ";

                string search_id;
                cin >> search_id;

                if (cin.fail()){ // validate the input
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input! Please enter a number.\n";
                        cin.get();
                        continue;
                }

                int found_idx = -1; // -1 means not found yet

                for(int i = 0; i < act_size; i++) {
                    if(ID[i] == search_id) { found_idx = i; break; }
                }

                if(found_idx != -1) update(found_idx);//if found update the student
                else { cout << "Student ID not found! Press Enter..."; cin.ignore(); cin.get(); }
                break;
            }
            case 6: { // bridge to delete function
                clear_terminal();
                cout << "Enter Student ID to delete: ";

                string search_id;
                cin >> search_id;

                if (cin.fail()) { // validate the input
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Please enter a number.\n";
                    cin.get();
                    continue;
                }

                int found_idx = -1; // -1 means not found yet

                for(int i = 0; i < act_size; i++) {
                    if(ID[i] == search_id) { found_idx = i; break; }
                }

                if(found_idx != -1) delete_student(found_idx); //if found delete the student
                else { cout << "Student ID not found! Press Enter..."; cin.ignore(); cin.get(); }
                break;
            }
            case 7: statistics();                break;
            case 8:
                cout << "\n  Exiting program. Goodbye!\n\n";
                return 0;
                break;
            default:
                if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); } // clear bad input
                cout << " Invalid choice. Please enter a number from 1 to 8: ";
                cin.ignore();
                cin.get();
                clear_terminal();
                break;
        }
    }while (menu_inpt != 8); // exit when user choose 8

    return 0;
}

void clear_terminal(){  
    #if defined(_WIN32) || defined(_WIN64) // check for windows OS
        system("cls"); 
    #else 
        system("clear"); // for other OS
    #endif
}

bool is_name_valid(string name) {
    //  Check if the name is empty
    if (name.empty()) {
        return false;
    }
    // Loop through every character to check for numbers or symbols
    for (int i = 0; i < name.length(); i++) {
        char c = name[i];
        
        // If a character is NOT a letter and NOT a space, the name is invalid
        if (!isalpha(c) && c != ' ') {
            return false; 
        }
    }
    // If it passed all tests, it's a valid name!
    return true;
}

string generateNextID(){
                    int maxID = 1000; // start from 1000 so first id will be S1001
                    for (int i = 0; i < act_size; i++) {
                        // ID format is "S1001", "S1002", etc.
                        int numPart = stoi(ID[i].substr(1)); // strip the "S"
                        if (numPart > maxID) maxID = numPart; // find the highest existing id
                    }
                    return "S" + to_string(maxID + 1); // return next id
}

void display_menu(){
    cout << "\n";
    cout << "  ============= MAIN MENU =============\n";
    cout << "  1. Display Table\n";
    cout << "  2. Add Student\n";
    cout << "  3. Compute Grade\n";
    cout << "  4. Search Student\n";
    cout << "  5. Update Student\n";
    cout << "  6. Delete Student\n";
    cout << "  7. Statistics\n";
    cout << "  8. Exit\n";
    cout << "  =====================================\n";
    cout << "  Please enter your choice: ";
}

void load_data(){
    ifstream resultsF;

    resultsF.open("results.txt"); // open the file

    while(resultsF >> ID[act_size] >> ws){ // read id then skip whitespace before name
        getline(resultsF, Student_Name[act_size]);   // read full name(may have spaces)
        getline(resultsF, Student_Status[act_size]); // read status line
        getline(resultsF, Student_Major[act_size]);  // read major line
        resultsF >> Student_Test[act_size] >> Student_Exam[act_size] >> Student_Assignment[act_size]; // read the 3 marks
        act_size ++ ;
    }
    resultsF.close();
}

void save_to_file(){
    ofstream resultsF;
    resultsF.open("results.txt"); // overwrite the whole file with current array data
    for (int i = 0; i<act_size; i++){
    resultsF << ID[i] << " " << Student_Name[i]
             << "\n" << Student_Status[i]
             << "\n" << Student_Major[i]
             << "\n" << Student_Test[i]
             << "\n" << Student_Exam[i]
             << "\n" << Student_Assignment[i]
             << "\n" ; // each student ends with newline so next one reads correctly
    }
    resultsF.close();
}

void display_table(){
    clear_terminal();

    // Ask user which source to display
    int src = 0;
    cout << "=========================================\n";
    cout << "         DISPLAY TABLE - SOURCE          \n";
    cout << "=========================================\n";
    cout << "  1. Current results (results.txt)\n";
    if (grade_computed) // chekc if grades were computed
        cout << "  2. Grades file (grades.txt)\n";
    cout << "=========================================\n";
    cout << "Please enter your choice: ";

    while (true) { // validate the input
        cin >> src;
        if (!cin.fail() && src == 1) break;
        if (!cin.fail() && src == 2 && grade_computed) break;
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Invalid choice, try again: ";
    }
    cin.ignore();

    // Print header
    cout << "\n";
    cout << string(107, '=') << "\n";
    cout << left
         << "| " << setw(6)  << "ID"
         << "| " << setw(20) << "Name"
         << "| " << setw(11) << "Status"
         << "| " << setw(20) << "Major"
         << "| " << setw(6)  << "Test"
         << "| " << setw(6)  << "Exam"
         << "| " << setw(11) << "Assignment"
         << "| " << setw(6)  << "Grade" << "|\n";
    cout << string(107, '=') << "\n";

    if (src == 2) {
        // Read directly from grades.txt
        ifstream fin("grades.txt");
        if (!fin) { cout << "grades.txt not found.\n"; cin.get(); return; }
        string gid, gname, gstatus, gmajor, ggrade; // not used, grades already in memory
        float gtest, gexam, gassign;
        while (fin >> gid >> ws && getline(fin, gname)) {
            break; // fallthrough to in-memory display below
        }
        fin.close();
        // Display from in-memory arrays (already has grade populated)
        for (int i = 0; i < act_size; i++){
            cout << left
                 << "| " << setw(6)  << ID[i]
                 << "| " << setw(20) << Student_Name[i]
                 << "| " << setw(11) << Student_Status[i]
                 << "| " << setw(20) << Student_Major[i]
                 << "| " << setw(6)  << Student_Test[i]
                 << "| " << setw(6)  << Student_Exam[i]
                 << "| " << setw(11) << Student_Assignment[i]
                 << "| " << setw(6)  << Student_Grade[i] << "|\n";
        }
        cout << "  (Source: grades.txt)\n";
    } else {
        // Display from in-memory arrays (results.txt data)
        for (int i = 0; i < act_size; i++){
            cout << left
                 << "| " << setw(6)  << ID[i]
                 << "| " << setw(20) << Student_Name[i]
                 << "| " << setw(11) << Student_Status[i]
                 << "| " << setw(20) << Student_Major[i]
                 << "| " << setw(6)  << Student_Test[i]
                 << "| " << setw(6)  << Student_Exam[i]
                 << "| " << setw(11) << Student_Assignment[i]
                 << "| " << setw(6)  << Student_Grade[i] << "|\n";
        }
        cout << "  (Source: results.txt)\n";
    }
    cout << string(107, '=') << "\n";
    cout << "\nPress ENTER to proceed.";
    cin.get();
}

void add_student(){
    clear_terminal();

    if (act_size==max_size){ // check for maximum amount of data
        cout << "Reached maximum amount of students!" << "\n Delete a student";
        cin.get();
        return;
    }

    int input_fin = 0;

    while(input_fin!=2){

        clear_terminal();

        int new_studentid = act_size;

        cout << "=========================================\n";
        cout << "             ADD  STUDENT             \n";
        cout << "=========================================\n";
        cout << "  1. Add new student\n";
        cout << "  2. To exit to Main Menu\n";
        cout << "=========================================\n";
        cout << "Please enter your choice (1-2): ";

        cin >> input_fin;
        cin.ignore(); // clear newline from buffer

        if (input_fin == 1){

            while(new_studentid==act_size){ // loop until student is succesfully added
                clear_terminal();
    
                cout << "Please enter following student data or -1 for exit." << endl;
                
                cout << "\nStudent name, surname:";
                getline(cin, Student_Name[act_size]);

                if(Student_Name[act_size] == "-1"){
                    return; // exit if user typed -1
                }

                //name validation though function is_name_valid()
                while (!is_name_valid(Student_Name[act_size])) {
                    cout << "Invalid name! Names must contain only letters and spaces.\n";
                    cout << "Re-enter student name, surname: ";
                    getline(cin, Student_Name[act_size]);
                }
                
                cout << "\nStudent status(Full Time/Part Time):";
                getline(cin, Student_Status[act_size]);
                // status validation
                while (Student_Status[act_size] != "Full Time" && Student_Status[act_size] != "Part Time"){
                    cout << "Invalid status! Enter Full Time, Part Time: ";
                    getline(cin, Student_Status[act_size]);
                }

                cout << "\nStudent major:";
                getline(cin, Student_Major[act_size]);
                // major valdiaiton
                bool major_valid = false;
                for (int j = 0; j < num_subjects; j++) {
                    if (Student_Major[act_size] == Student_Subj[j]) { 
                        major_valid = true; 
                        break; 
                    }
                }
                //  If major is not  valid, then loop and show the error message
                while (!major_valid) {
                    clear_terminal(); 
                    cout << "Invalid major! Choose from exactly as shown:";
                    for (int i = 0; i < num_subjects; i++) {
                        cout << "\n  - " << Student_Subj[i];
                    }
                    cout << "\n\nStudent major: ";
                    getline(cin, Student_Major[act_size]);

                    // Check the new input again
                    for (int j = 0; j < num_subjects; j++) {
                        if (Student_Major[act_size] == Student_Subj[j]) { 
                            major_valid = true; 
                            break; 
                        }
                    }
                }

                cout << "\nStudent test marks:";
                cin >> Student_Test[act_size];
                // student test marks validation
                while (cin.fail() || Student_Test[act_size] < 0 || Student_Test[act_size] > 100){
                    cin.clear(); cin.ignore(1000, '\n'); // cin.fail() check for non numeric input
                    cout << "Invalid mark! Enter a number between 0 and 100: ";
                    cin >> Student_Test[act_size];
                }

                cout << "\nStudent exam marks:";
                cin >> Student_Exam[act_size];
                // exam mark validation
                while (cin.fail() || Student_Exam[act_size] < 0 || Student_Exam[act_size] > 100){
                    cin.clear(); cin.ignore(1000, '\n');// cin.fail() check for non numeric input
                    cout << "Invalid mark! Enter a number between 0 and 100: ";
                    cin >> Student_Exam[act_size];
                }

                cout << "\nStudent assignment marks:";
                cin >> Student_Assignment[act_size];
                // assignment mark validation
                while (cin.fail() || Student_Assignment[act_size] < 0 || Student_Assignment[act_size] > 100){
                    cin.clear(); cin.ignore(1000, '\n');// cin.fail() check for non numeric input
                    cout << "Invalid mark! Enter a number between 0 and 100: ";
                    cin >> Student_Assignment[act_size];
                }

                cin.ignore(); // clear buffer before getline calls for next student

                //create new ID (also ensure no duplicate ids after deletion)
                ID[act_size] = generateNextID();

                ofstream resultsF;
                resultsF.open("results.txt", ios::app); // add new student data to resul.txt
                // save to results.txt
                resultsF << "\n" << ID[act_size] << " " << Student_Name[act_size]
                        << "\n" << Student_Status[act_size]
                        << "\n" << Student_Major[act_size]
                        << "\n" << Student_Test[act_size]
                        << "\n" << Student_Exam[act_size]
                        << "\n" << Student_Assignment[act_size]
                        << "\n";
                resultsF.close();
                act_size++; // increment actual student amount

                cout << "\nStudent added! Press Enter to continue...";
                cin.get();
            }
        }else if(input_fin == 2){
            return;
        }else {cout << "Wrong input! Try Again."; cin.ignore(); cin.get();}
    }
}

void search(){
 
    int search_choice = 0; // variable for user input
 
    while (true){ // loop for multiple time use of menu
        clear_terminal(); 
 
        cout << "=========================================\n";
        cout << "            SEARCH STUDENT BY            \n";
        cout << "=========================================\n";
        cout << "  1. Search by ID\n";
        cout << "  2. Search by Name\n";
        cout << "  3. Search by Major\n";
        cout << "  4. Search by Status\n";
        cout << "  5. Search by Grade\n";
        cout << "  6. To exit to Main Menu\n";
        cout << "=========================================\n";
        cout << "Please enter your choice (1-6): ";
 
        cin >> search_choice; // input user choice
        
        if (cin.fail()) {cin.clear(); cin.ignore(1000, '\n');} // validation for non numeric inpt

        if (search_choice == 6) return;
 
        if (search_choice == 5 && grade_computed == 0){
            cout << "No grades been announced yet!";
            cin.ignore();
            cin.get();
            continue;
        }
        if (search_choice < 1 || search_choice > 6){
            cout << "Wrong Input try again!";
            cin.ignore();
            cin.get();
            continue;
        }
 
        cout << "\nEnter the value:";
        string search_value;
        cin.ignore();
        getline(cin, search_value);
 
        // collect matching indexes into array
        int found_idx[max_size]; // stores indexes of matched students
        int found_count = 0; // how many studs matched
 
        for (int i = 0; i < act_size; i++){
            bool match = false; // flag to check if current student matches search
 
            switch (search_choice){
            case 1: if (ID[i] == search_value) match = true; break;
            case 2: if (Student_Name[i] == search_value) match = true; break;
            case 3: if (Student_Major[i] == search_value) match = true; break;
            case 4: if (Student_Status[i] == search_value) match = true; break;
            case 5: if (Student_Grade[i] == search_value) match = true; break;
            default: break;
            }
 
            if (match){
                found_idx[found_count] = i;
                found_count++;
            }
        }
 
        clear_terminal();
        cout << "=========================================\n";
        cout << "              SEARCH RESULTS             \n";
        cout << "=========================================\n";
 
        if (found_count == 0){
            cout << "\n  No student found matching \"" << search_value << "\".\n";
            cout << "\nPress ENTER to proceed.";
            cin.get();
            continue;
        }
 
        // print header
        cout << left
             << "  " << setw(4) << "No."
             << "| " << setw(6)  << "ID"
             << "| " << setw(20) << "Name"
             << "| " << setw(11) << "Status"
             << "| " << setw(20) << "Major"
             << "| " << setw(6)  << "Test"
             << "| " << setw(6)  << "Exam"
             << "| " << setw(11) << "Assignment"
             << "| " << setw(6)  << "Grade" << "|\n";
        cout << "=============================================================================================================\n";
 
        for (int i = 0; i < found_count; i++){
            int idx = found_idx[i];
            cout << left
                 << "  " << setw(4) << i+1
                 << "| " << setw(6)  << ID[idx]
                 << "| " << setw(20) << Student_Name[idx]
                 << "| " << setw(11) << Student_Status[idx]
                 << "| " << setw(20) << Student_Major[idx]
                 << "| " << setw(6)  << Student_Test[idx]
                 << "| " << setw(6)  << Student_Exam[idx]
                 << "| " << setw(11) << Student_Assignment[idx]
                 << "| " << setw(6)  << Student_Grade[idx] << "|\n";
        }
        cout << "=============================================================================================================\n";
 
        // post-search menu
        cout << "\n=========================================\n";
        cout << "  1. Update a student from this list\n";
        cout << "  2. Delete a student from this list\n";
        cout << "  3. Search again\n";
        cout << "  4. Exit to Main Menu\n";
        cout << "=========================================\n";
        cout << "Please enter your choice (1-4): ";
 
        int action; // what user wants to do with the search results
        cin >> action;
        cin.ignore();
 
        if (action == 4) return;
 
        if (action == 3) continue;
 
        if (action == 1 || action == 2){
            int pick;
            while(true){
                cout << "Enter student number from the list (1-" << found_count << "): ";
                if (cin >> pick && pick >= 1 && pick <= found_count) break;
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid number, try again.\n";
            }
            cin.ignore();
 
            int target_idx = found_idx[pick - 1]; // actual index in global arrays
 
            if (action == 1) update(target_idx);
            else             delete_student(target_idx);
 
        } else {
            cout << "Wrong input!";
            cin.get();
        }
    }
}

void comp_grade(){
    clear_terminal();
    
    if(act_size==0){
        cout << "No student records to compute.";
        cout << "\nPress ENTER to proceed.";
        cin.ignore(); // delete the buffer input
        cin.get();
        return;
    }

    ofstream gradesF;
    gradesF.open("grades.txt");

    float final_mark = 0;
    
    for (int i=0; i<act_size; i++){
        // calculate overall mark using the given formula
        final_mark = Student_Test[i]*0.3 + Student_Assignment[i]*0.2 + Student_Exam[i]*0.5;
        
        Student_Overall[i] = final_mark; // store for use in statistics later

        // assign grade based on overall mark
        if(final_mark>90){
            Student_Grade[i] = 'A';
        }else if(final_mark>=80){
            Student_Grade[i] = 'B';
        }else if(final_mark>=70){
            Student_Grade[i] = 'C';
        }else if(final_mark>=60){
            Student_Grade[i] = 'D';
        }else if(final_mark>=50){
            Student_Grade[i] = 'E';
        }else if(final_mark<50){
            Student_Grade[i] = 'F';
        }

        gradesF << ID[i] << " " << Student_Name[i]
             << " " << Student_Status[i]
             << " " << Student_Major[i]
             << " " << Student_Test[i]
             << " " << Student_Exam[i]
             << " " << Student_Assignment[i]
             << " " << Student_Grade[i]
             << endl; // write each student on one line
    }
    gradesF.close();
    grade_computed = 1; // mark that grades have been computed
    display_table();
}

void update(int idx){
    clear_terminal();
 
    cout << "=========================================\n";
    cout << "           UPDATE STUDENT INFO           \n";
    cout << "=========================================\n";
    cout << "  Student: " << ID[idx] << " " << Student_Name[idx] << "\n";
    cout << "=========================================\n";
    cout << "  1. Name       : " << Student_Name[idx] << "\n";
    cout << "  2. Status     : " << Student_Status[idx] << "\n";
    cout << "  3. Major      : " << Student_Major[idx] << "\n";
    cout << "  4. Test mark  : " << Student_Test[idx] << "\n";
    cout << "  5. Exam mark  : " << Student_Exam[idx] << "\n";
    cout << "  6. Assignment : " << Student_Assignment[idx] << "\n";
    cout << "  7. Cancel\n";
    cout << "=========================================\n";
 
    int attr_choice;
    // attr_choice input validation
    while (true){ 
        cout << "Choose attribute to update (1-7): ";
        cin >> attr_choice;
        
        if (cin.fail() || attr_choice < 1 || attr_choice > 7) {
            cin.clear();            
            cin.ignore(1000, '\n'); 
            cout << "Invalid choice! Please enter a number between 1 and 7.\n\n";
        } else {
            cin.ignore(); 
            break;        
        }
    }
    // cin >> attr_choice;
    // cin.ignore();
 
    if (attr_choice == 7) return; // user cancelled
 
    if (attr_choice < 1 || attr_choice > 7){ // shouldnt reach here but just in case
        cout << "Wrong input!";
        cin.get();
        return;
    }
 
    if (attr_choice == 1){
        string new_name;
        while(true){
            cout << "Enter new name: ";
            getline(cin, new_name);
            if (is_name_valid(new_name)) break; 
            cout << "Invalid name, try again.\n";
        }
        Student_Name[idx] = new_name;
 
    } else if (attr_choice == 2){
        string new_status;
        while(true){
            cout << "Enter new status (Full Time / Part Time): ";
            getline(cin, new_status);
            if (new_status == "Full Time" || new_status == "Part Time") break;
            cout << "Invalid status. Must be 'Full Time', 'Part Time'.\n";
        }
        Student_Status[idx] = new_status;
 
    } else if (attr_choice == 3){
        string new_major; // will store selected major name
        cout << "Available majors:\n";
        for (int j = 0; j < num_subjects; j++){
            cout << "  " << j+1 << ". " << Student_Subj[j] << "\n";
        }
        int maj_choice;
        while(true){
            cout << "Enter major number (1-9): ";
            if (cin >> maj_choice && maj_choice >= 1 && maj_choice <= num_subjects) break;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid choice, try again.\n";
        }
        cin.ignore();
        Student_Major[idx] = Student_Subj[maj_choice - 1];
 
    } else if (attr_choice == 4){
        float new_mark; // temp variable for new mark input
        while(true){
            cout << "Enter new Test mark (0-100): ";
            if (cin >> new_mark && new_mark >= 0 && new_mark <= 100) break;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid mark. Must be between 0 and 100.\n";
        }
        cin.ignore();
        Student_Test[idx] = new_mark;
 
    } else if (attr_choice == 5){
        float new_mark;
        while(true){
            cout << "Enter new Exam mark (0-100): ";
            if (cin >> new_mark && new_mark >= 0 && new_mark <= 100) break;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid mark. Must be between 0 and 100.\n";
        }
        cin.ignore();
        Student_Exam[idx] = new_mark;
 
    } else if (attr_choice == 6){
        float new_mark;
        while(true){
            cout << "Enter new Assignment mark (0-100): ";
            if (cin >> new_mark && new_mark >= 0 && new_mark <= 100) break;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid mark. Must be between 0 and 100.\n";
        }
        cin.ignore();
        Student_Assignment[idx] = new_mark;
    }
 
    // save change to update.txt
    ofstream updateF;
    updateF.open("update.txt", ios::app);
    updateF << "Updated: " << ID[idx] << " " << Student_Name[idx]
            << " | " << Student_Status[idx]
            << " | " << Student_Major[idx]
            << " | Test:" << Student_Test[idx]
            << " | Exam:" << Student_Exam[idx]
            << " | Assign:" << Student_Assignment[idx] << "\n";
    updateF.close();
 
    save_to_file();
    cout << "\nStudent updated successfully!";
    cout << "\nPress ENTER to proceed.";
    cin.get();
}

void delete_student(int idx){
    clear_terminal();
 
    cout << "=========================================\n";
    cout << "           DELETE STUDENT                \n";
    cout << "=========================================\n";
    cout << "  Are you sure you want to delete:\n";
    cout << "  " << ID[idx] << " " << Student_Name[idx] << " (" << Student_Major[idx] << ")?\n";
    cout << "=========================================\n";
    cout << "  1. Yes, delete\n";
    cout << "  2. Cancel\n";
    cout << "=========================================\n";
    cout << "Enter your choice (1-2): ";
 
    int del_choice;
    cin >> del_choice;
    if (cin.fail()) { // validation for non numeric inpt
        cin.clear();
        cin.ignore(1000, '\n');
        del_choice = 0; 
    }   
    cin.ignore();
 
    if (del_choice == 2){
        cout << "Deletion cancelled.";
        cout << "\nPress ENTER to proceed.";
        cin.get();
        return;
    }

    //save deleted student data to delete.txt
    ofstream deleteF;
    deleteF.open("delete.txt", ios::app);
    deleteF << ID[idx] << " " << Student_Name[idx]
            << "\n" << Student_Status[idx]
            << "\n" << Student_Major[idx]
            << "\n" << Student_Test[idx]
            << "\n" << Student_Exam[idx]
            << "\n" << Student_Assignment[idx] << "\n";
    deleteF.close();

    //shift every student data to the left of array to delete the student
    for (int i = idx; i < act_size - 1; i++){
            ID[i]                 = ID[i+1];
            Student_Name[i]       = Student_Name[i+1];
            Student_Status[i]     = Student_Status[i+1];
            Student_Major[i]      = Student_Major[i+1];
            Student_Test[i]       = Student_Test[i+1];
            Student_Exam[i]       = Student_Exam[i+1];
            Student_Assignment[i] = Student_Assignment[i+1];
            Student_Grade[i]      = Student_Grade[i+1];
            Student_Overall[i]    = Student_Overall[i+1];
    }   
    act_size--; // decremenetthe actual amount of students

    save_to_file(); // delete from results.txt as well
    cout << "\nStudent deleted successfully!";
    cout << "\nPress ENTER to proceed.";
    cin.get();
}

void statistics(){
    clear_terminal();

    if(grade_computed==0){ // check for grades
        cout << "Grades not been computed yet!";
        cout << "\nPress ENTER to proceed.";
        cin.ignore(); // delete the buffer input
        cin.get();
        return;
    }
    
    // count full/part time studs
    int full_part_time[2] = {0};

    //calculate students per grade and passed/failed studs
    int grades_num[6] = {0};
    int pass_fail[2] = {0};

    // highest and lowest overal mark among all stud and track their index
    float highest_overmark = -1;
    float lowest_overmark = 101;
    int highestIndx; // index of student with highest mark
    int lowestIndx;  // index of student with lowest mark

    int subject_count[num_subjects] = {0}; // track students per subj
    float subject_mark_highest[num_subjects];// track highest mark for each subj
    float subject_mark_lowest[num_subjects];// track lowest mark for each subj
    int subHighIndx[num_subjects]; // track index of highest mark student per subj
    int subLowIndx[num_subjects];// track index of lowest mark student per subj
    fill_n(subject_mark_highest, num_subjects, -1);// fill all elements to -1 
    fill_n(subject_mark_lowest, num_subjects, 101);// fill all elements to 101


    for(int i = 0; i<act_size; i++){
        // track full/part time studs
        if (Student_Status[i]=="Full Time") full_part_time[0] += 1;
        else full_part_time[1] += 1;

        if (Student_Grade[i] == "A") grades_num[0]++;
        else if (Student_Grade[i] == "B") grades_num[1]++;
        else if (Student_Grade[i] == "C") grades_num[2]++;
        else if (Student_Grade[i] == "D") grades_num[3]++;
        else if (Student_Grade[i] == "E") grades_num[4]++;
        else if (Student_Grade[i] == "F") grades_num[5]++;

        if(Student_Grade[i]=="F") pass_fail[1]++;
        else pass_fail[0]++;

        // Highest/Lowest overall
        if (Student_Overall[i] >= highest_overmark) {
            highest_overmark = Student_Overall[i];
            highestIndx = i;
        }
        if (Student_Overall[i] <= lowest_overmark) {
            lowest_overmark = Student_Overall[i];
            lowestIndx = i;
        }

        // students per subject
        if(Student_Major[i]=="Mathematics") subject_count[0]++;
        else if(Student_Major[i]=="English") subject_count[1]++;
        else if(Student_Major[i]=="Science") subject_count[2]++;
        else if(Student_Major[i]=="History") subject_count[3]++;
        else if(Student_Major[i]=="Computer Science") subject_count[4]++;
        else if(Student_Major[i]=="Biology") subject_count[5]++;
        else if(Student_Major[i]=="Chemistry") subject_count[6]++;
        else if(Student_Major[i]=="Physics") subject_count[7]++;
        else if(Student_Major[i]=="Geography") subject_count[8]++;

        int subIndx;// index of student subj in subject array
        // which index does students major belongs to in subject array
        for (int j = 0; j < num_subjects; j++) {
            if (Student_Major[i] == Student_Subj[j]) {
                subIndx = j;
                break;
            }
        }
        // highest mark per subject
        if (Student_Overall[i] >= subject_mark_highest[subIndx]){
            subject_mark_highest[subIndx] = Student_Overall[i];
            subHighIndx[subIndx] = i;
        }
        //lowest mark per subject
        if (Student_Overall[i] <= subject_mark_lowest[subIndx]){
            subject_mark_lowest[subIndx] = Student_Overall[i];
            subLowIndx[subIndx] = i;
        }
    }
    
    // Display 
    cout << "\n======== STATISTICS ========\n\n";
 
    cout << "-- Grade Distribution --\n";
    string grades[] = {"A", "B", "C", "D", "E", "F"};

    for (int i = 0; i < 6; i++) {
        double pct = (double)grades_num[i] / act_size * 100.0; // calculate percentage
        cout << "  Grade " << grades[i] << ": " << grades_num[i]
             << " students (" << fixed << setprecision(0) << pct << "%)\n";
    }
 
    cout << "\n-- Pass / Fail --\n";
    cout << "  Pass : " << pass_fail[0] << " students ("
         << fixed << setprecision(0) << (double)pass_fail[0] / act_size * 100.0 << "%)\n";
    cout << "  Fail : " << pass_fail[1] << " students ("
         << fixed << setprecision(0) << (double)pass_fail[1] / act_size * 100.0 << "%)\n";
 
    cout << "\n-- Highest & Lowest Overall Mark --\n";
    cout << "  Highest: " << ID[highestIndx] << " " << Student_Name[highestIndx]
         << " (" << fixed << setprecision(2) << highest_overmark << ")\n";
    cout << "  Lowest : " << ID[lowestIndx]  << " " << Student_Name[lowestIndx]
         << " (" << fixed << setprecision(2) << lowest_overmark  << ")\n";
 
    cout << "\n-- Students per Subject --\n";
    for (int j = 0; j < num_subjects; j++) {
        cout << "  " << left << setw(20) << Student_Subj[j] << ": " << subject_count[j] << " student(s)\n";
    }
 
    cout << "\n-- Full Time vs Part Time --\n";
    cout << "  Full Time: " << full_part_time[0] << " students\n";
    cout << "  Part Time: " << full_part_time[1] << " students\n";
 
    cout << "\n-- Highest & Lowest per Subject --\n";
    for (int j = 0; j < num_subjects; j++) {
        cout << "  " << Student_Subj[j] << ":\n";
        cout << "    Highest: " << ID[subHighIndx[j]] << " " << Student_Name[subHighIndx[j]]
             << " (" << fixed << setprecision(2) << subject_mark_highest[j]<< ")\n";
        cout << "    Lowest : " << ID[subLowIndx[j]]  << " " << Student_Name[subLowIndx[j]]
             << " (" << fixed << setprecision(2) << subject_mark_lowest[j]  << ")\n";
    }

    // save to statistics.txt
    ofstream fout;
    fout.open("statistics.txt");

    if (fout) { // check file opened succesfully before writing
        fout << "======== STATISTICS ========\n\n";
        fout << "-- Grade Distribution --\n";
        for (int i = 0; i < 6; i++) {
            double pct = (double)grades_num[i] / act_size * 100.0; // percentage for each grade
            fout << "  Grade " << grades[i] << ": " << grades_num[i]
                 << " students (" << fixed << setprecision(0) << pct << "%)\n";
        }
        fout << "\n-- Pass / Fail --\n";
        fout << "  Pass : " << pass_fail[0] << " students ("
             << fixed << setprecision(0) << (double) pass_fail[0] / act_size * 100.0 << "%)\n";
        fout << "  Fail : " << pass_fail[1] << " students ("
             << fixed << setprecision(0) << (double)pass_fail[1] / act_size * 100.0 << "%)\n";
        fout << "\n-- Highest & Lowest Overall Mark --\n";
        fout << "  Highest: " << ID[highestIndx] << " " << Student_Name[highestIndx]
             << " (" << fixed << setprecision(2) << highest_overmark << ")\n";
        fout << "  Lowest : " << ID[lowestIndx]  << " " << Student_Name[lowestIndx]
             << " (" << fixed << setprecision(2) << lowest_overmark  << ")\n";
        fout << "\n-- Students per Subject --\n";
        for (int j = 0; j < num_subjects; j++) {
            fout << "  " << left << setw(20) << Student_Subj[j] << ": " << subject_count[j] << " student(s)\n";
        }
        fout << "\n-- Full Time vs Part Time --\n";
        fout << "  Full Time: " << full_part_time[0] << " students\n";
        fout << "  Part Time: " << full_part_time[1] << " students\n";
        fout << "\n-- Highest & Lowest per Subject --\n";
        for (int j = 0; j < num_subjects; j++) {
            fout << "  " << Student_Subj[j] << ":\n";
            fout << "    Highest: " << ID[subHighIndx[j]] << " " << Student_Name[subHighIndx[j]]
                 << " (" << fixed << setprecision(2) << subject_mark_highest[j] << ")\n";
            fout << "    Lowest : " << ID[subLowIndx[j]]  << " " << Student_Name[subLowIndx[j]]
                 << " (" << fixed << setprecision(2) << subject_mark_lowest[j]  << ")\n";
        }
    }
    fout.close();

    cout << "\n  Statistics executed and saved to statistics.txt.\n";
    cout << "\nPress ENTER to proceed.";
    cin.ignore();
    cin.get();
}
