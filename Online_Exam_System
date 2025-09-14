#include<bits/stdc++.h>
using namespace std;

// Question structure to store MCQ data
struct Question {
    string questionText;
    vector<string> options;
    int correctAnswer;  // 0-based index
    string subject;
    int marks;
    
    Question() : correctAnswer(0), marks(1) {}
    
    Question(string text, vector<string> opts, int correct, string subj = "General", int m = 1) 
        : questionText(text), options(opts), correctAnswer(correct), subject(subj), marks(m) {}
};

// User structure for authentication
struct User {
    string username;
    string password;
    string fullName;
    string email;
    bool isAdmin;
    vector<int> examScores;
    
    User() : isAdmin(false) {}
    
    User(string user, string pass, string name, string mail, bool admin = false) 
        : username(user), password(pass), fullName(name), email(mail), isAdmin(admin) {}
};

// Exam result structure
struct ExamResult {
    string username;
    string examDate;
    int totalQuestions;
    int correctAnswers;
    int totalMarks;
    int obtainedMarks;
    double percentage;
    time_t timestamp;
};

class OnlineExamSystem {
private:
    vector<Question> questionBank;
    vector<User> users;
    vector<ExamResult> results;
    User* currentUser;
    string questionsFile = "questions.txt";
    string usersFile = "users.txt";
    string resultsFile = "results.txt";
    
public:
    OnlineExamSystem() : currentUser(nullptr) {
        loadData();
        initializeDefaultData();
    }
    
    ~OnlineExamSystem() {
        saveData();
    }
    
    // Initialize with some default questions and admin user
    void initializeDefaultData() {
        if (users.empty()) {
            // Create default admin user
            users.push_back(User("admin", "admin123", "System Administrator", "admin@exam.com", true));
            users.push_back(User("student", "student123", "Demo Student", "student@exam.com", false));
        }
        
        if (questionBank.empty()) {
            // Add sample questions
            questionBank.push_back(Question(
                "What is the time complexity of binary search?",
                {"O(n)", "O(log n)", "O(n log n)", "O(1)"},
                1, "Data Structures", 2
            ));
            
            questionBank.push_back(Question(
                "Which of the following is not a valid C++ data type?",
                {"int", "float", "real", "double"},
                2, "C++ Programming", 2
            ));
            
            questionBank.push_back(Question(
                "What does CPU stand for?",
                {"Central Processing Unit", "Computer Personal Unit", "Central Program Utility", "Computer Processing Unit"},
                0, "Computer Basics", 1
            ));
            
            questionBank.push_back(Question(
                "Which sorting algorithm has the best average time complexity?",
                {"Bubble Sort", "Selection Sort", "Quick Sort", "Insertion Sort"},
                2, "Algorithms", 3
            ));
            
            questionBank.push_back(Question(
                "What is the size of int data type in C++?",
                {"2 bytes", "4 bytes", "8 bytes", "Depends on system"},
                3, "C++ Programming", 2
            ));
        }
    }
    
    // File handling functions
    void loadData() {
        loadQuestions();
        loadUsers();
        loadResults();
    }
    
    void saveData() {
        saveQuestions();
        saveUsers();
        saveResults();
    }
    
    void loadQuestions() {
        ifstream file(questionsFile);
        if (!file.is_open()) return;
        
        questionBank.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Question q;
            q.questionText = line;
            
            // Read options
            for (int i = 0; i < 4; i++) {
                if (getline(file, line)) {
                    q.options.push_back(line);
                }
            }
            
            // Read correct answer
            if (getline(file, line)) {
                q.correctAnswer = stoi(line);
            }
            
            // Read subject
            if (getline(file, line)) {
                q.subject = line;
            }
            
            // Read marks
            if (getline(file, line)) {
                q.marks = stoi(line);
            }
            
            questionBank.push_back(q);
            getline(file, line); // Empty line separator
        }
        file.close();
    }
    
    void saveQuestions() {
        ofstream file(questionsFile);
        if (!file.is_open()) return;
        
        for (const auto& q : questionBank) {
            file << q.questionText << "\n";
            for (const auto& opt : q.options) {
                file << opt << "\n";
            }
            file << q.correctAnswer << "\n";
            file << q.subject << "\n";
            file << q.marks << "\n\n";
        }
        file.close();
    }
    
    void loadUsers() {
        ifstream file(usersFile);
        if (!file.is_open()) return;
        
        users.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            User u;
            u.username = line;
            
            if (getline(file, line)) u.password = line;
            if (getline(file, line)) u.fullName = line;
            if (getline(file, line)) u.email = line;
            if (getline(file, line)) u.isAdmin = (line == "1");
            
            users.push_back(u);
            getline(file, line); // Empty line separator
        }
        file.close();
    }
    
    void saveUsers() {
        ofstream file(usersFile);
        if (!file.is_open()) return;
        
        for (const auto& u : users) {
            file << u.username << "\n";
            file << u.password << "\n";
            file << u.fullName << "\n";
            file << u.email << "\n";
            file << (u.isAdmin ? "1" : "0") << "\n\n";
        }
        file.close();
    }
    
    void loadResults() {
        ifstream file(resultsFile);
        if (!file.is_open()) return;
        
        results.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            ExamResult r;
            r.username = line;
            
            if (getline(file, line)) r.examDate = line;
            if (getline(file, line)) r.totalQuestions = stoi(line);
            if (getline(file, line)) r.correctAnswers = stoi(line);
            if (getline(file, line)) r.totalMarks = stoi(line);
            if (getline(file, line)) r.obtainedMarks = stoi(line);
            if (getline(file, line)) r.percentage = stod(line);
            if (getline(file, line)) r.timestamp = stoll(line);
            
            results.push_back(r);
            getline(file, line); // Empty line separator
        }
        file.close();
    }
    
    void saveResults() {
        ofstream file(resultsFile);
        if (!file.is_open()) return;
        
        for (const auto& r : results) {
            file << r.username << "\n";
            file << r.examDate << "\n";
            file << r.totalQuestions << "\n";
            file << r.correctAnswers << "\n";
            file << r.totalMarks << "\n";
            file << r.obtainedMarks << "\n";
            file << r.percentage << "\n";
            file << r.timestamp << "\n\n";
        }
        file.close();
    }
    
    // Utility functions
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void pauseScreen() {
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
    string getCurrentDateTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string dateTime(dt);
        dateTime.pop_back(); // Remove newline
        return dateTime;
    }
    
    // Authentication functions
    bool login() {
        clearScreen();
        cout << "=== LOGIN ===" << endl;
        cout << "Username: ";
        string username, password;
        cin >> username;
        cout << "Password: ";
        cin >> password;
        
        for (auto& user : users) {
            if (user.username == username && user.password == password) {
                currentUser = &user;
                cout << "\nLogin successful! Welcome " << user.fullName << endl;
                pauseScreen();
                return true;
            }
        }
        
        cout << "Invalid credentials!" << endl;
        pauseScreen();
        return false;
    }
    
    bool registerUser() {
        clearScreen();
        cout << "=== REGISTER ===" << endl;
        
        User newUser;
        cout << "Username: ";
        cin >> newUser.username;
        
        // Check if username already exists
        for (const auto& user : users) {
            if (user.username == newUser.username) {
                cout << "Username already exists!" << endl;
                pauseScreen();
                return false;
            }
        }
        
        cout << "Password: ";
        cin >> newUser.password;
        cin.ignore(); // Clear buffer
        
        cout << "Full Name: ";
        getline(cin, newUser.fullName);
        
        cout << "Email: ";
        cin >> newUser.email;
        
        newUser.isAdmin = false;
        users.push_back(newUser);
        
        cout << "Registration successful!" << endl;
        pauseScreen();
        return true;
    }
    
    void logout() {
        currentUser = nullptr;
        cout << "Logged out successfully!" << endl;
        pauseScreen();
    }
    
    // Exam functions
    void takeExam() {
        if (questionBank.empty()) {
            cout << "No questions available!" << endl;
            pauseScreen();
            return;
        }
        
        clearScreen();
        cout << "=== ONLINE EXAM ===" << endl;
        cout << "Total Questions: " << questionBank.size() << endl;
        cout << "Time Limit: " << questionBank.size() * 2 << " minutes" << endl;
        cout << "\nPress Enter to start the exam...";
        cin.ignore();
        cin.get();
        
        vector<int> userAnswers(questionBank.size(), -1);
        time_t startTime = time(0);
        int timeLimit = questionBank.size() * 120; // 2 minutes per question
        
        for (size_t i = 0; i < questionBank.size(); i++) {
            clearScreen();
            
            // Check time limit
            time_t currentTime = time(0);
            int remainingTime = timeLimit - (currentTime - startTime);
            if (remainingTime <= 0) {
                cout << "Time's up!" << endl;
                break;
            }
            
            cout << "=== Question " << (i + 1) << " of " << questionBank.size() << " ===" << endl;
            cout << "Time remaining: " << remainingTime / 60 << ":" << setfill('0') << setw(2) << remainingTime % 60 << endl;
            cout << "Subject: " << questionBank[i].subject << " | Marks: " << questionBank[i].marks << endl;
            cout << "\n" << questionBank[i].questionText << endl;
            
            for (size_t j = 0; j < questionBank[i].options.size(); j++) {
                cout << (j + 1) << ". " << questionBank[i].options[j] << endl;
            }
            
            int answer;
            cout << "\nYour answer (1-4, 0 to skip): ";
            cin >> answer;
            
            if (answer >= 1 && answer <= 4) {
                userAnswers[i] = answer - 1; // Convert to 0-based index
            }
        }
        
        // Calculate results
        calculateAndSaveResult(userAnswers);
    }
    
    void calculateAndSaveResult(const vector<int>& userAnswers) {
        ExamResult result;
        result.username = currentUser->username;
        result.examDate = getCurrentDateTime();
        result.totalQuestions = questionBank.size();
        result.correctAnswers = 0;
        result.totalMarks = 0;
        result.obtainedMarks = 0;
        result.timestamp = time(0);
        
        clearScreen();
        cout << "=== EXAM RESULTS ===" << endl;
        cout << "Student: " << currentUser->fullName << endl;
        cout << "Date: " << result.examDate << endl;
        cout << "----------------------------------------" << endl;
        
        for (size_t i = 0; i < questionBank.size(); i++) {
            result.totalMarks += questionBank[i].marks;
            
            cout << "Q" << (i + 1) << ". " << questionBank[i].questionText.substr(0, 50) << "..." << endl;
            cout << "Your answer: ";
            
            if (userAnswers[i] == -1) {
                cout << "Not answered";
            } else {
                cout << questionBank[i].options[userAnswers[i]];
                if (userAnswers[i] == questionBank[i].correctAnswer) {
                    cout << " ✓ CORRECT";
                    result.correctAnswers++;
                    result.obtainedMarks += questionBank[i].marks;
                } else {
                    cout << " ✗ INCORRECT";
                }
            }
            
            cout << "\nCorrect answer: " << questionBank[i].options[questionBank[i].correctAnswer] << endl;
            cout << "----------------------------------------" << endl;
        }
        
        result.percentage = (result.totalMarks > 0) ? 
            (double(result.obtainedMarks) / result.totalMarks) * 100 : 0;
        
        cout << "\n=== SUMMARY ===" << endl;
        cout << "Total Questions: " << result.totalQuestions << endl;
        cout << "Correct Answers: " << result.correctAnswers << endl;
        cout << "Obtained Marks: " << result.obtainedMarks << "/" << result.totalMarks << endl;
        cout << "Percentage: " << fixed << setprecision(2) << result.percentage << "%" << endl;
        
        if (result.percentage >= 90) cout << "Grade: A+ (Excellent!)" << endl;
        else if (result.percentage >= 80) cout << "Grade: A (Very Good)" << endl;
        else if (result.percentage >= 70) cout << "Grade: B (Good)" << endl;
        else if (result.percentage >= 60) cout << "Grade: C (Average)" << endl;
        else if (result.percentage >= 50) cout << "Grade: D (Pass)" << endl;
        else cout << "Grade: F (Fail)" << endl;
        
        results.push_back(result);
        saveResults();
        
        pauseScreen();
    }
    
    void viewMyResults() {
        clearScreen();
        cout << "=== MY EXAM HISTORY ===" << endl;
        
        vector<ExamResult> myResults;
        for (const auto& result : results) {
            if (result.username == currentUser->username) {
                myResults.push_back(result);
            }
        }
        
        if (myResults.empty()) {
            cout << "No exam history found!" << endl;
        } else {
            cout << "----------------------------------------" << endl;
            for (size_t i = 0; i < myResults.size(); i++) {
                cout << "Exam #" << (i + 1) << endl;
                cout << "Date: " << myResults[i].examDate << endl;
                cout << "Score: " << myResults[i].obtainedMarks << "/" << myResults[i].totalMarks << endl;
                cout << "Percentage: " << fixed << setprecision(2) << myResults[i].percentage << "%" << endl;
                cout << "----------------------------------------" << endl;
            }
        }
        
        pauseScreen();
    }
    
    // Admin functions
    void addQuestion() {
        if (!currentUser || !currentUser->isAdmin) {
            cout << "Access denied! Admin privileges required." << endl;
            pauseScreen();
            return;
        }
        
        clearScreen();
        cout << "=== ADD NEW QUESTION ===" << endl;
        
        Question newQuestion;
        cin.ignore(); // Clear buffer
        
        cout << "Enter question: ";
        getline(cin, newQuestion.questionText);
        
        cout << "Enter 4 options:" << endl;
        for (int i = 0; i < 4; i++) {
            string option;
            cout << "Option " << (i + 1) << ": ";
            getline(cin, option);
            newQuestion.options.push_back(option);
        }
        
        cout << "Correct answer (1-4): ";
        int correct;
        cin >> correct;
        newQuestion.correctAnswer = correct - 1; // Convert to 0-based
        
        cin.ignore(); // Clear buffer
        cout << "Subject: ";
        getline(cin, newQuestion.subject);
        
        cout << "Marks: ";
        cin >> newQuestion.marks;
        
        questionBank.push_back(newQuestion);
        cout << "Question added successfully!" << endl;
        pauseScreen();
    }
    
    void viewAllQuestions() {
        if (!currentUser || !currentUser->isAdmin) {
            cout << "Access denied! Admin privileges required." << endl;
            pauseScreen();
            return;
        }
        
        clearScreen();
        cout << "=== ALL QUESTIONS ===" << endl;
        
        if (questionBank.empty()) {
            cout << "No questions in database!" << endl;
        } else {
            for (size_t i = 0; i < questionBank.size(); i++) {
                cout << "Q" << (i + 1) << ". " << questionBank[i].questionText << endl;
                cout << "Subject: " << questionBank[i].subject << " | Marks: " << questionBank[i].marks << endl;
                for (size_t j = 0; j < questionBank[i].options.size(); j++) {
                    cout << "   " << (j + 1) << ". " << questionBank[i].options[j];
                    if (j == questionBank[i].correctAnswer) cout << " ✓";
                    cout << endl;
                }
                cout << "----------------------------------------" << endl;
            }
        }
        
        pauseScreen();
    }
    
    void viewAllResults() {
        if (!currentUser || !currentUser->isAdmin) {
            cout << "Access denied! Admin privileges required." << endl;
            pauseScreen();
            return;
        }
        
        clearScreen();
        cout << "=== ALL EXAM RESULTS ===" << endl;
        
        if (results.empty()) {
            cout << "No results found!" << endl;
        } else {
            cout << left << setw(15) << "Username" << setw(20) << "Date" 
                 << setw(8) << "Score" << setw(12) << "Percentage" << endl;
            cout << "--------------------------------------------------------" << endl;
            
            for (const auto& result : results) {
                cout << left << setw(15) << result.username 
                     << setw(20) << result.examDate.substr(0, 19)
                     << setw(8) << (to_string(result.obtainedMarks) + "/" + to_string(result.totalMarks))
                     << setw(12) << (to_string((int)result.percentage) + "%") << endl;
            }
        }
        
        pauseScreen();
    }
    
    void deleteQuestion() {
        if (!currentUser || !currentUser->isAdmin) {
            cout << "Access denied! Admin privileges required." << endl;
            pauseScreen();
            return;
        }
        
        if (questionBank.empty()) {
            cout << "No questions to delete!" << endl;
            pauseScreen();
            return;
        }
        
        clearScreen();
        cout << "=== DELETE QUESTION ===" << endl;
        
        // Show all questions with numbers
        for (size_t i = 0; i < questionBank.size(); i++) {
            cout << (i + 1) << ". " << questionBank[i].questionText << endl;
        }
        
        cout << "\nEnter question number to delete (0 to cancel): ";
        int choice;
        cin >> choice;
        
        if (choice > 0 && choice <= (int)questionBank.size()) {
            questionBank.erase(questionBank.begin() + choice - 1);
            cout << "Question deleted successfully!" << endl;
        } else if (choice != 0) {
            cout << "Invalid choice!" << endl;
        }
        
        pauseScreen();
    }
    
    // Menu functions
    void studentMenu() {
        int choice;
        do {
            clearScreen();
            cout << "=== STUDENT PORTAL ===" << endl;
            cout << "Welcome, " << currentUser->fullName << "!" << endl;
            cout << "1. Take Exam" << endl;
            cout << "2. View My Results" << endl;
            cout << "3. Logout" << endl;
            cout << "Choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: takeExam(); break;
                case 2: viewMyResults(); break;
                case 3: logout(); break;
                default: cout << "Invalid choice!" << endl; pauseScreen();
            }
        } while (choice != 3 && currentUser != nullptr);
    }
    
    void adminMenu() {
        int choice;
        do {
            clearScreen();
            cout << "=== ADMIN PORTAL ===" << endl;
            cout << "Welcome, " << currentUser->fullName << "!" << endl;
            cout << "1. Add Question" << endl;
            cout << "2. View All Questions" << endl;
            cout << "3. Delete Question" << endl;
            cout << "4. View All Results" << endl;
            cout << "5. Take Exam (Test)" << endl;
            cout << "6. Logout" << endl;
            cout << "Choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: addQuestion(); break;
                case 2: viewAllQuestions(); break;
                case 3: deleteQuestion(); break;
                case 4: viewAllResults(); break;
                case 5: takeExam(); break;
                case 6: logout(); break;
                default: cout << "Invalid choice!" << endl; pauseScreen();
            }
        } while (choice != 6 && currentUser != nullptr);
    }
    
    void mainMenu() {
        int choice;
        do {
            clearScreen();
            cout << "======================================" << endl;
            cout << "    ONLINE EXAMINATION SYSTEM" << endl;
            cout << "======================================" << endl;
            cout << "1. Login" << endl;
            cout << "2. Register" << endl;
            cout << "3. Exit" << endl;
            cout << "Choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1:
                    if (login()) {
                        if (currentUser->isAdmin) {
                            adminMenu();
                        } else {
                            studentMenu();
                        }
                    }
                    break;
                case 2:
                    registerUser();
                    break;
                case 3:
                    cout << "Thank you for using Online Exam System!" << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
                    pauseScreen();
            }
        } while (choice != 3);
    }
    
    void run() {
        mainMenu();
    }
};

int main() {
    OnlineExamSystem examSystem;
    examSystem.run();
    return 0;
}
