#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include<bits/stdc++.h>


class Student {
private:
    std::string name;
    int rollNumber;
    std::vector<float> subjects;
    float totalMarks;
    float percentage;
    int rank;
    char grade;

public:
    // Constructor
    Student(std::string n, int roll, std::vector<float> marks) 
        : name(n), rollNumber(roll), subjects(marks), rank(0) {
        calculateTotalAndPercentage();
        calculateGrade();
    }

    // Calculate total marks and percentage
    void calculateTotalAndPercentage() {
        totalMarks = 0;
        for (float mark : subjects) {
            totalMarks += mark;
        }
        percentage = (totalMarks / (subjects.size() * 100)) * 100;
    }

    // Calculate grade based on percentage
    void calculateGrade() {
        if (percentage >= 90) grade = 'A';
        else if (percentage >= 80) grade = 'B';
        else if (percentage >= 70) grade = 'C';
        else if (percentage >= 60) grade = 'D';
        else if (percentage >= 40) grade = 'E';
        else grade = 'F';
    }

    // Getters
    std::string getName() const { return name; }
    int getRollNumber() const { return rollNumber; }
    float getTotalMarks() const { return totalMarks; }
    float getPercentage() const { return percentage; }
    int getRank() const { return rank; }
    char getGrade() const { return grade; }
    std::vector<float> getSubjects() const { return subjects; }

    // Setters
    void setRank(int r) { rank = r; }
    void setName(const std::string& n) { name = n; }
    void setSubjects(const std::vector<float>& marks) { 
        subjects = marks; 
        calculateTotalAndPercentage();
        calculateGrade();
    }

    // Display student information
    void displayInfo() const {
        std::cout << std::setw(5) << rollNumber 
                  << std::setw(20) << name 
                  << std::setw(10) << std::fixed << std::setprecision(2) << totalMarks
                  << std::setw(12) << percentage << "%"
                  << std::setw(8) << grade
                  << std::setw(8) << rank << std::endl;
    }

    // For sorting by percentage (descending)
    bool operator>(const Student& other) const {
        return percentage > other.percentage;
    }
};

class StudentRankManager {
private:
    std::vector<Student> students;
    std::vector<std::string> subjectNames;

public:
    // Constructor
    StudentRankManager() {
        subjectNames = {"Math", "Physics", "Chemistry", "English", "Biology"};
    }

    // Set custom subject names
    void setSubjectNames(const std::vector<std::string>& subjects) {
        subjectNames = subjects;
    }

    // Add a new student
    void addStudent() {
        std::string name;
        int rollNumber;
        std::vector<float> marks;

        std::cout << "\n--- Add New Student ---\n";
        std::cout << "Enter student name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        
        std::cout << "Enter roll number: ";
        std::cin >> rollNumber;

        // Check if roll number already exists
        for (const auto& student : students) {
            if (student.getRollNumber() == rollNumber) {
                std::cout << "Error: Roll number " << rollNumber << " already exists!\n";
                return;
            }
        }

        std::cout << "Enter marks for " << subjectNames.size() << " subjects:\n";
        for (size_t i = 0; i < subjectNames.size(); ++i) {
            float mark;
            std::cout << subjectNames[i] << " (0-100): ";
            std::cin >> mark;
            
            // Validate marks
            while (mark < 0 || mark > 100) {
                std::cout << "Invalid mark! Please enter marks between 0-100: ";
                std::cin >> mark;
            }
            marks.push_back(mark);
        }

        students.emplace_back(name, rollNumber, marks);
        std::cout << "Student added successfully!\n";
        calculateRanks();
    }

    // Remove a student by roll number
    void removeStudent() {
        if (students.empty()) {
            std::cout << "No students in the database!\n";
            return;
        }

        int rollNumber;
        std::cout << "Enter roll number to remove: ";
        std::cin >> rollNumber;

        auto it = std::find_if(students.begin(), students.end(),
                              [rollNumber](const Student& s) {
                                  return s.getRollNumber() == rollNumber;
                              });

        if (it != students.end()) {
            std::cout << "Removing student: " << it->getName() << std::endl;
            students.erase(it);
            calculateRanks();
            std::cout << "Student removed successfully!\n";
        } else {
            std::cout << "Student with roll number " << rollNumber << " not found!\n";
        }
    }

    // Calculate ranks for all students
    void calculateRanks() {
        if (students.empty()) return;

        // Sort students by percentage in descending order
        std::sort(students.begin(), students.end(), std::greater<Student>());

        // Assign ranks
        for (size_t i = 0; i < students.size(); ++i) {
            students[i].setRank(i + 1);
        }
    }

    // Display all students
    void displayAllStudents() const {
        if (students.empty()) {
            std::cout << "No students in the database!\n";
            return;
        }

        std::cout << "\n--- Student Ranking List ---\n";
        std::cout << std::string(75, '-') << std::endl;
        std::cout << std::setw(5) << "Roll" 
                  << std::setw(20) << "Name" 
                  << std::setw(10) << "Total"
                  << std::setw(12) << "Percentage"
                  << std::setw(8) << "Grade"
                  << std::setw(8) << "Rank" << std::endl;
        std::cout << std::string(75, '-') << std::endl;

        for (const auto& student : students) {
            student.displayInfo();
        }
        std::cout << std::string(75, '-') << std::endl;
    }

    // Search student by roll number
    void searchStudent() const {
        if (students.empty()) {
            std::cout << "No students in the database!\n";
            return;
        }

        int rollNumber;
        std::cout << "Enter roll number to search: ";
        std::cin >> rollNumber;

        auto it = std::find_if(students.begin(), students.end(),
                              [rollNumber](const Student& s) {
                                  return s.getRollNumber() == rollNumber;
                              });

        if (it != students.end()) {
            std::cout << "\n--- Student Details ---\n";
            std::cout << "Name: " << it->getName() << std::endl;
            std::cout << "Roll Number: " << it->getRollNumber() << std::endl;
            std::cout << "Subject-wise Marks:\n";
            
            auto marks = it->getSubjects();
            for (size_t i = 0; i < marks.size() && i < subjectNames.size(); ++i) {
                std::cout << "  " << subjectNames[i] << ": " << marks[i] << std::endl;
            }
            
            std::cout << "Total Marks: " << it->getTotalMarks() << std::endl;
            std::cout << "Percentage: " << std::fixed << std::setprecision(2) 
                      << it->getPercentage() << "%" << std::endl;
            std::cout << "Grade: " << it->getGrade() << std::endl;
            std::cout << "Rank: " << it->getRank() << std::endl;
        } else {
            std::cout << "Student with roll number " << rollNumber << " not found!\n";
        }
    }

    // Display top N students
    void displayTopStudents() const {
        if (students.empty()) {
            std::cout << "No students in the database!\n";
            return;
        }

        int n;
        std::cout << "Enter number of top students to display: ";
        std::cin >> n;

        if (n <= 0) {
            std::cout << "Invalid number!\n";
            return;
        }

        std::cout << "\n--- Top " << std::min(n, (int)students.size()) << " Students ---\n";
        std::cout << std::string(75, '-') << std::endl;
        std::cout << std::setw(5) << "Roll" 
                  << std::setw(20) << "Name" 
                  << std::setw(10) << "Total"
                  << std::setw(12) << "Percentage"
                  << std::setw(8) << "Grade"
                  << std::setw(8) << "Rank" << std::endl;
        std::cout << std::string(75, '-') << std::endl;

        for (int i = 0; i < std::min(n, (int)students.size()); ++i) {
            students[i].displayInfo();
        }
        std::cout << std::string(75, '-') << std::endl;
    }

    // Update student marks
    void updateStudent() {
        if (students.empty()) {
            std::cout << "No students in the database!\n";
            return;
        }

        int rollNumber;
        std::cout << "Enter roll number to update: ";
        std::cin >> rollNumber;

        auto it = std::find_if(students.begin(), students.end(),
                              [rollNumber](const Student& s) {
                                  return s.getRollNumber() == rollNumber;
                              });

        if (it != students.end()) {
            std::cout << "Updating marks for: " << it->getName() << std::endl;
            std::vector<float> newMarks;
            
            for (size_t i = 0; i < subjectNames.size(); ++i) {
                float mark;
                std::cout << subjectNames[i] << " (current: " 
                          << it->getSubjects()[i] << "): ";
                std::cin >> mark;
                
                while (mark < 0 || mark > 100) {
                    std::cout << "Invalid mark! Please enter marks between 0-100: ";
                    std::cin >> mark;
                }
                newMarks.push_back(mark);
            }
            
            it->setSubjects(newMarks);
            calculateRanks();
            std::cout << "Student marks updated successfully!\n";
        } else {
            std::cout << "Student with roll number " << rollNumber << " not found!\n";
        }
    }

    // Save data to file
    void saveToFile() const {
        std::ofstream file("student_data.txt");
        if (!file.is_open()) {
            std::cout << "Error opening file for writing!\n";
            return;
        }

        file << students.size() << std::endl;
        for (const auto& student : students) {
            file << student.getRollNumber() << std::endl;
            file << student.getName() << std::endl;
            auto marks = student.getSubjects();
            for (float mark : marks) {
                file << mark << " ";
            }
            file << std::endl;
        }
        file.close();
        std::cout << "Data saved to student_data.txt successfully!\n";
    }

    // Load data from file
    void loadFromFile() {
        std::ifstream file("student_data.txt");
        if (!file.is_open()) {
            std::cout << "No previous data file found.\n";
            return;
        }

        students.clear();
        int numStudents;
        file >> numStudents;

        for (int i = 0; i < numStudents; ++i) {
            int rollNumber;
            std::string name;
            std::vector<float> marks(subjectNames.size());

            file >> rollNumber;
            file.ignore();
            std::getline(file, name);
            
            for (size_t j = 0; j < subjectNames.size(); ++j) {
                file >> marks[j];
            }

            students.emplace_back(name, rollNumber, marks);
        }
        file.close();
        calculateRanks();
        std::cout << "Data loaded from student_data.txt successfully!\n";
    }

    // Display statistics
    void displayStatistics() const {
        if (students.empty()) {
            std::cout << "No students in the database!\n";
            return;
        }

        float totalPercentage = 0;
        float maxPercentage = students[0].getPercentage();
        float minPercentage = students[0].getPercentage();
        
        int gradeCount[6] = {0}; // A, B, C, D, E, F

        for (const auto& student : students) {
            float percentage = student.getPercentage();
            totalPercentage += percentage;
            
            if (percentage > maxPercentage) maxPercentage = percentage;
            if (percentage < minPercentage) minPercentage = percentage;

            char grade = student.getGrade();
            switch (grade) {
                case 'A': gradeCount[0]++; break;
                case 'B': gradeCount[1]++; break;
                case 'C': gradeCount[2]++; break;
                case 'D': gradeCount[3]++; break;
                case 'E': gradeCount[4]++; break;
                case 'F': gradeCount[5]++; break;
            }
        }

        float avgPercentage = totalPercentage / students.size();

        std::cout << "\n--- Class Statistics ---\n";
        std::cout << "Total Students: " << students.size() << std::endl;
        std::cout << "Average Percentage: " << std::fixed << std::setprecision(2) 
                  << avgPercentage << "%" << std::endl;
        std::cout << "Highest Percentage: " << maxPercentage << "%" << std::endl;
        std::cout << "Lowest Percentage: " << minPercentage << "%" << std::endl;
        std::cout << "\nGrade Distribution:\n";
        
        char grades[] = {'A', 'B', 'C', 'D', 'E', 'F'};
        for (int i = 0; i < 6; ++i) {
            std::cout << "Grade " << grades[i] << ": " << gradeCount[i] << " students\n";
        }
    }
};

void displayMenu() {
    std::cout << "\n=== Student Rank Management System ===\n";
    std::cout << "1. Add Student\n";
    std::cout << "2. Remove Student\n";
    std::cout << "3. Update Student Marks\n";
    std::cout << "4. Search Student\n";
    std::cout << "5. Display All Students\n";
    std::cout << "6. Display Top Students\n";
    std::cout << "7. Display Statistics\n";
    std::cout << "8. Save Data to File\n";
    std::cout << "9. Load Data from File\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    StudentRankManager manager;
    int choice;

    // Load data at startup
    manager.loadFromFile();

    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                manager.addStudent();
                break;
            case 2:
                manager.removeStudent();
                break;
            case 3:
                manager.updateStudent();
                break;
            case 4:
                manager.searchStudent();
                break;
            case 5:
                manager.displayAllStudents();
                break;
            case 6:
                manager.displayTopStudents();
                break;
            case 7:
                manager.displayStatistics();
                break;
            case 8:
                manager.saveToFile();
                break;
            case 9:
                manager.loadFromFile();
                break;
            case 0:
                manager.saveToFile();
                std::cout << "Thank you for using Student Rank Management System!\n";
                break;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
