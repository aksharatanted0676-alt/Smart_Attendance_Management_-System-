#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Student {
public:
    int id;
    string name;

    Student(int i, string n) {
        id = i;
        name = n;
    }
};

class Course {
public:
    int id;
    string name;

    Course(int i, string n) {
        id = i;
        name = n;
    }
};

class Attendance {
public:
    int studentId;
    int courseId;
    string date;
    bool present;

    Attendance(int s, int c, string d, bool p) {
        studentId = s;
        courseId = c;
        date = d;
        present = p;
    }
};

vector<Student> students;
vector<Course> courses;
vector<Attendance> records;

void saveStudents() {

    ofstream file("students.txt");

    for (auto s : students)
        file << s.id << " " << s.name << endl;

    file.close();
}

void loadStudents() {

    ifstream file("students.txt");

    if(!file)
        return;

    int id;
    string name;

    while(file >> id) {
        file.ignore();
        getline(file,name);
        students.push_back(Student(id,name));
    }

    file.close();
}

void saveCourses() {

    ofstream file("courses.txt");

    for (auto c : courses)
        file << c.id << " " << c.name << endl;

    file.close();
}

void loadCourses() {

    ifstream file("courses.txt");

    if(!file)
        return;

    int id;
    string name;

    while(file >> id) {
        file.ignore();
        getline(file,name);
        courses.push_back(Course(id,name));
    }

    file.close();
}

void saveAttendance() {

    ofstream file("attendance.txt");

    for (auto a : records) {

        file << a.studentId << " "
             << a.courseId << " "
             << a.date << " "
             << a.present << endl;
    }

    file.close();
}

void loadAttendance() {

    ifstream file("attendance.txt");

    if(!file)
        return;

    int sid,cid,p;
    string date;

    while(file >> sid >> cid >> date >> p) {

        records.push_back(
            Attendance(sid,cid,date,p)
        );
    }

    file.close();
}

string getStudentName(int id) {

    for (auto s : students)
        if (s.id == id)
            return s.name;

    return "Unknown";
}

string getCourseName(int id) {

    for (auto c : courses)
        if (c.id == id)
            return c.name;

    return "Unknown";
}

void addStudent() {

    int id;
    string name;

    cout << "Enter Student ID: ";
    cin >> id;

    for (auto s : students) {

        if (s.id == id) {
            cout << "Duplicate ID!\n";
            return;
        }
    }

    cin.ignore();

    cout << "Enter Student Name: ";
    getline(cin,name);

    students.push_back(Student(id,name));

    saveStudents();

    cout << "Student Added Successfully\n";
}

void addCourse() {

    int id;
    string name;

    cout << "Enter Course ID: ";
    cin >> id;

    cin.ignore();

    cout << "Enter Course Name: ";
    getline(cin,name);

    courses.push_back(Course(id,name));

    saveCourses();

    cout << "Course Added Successfully\n";
}

void markAttendance() {

    int sid,cid;
    string date;
    char status;

    cout << "Enter Student ID: ";
    cin >> sid;

    cout << "Enter Course ID: ";
    cin >> cid;

    cout << "Enter Date (YYYY-MM-DD): ";
    cin >> date;

    cout << "Present (P) / Absent (A): ";
    cin >> status;

    bool present = (status=='P' || status=='p');

    records.push_back(
        Attendance(sid,cid,date,present)
    );

    saveAttendance();

    cout << "Attendance Recorded\n";
}

void studentReport() {

    int sid;

    cout << "Enter Student ID: ";
    cin >> sid;

    cout << "\nStudent Name: "
         << getStudentName(sid) << endl;

    vector<int> courseIds;
    vector<int> totalCourse;
    vector<int> presentCourse;

    vector<string> months;
    vector<int> totalMonth;
    vector<int> presentMonth;

    int totalTotal = 0;
    int presentTotal = 0;

    for (auto a : records) {

        if (a.studentId == sid) {

            int index = -1;

            for (int i = 0; i < courseIds.size(); i++) {
                if (courseIds[i] == a.courseId)
                    index = i;
            }

            if (index == -1) {
                courseIds.push_back(a.courseId);
                totalCourse.push_back(0);
                presentCourse.push_back(0);
                index = courseIds.size() - 1;
            }

            totalCourse[index]++;

            if (a.present)
                presentCourse[index]++;

            string month = a.date.substr(0,7);

            int mindex = -1;

            for (int i = 0; i < months.size(); i++) {
                if (months[i] == month)
                    mindex = i;
            }

            if (mindex == -1) {
                months.push_back(month);
                totalMonth.push_back(0);
                presentMonth.push_back(0);
                mindex = months.size() - 1;
            }

            totalMonth[mindex]++;

            if (a.present)
                presentMonth[mindex]++;

            totalTotal++;

            if (a.present)
                presentTotal++;
        }
    }

    cout << "\nAttendance % By Course\n";

    for (int i = 0; i < courseIds.size(); i++) {

        double percent = 0;

        if(totalCourse[i]!=0)
            percent =
            (presentCourse[i]*100.0)
            /totalCourse[i];

        cout << getCourseName(courseIds[i])
             << " : "
             << percent
             << "%\n";
    }


    cout << "\nAttendance % By Month\n";

    for (int i = 0; i < months.size(); i++) {

        double percent = 0;

        if(totalMonth[i]!=0)
            percent =
            (presentMonth[i]*100.0)
            /totalMonth[i];

        cout << months[i]
             << " : "
             << percent
             << "%\n";
    }

    double overall = 0;

    if(totalTotal!=0)
        overall =
        (presentTotal*100.0)
        /totalTotal;

    cout << "\nOverall Attendance: "
         << overall << "%\n";

    if(overall < 75)
        cout << "Warning: Student is a Defaulter\n";
}

void mostAbsentStudents() {

    vector<int> studentIds;
    vector<int> absentCount;

    for (auto a : records) {

        if(!a.present) {

            int index = -1;

            for (int i = 0; i < studentIds.size(); i++) {
                if (studentIds[i] == a.studentId)
                    index = i;
            }

            if (index == -1) {
                studentIds.push_back(a.studentId);
                absentCount.push_back(0);
                index = studentIds.size() - 1;
            }

            absentCount[index]++;
        }
    }

    int maxAbsent = 0;

    for (int i = 0; i < absentCount.size(); i++)
        if(absentCount[i] > maxAbsent)
            maxAbsent = absentCount[i];

    cout << "\nMost Absent Students\n";

    for (int i = 0; i < studentIds.size(); i++) {

        if (absentCount[i] == maxAbsent) {

            cout << getStudentName(studentIds[i])
                 << " (ID: "
                 << studentIds[i]
                 << ") "
                 << absentCount[i]
                 << " absences\n";
        }
    }
}

void highestAbsentDay() {

    vector<string> days;
    vector<int> absentCount;

    for (auto a : records) {

        if(!a.present) {

            int index = -1;

            for (int i = 0; i < days.size(); i++) {
                if (days[i] == a.date)
                    index = i;
            }

            if (index == -1) {
                days.push_back(a.date);
                absentCount.push_back(0);
                index = days.size() - 1;
            }

            absentCount[index]++;
        }
    }

    int maxAbsent = 0;
    string day;

    for (int i = 0; i < days.size(); i++) {

        if(absentCount[i] > maxAbsent) {

            maxAbsent = absentCount[i];
            day = days[i];
        }
    }

    cout << "\nDay With Highest Absentees\n";

    if(maxAbsent > 0)

        cout << day
             << " : "
             << maxAbsent
             << " absences\n";

    else
        cout << "No absences recorded\n";
}
void removeStudent() {

    int id;
    cout << "Enter Student ID to Remove: ";
    cin >> id;

    bool found = false;

    // Remove student from students vector
    for (int i = 0; i < students.size(); i++) {

        if (students[i].id == id) {

            students.erase(students.begin() + i);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Student Not Found\n";
        return;
    }

    // Remove related attendance records
    for (int i = 0; i < records.size(); ) {

        if (records[i].studentId == id)
            records.erase(records.begin() + i);
        else
            i++;
    }

    saveStudents();
    saveAttendance();

    cout << "Student Removed Successfully\n";
}

int main() {

    loadStudents();
    loadCourses();
    loadAttendance();

    int choice;

    do {

        cout << "\n------ Smart Attendance System ------\n";

        cout << "1. Add Student\n";
cout << "2. Add Course\n";
cout << "3. Mark Attendance\n";
cout << "4. Student Report\n";
cout << "5. Most Absent Students\n";
cout << "6. Highest Absent Day\n";
cout << "7. Remove Student\n";
cout << "8. Exit\n";

        cout << "Enter Choice: ";
        cin >> choice;

        switch(choice) {

            case 1:
                addStudent();
                break;

            case 2:
                addCourse();
                break;

            case 3:
                markAttendance();
                break;

            case 4:
                studentReport();
                break;

            case 5:
                mostAbsentStudents();
                break;

            case 6:
                highestAbsentDay();
                break;

            case 7:
    removeStudent();
    break;

case 8:
    cout << "Exiting Program\n";
    break;


            default:
                cout << "Invalid Choice\n";
        }

    } while(choice != 7);

    return 0;
}