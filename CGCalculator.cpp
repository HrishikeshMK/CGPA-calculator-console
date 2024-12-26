#include <fstream>
#include <bits/stdc++.h>
#include <getopt.h>
#include <io.h>
#include <wchar.h>
using namespace std;
//Course structure
struct Course {
    string name;
    int credits;
    int gradeInCourse;
};

//Student structure
struct Student {
    string name;
    string ID;
    int totCredits=0;
};

// Function prototypes
void addCourse(vector<Course>& courses);
void addStudent(vector<Student>& student);
void loadCourses(vector<Course>& courses, string id);
void viewCourses(const vector<Course>& courses);
void loadStudents(vector<Student>& students);
void loadSelectedStudentsCourse(const Student& student, vector<Course>& selectedStudentsCourses);
void selectCourse(Student& student, const vector<Course>& courses, Course& currcourse, vector<Course>& selectedStudentsCourses);
void selectStudent(Student& student, vector<Student>& students, vector<Course>& courses);
void saveCourses(vector<Course>& courses, string id);
void saveStudent(vector<Student>& students);
void saveSelectedStudentsCourse(const Student& student, vector<Course>& selectedStudentsCourses);
void calculateCGPA(Student& student,const vector<Course>& selectedStudentsCourses);
void printselectedStudentsCourses(const vector<Course>& selectedStudentsCourses);
void deleteoreditselectedStudentsCourse(vector<Course>&selectedStudentsCourses, Student& student);

int main() {
    vector<Course> courses;
    vector<Student> students;
    Student student;
    
    loadStudents(students);
    
    int choice;
    string s;
    while (true) {
        cout << "1. Add a new course\n";
        cout << "2. View existing courses in database\n";
        cout << "3. Add a new student\n";
        cout << "4. Select a student\n";
        cout << "5. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cout << "\n";
        switch (choice) {
            case 1:
                cout<<"Enter the Branch ID (Common, Huel, AA etc): ";
                cin>>s;
                loadCourses(courses, s);
                addCourse(courses);
                saveCourses(courses, s);
                courses.clear();
                break;
            case 2:
                cout<<"Enter the Branch ID (AA - ECE): ";
                cin>>s;
                loadCourses(courses, s);
                viewCourses(courses);
                courses.clear();
                break;
            case 3:
                addStudent(students);
                saveStudent(students);
                break;
            case 4:
                selectStudent(student, students, courses);
                // saveStudent(students);
                break;
            case 5:
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n\n";
                break;
        }
    }
    return 0;
}

//add a new course
void addCourse(vector<Course>& courses) {
    Course newCourse;
    cout << "Enter course name: ";
    cin.ignore();
    getline(cin, newCourse.name);
    cout << "Enter number of credits: ";
    cin >> newCourse.credits;
    courses.emplace_back(newCourse);
    cout << "Course added successfully.";
    cout<<"\n______________________________\n\n";
}
//add a new Student
void addStudent(vector<Student>& students){
    Student newStudent;
    cout<<"Enter your name: ";
    cin.ignore();
    getline(cin, newStudent.name);
    
    cout<< "Enter your ID: ";
    cin>>newStudent.ID;

    students.emplace_back(newStudent);
    cout<< "Student added successfully.";
    cout<<"\n______________________________\n\n";
}

//save student details to a file
void saveStudent(vector<Student>& students){
    ofstream outFile("data/students/students.txt");
    if (!outFile) {
        cerr << "Error opening file for writing: " << "students.txt" << endl;
        return;
    }
    
    for (const auto& student: students){
        outFile<< student.name<< '\n' << student.ID << '\n' << student.totCredits<< '\n';
    }
    outFile.close();
}
//save courses to a file
void saveCourses(vector<Course>& courses, string id) {
    ofstream outFile("data/courses/" + id +"courses.txt");
    if (!outFile) {
        addCourse(courses);
        return;
    }
    for (const auto& course : courses) {
        outFile << course.name << '\n' << course.credits << '\n';
    }
    outFile.close();
}
//save the Course details of a student
void saveSelectedStudentsCourse(const Student& student, vector<Course>& selectedStudentsCourses){
    ofstream out("data/students/" + student.ID + "selected_course.txt");
    if (!out) {
        cerr << "Error opening file for writing: " <<student.ID << "selected_course.txt" << endl;
        return;
    }
    for(auto course: selectedStudentsCourses){
        out<<course.name<<'\n';
        out<<course.credits<<'\n';
        out<<course.gradeInCourse<<'\n';
    }
    out.close();
}

//load students from a file
void loadStudents(vector<Student>& students){
    ifstream inFile("data/students/students.txt");
    if (!inFile) {
        cerr << "Error opening file for reading: " <<"students.txt" << endl;
        return;
    }

    Student student;
    while (getline(inFile, student.name)) {
        inFile >> student.ID;
        inFile.ignore(); // Ignore the newline
        inFile >> student.totCredits;
        inFile.ignore();
        students.emplace_back(student);
    }
    inFile.close();
}
//load courses from a file
void loadCourses(vector<Course>& courses, string id) {
    ifstream inFile("data/courses/" + id + "courses.txt");
    if (!inFile) {
        cerr << "Error opening file for reading: " <<"data.txt" << endl;
        return;
    }
    
    Course course;
    while (getline(inFile, course.name)) {
        inFile >> course.credits;
        inFile.ignore(); // Ignore the newline character after the credits
        courses.emplace_back(course);
    }
    inFile.close();
}
//load courses (and its details) taken by a student from a file
void loadSelectedStudentsCourse(const Student& student, vector<Course>& selectedStudentsCourses ){
    ifstream in("data/students/" + student.ID + "selected_course.txt");
     if (!in) {
        cerr << "Error opening file for reading: " <<student.ID<<"selected_course.txt" << endl;
        return;
    }
    Course newCourse;
    while(getline(in, newCourse.name)){
        string s;
        in >> s;
        newCourse.credits = stoi(s);
        in.ignore();
        string s1;
        in >> s1;
        newCourse.gradeInCourse = stoi(s1);
        in.ignore();
        selectedStudentsCourses.emplace_back(newCourse);
    }
    in.close();
}

//view list of courses in the data base
void viewCourses(const vector<Course>& courses){
    if (courses.empty()) {
        cout << "Add a new course to get started.\n";
        return;
    }
    cout << "Available courses:\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        cout << i + 1 << ". " << courses[i].name << " (" << courses[i].credits << " credits)\n";
    }
    cout<< "______________________________\n\n";
}

//select a course for the student
void selectCourse(Student& student, const vector<Course>& courses, Course& currcourse, vector<Course>& selectedStudentsCourses){
    if (courses.empty()) {
        cout << "Add a new course to get started.\n";
        return;
    }
    cout << "Available courses:\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        cout << i + 1 << ". " << courses[i].name << " (" << courses[i].credits << " credits)\n";
    }
    size_t choice;
    cout << "\nEnter the number of the course to select: ";
    cin >> choice;
    if (choice < 1 || choice > courses.size()) {
        cout << "Invalid choice.\n";
    }else{
        currcourse.name = courses[choice - 1].name;
        currcourse.credits = courses[choice - 1].credits;
        cout << "\nCourse selected: " << currcourse.name << '\n';
        student.totCredits += currcourse.credits;
        bool flag = true;
        int Grade;
        while(flag){
            cout<<"\nEnter your grade in " << currcourse.name <<" (out of 10): ";
            cin>> Grade;
            if(Grade>10||Grade<0){
                cout<<"Invalid Grade! Try Again\n";
            }else{
                flag = false;
                currcourse.gradeInCourse=Grade;
            }
        }
        selectedStudentsCourses.emplace_back(currcourse);
    }
}
//select student from list
void selectStudent(Student& student, vector<Student>& students, vector<Course>& courses){
    if (students.empty()) {
        cout << "No students available to select.\n";
        return;
    }
    cout << "Available students:\n";
    for (size_t i = 0; i < students.size(); ++i) {
        cout << i + 1 << ". " << students[i].name << " (" << students[i].ID << " -ID)\n";
    }
    cout<<"______________________________\n";
    size_t ch; //ch -1 is the index of student
    bool flag = true;
    string br;
    while (flag){
        cout << "\nEnter the number corresponding to your Name and ID : ";
        cin >> ch;
        if (ch < 1 || ch > students.size()) {
            cout << "Invalid choice.\n\n";
        } else {
            cout << "Your ID is: " << students[ch-1].ID << endl;
            cout << "Continue? (Y/N) ";
            char in;
            cin>> in;
            if(in=='Y'||in=='y'){
                student = students[ch-1];//this one can be removed 
                flag = false;
                br = student.ID.substr(4,2);
            }
        }
    }
    int in;
    flag = true;
    while(flag){
        cout<<"______________________________\n\n";
        cout << "1. Add a CDC\n";
        cout << "2. Add a HUEL\n";
        cout << "3. Add common courses (First Year, PS)\n";
        cout << "4. Add other branch courses\n";
        cout << "5. Edit an existing course\n";
        cout << "6. View total credits taken\n";
        cout << "7. Calculate CGPA\n";
        cout<< "8. View completed courses\n";
        cout<< "9. Exit\n\n";
        cout << "Enter choice: ";
        cin >> in;
        cout <<'\n';
        Course c;
        vector<Course> selectedStudentCourses;
        loadSelectedStudentsCourse(student, selectedStudentCourses);
        
        switch(in){
            case 1:
                loadCourses(courses, br);
                selectCourse(student, courses, c, selectedStudentCourses);
                saveSelectedStudentsCourse(student, selectedStudentCourses);
                students[ch -1] = student; // work on removing student
                saveStudent(students);
                courses.clear();
                break;
            case 2:
                loadCourses(courses, "Huel");
                selectCourse(student, courses, c, selectedStudentCourses);
                saveSelectedStudentsCourse(student, selectedStudentCourses);
                students[ch -1] = student; // work on removing student
                saveStudent(students);
                courses.clear();
                break;
            case 3:
                loadCourses(courses, "Common");
                selectCourse(student, courses, c, selectedStudentCourses);
                saveSelectedStudentsCourse(student, selectedStudentCourses);
                students[ch -1] = student; // work on removing student
                saveStudent(students);
                courses.clear();
                break;
            case 4:
                cout<<"Enter course Branch code (AA, A7 etc):";
                cin>>br;
                loadCourses(courses, br);
                selectCourse(student, courses, c, selectedStudentCourses);
                saveSelectedStudentsCourse(student, selectedStudentCourses);
                students[ch -1] = student; // work on removing student
                saveStudent(students);
                courses.clear();
                break;
            case 5:
                deleteoreditselectedStudentsCourse(selectedStudentCourses, student);
                saveSelectedStudentsCourse(student, selectedStudentCourses);
                break;
            case 6:
                cout<<"Total credits taken: " << student.totCredits <<"\n";
                break;
            case 7:
                calculateCGPA(student,selectedStudentCourses);
                break;
            case 8:
                cout << "Courses Completed by " << student.name << ": \n";
                printselectedStudentsCourses(selectedStudentCourses);
                break;
            case 9:
                flag = false;
                cout<<"\n";
                break;
            default: 
                cout<<"Enter a valid choice!\n";
                break;
        }
    }
}

//Function to calculate CGPA of a given student
void calculateCGPA(Student& student,const vector<Course>& selectedStudentsCourses){
    float cgpa  = 0;
    for(auto course: selectedStudentsCourses){
        cgpa += course.credits * course.gradeInCourse;
    }
    cgpa /= student.totCredits;
    cout << "Your CGPA is: " << cgpa <<'\n';
}

//Function to print a given Students Courses
void printselectedStudentsCourses(const vector<Course>& selectedStudentsCourses){
    int i = 0;
    for(auto course: selectedStudentsCourses){
        cout<<++i<<". "<<course.name<<'\n';
    }
}

//deletes or edits the selected Students Course
void deleteoreditselectedStudentsCourse(vector<Course>& selectedStudentsCourses, Student& student){
    int c=0; 
    string courseName;
    cout<<"Enter Course Name to modify: \n";
    cin.ignore();
    getline(cin, courseName);
    for(auto course: selectedStudentsCourses){
        if(course.name == courseName){
            cout<<"1. Delete Course: "<<courseName<<endl;
            cout<<"2. Edit Course: "<<courseName<<endl;
            int j;
            int grade;
            cout<<"Enter the number corresponding to your choice: ";
            cin>>j;
            switch(j){
                case 1:
                    cout<<"This action can't be undone. You sure to proceed? ";
                    char s;
                    cin>>s;
                    if(s == 'y'|| 'Y'){
                        student.totCredits -= course.credits;
                        selectedStudentsCourses.erase(selectedStudentsCourses.begin()+c);
                        cout<<"Course has been removed from student profile.\n";   
                    }
                    break;
                case 2:
                    cout<<"Enter the correct grade (out of 10) in the course: ";
                    cin>>grade;
                    selectedStudentsCourses[c].gradeInCourse = grade;
                    cout<<"Grade for course "<<selectedStudentsCourses[c].name<<" has been modified.\n";
                    break;
            }
            return;
        }
        c++;
    }
    cout<<"The course is not present, please check your input! \n";
    deleteoreditselectedStudentsCourse(selectedStudentsCourses, student);
}

//Next up:
// Create password for accessing users. Use Hashmap for storing password
// Remove or edit Courses
//Improve the readability
//Redundancy checker for all files. Check redundancy before pushback operations into three vectors, student, courses, selectedstudentcourses
