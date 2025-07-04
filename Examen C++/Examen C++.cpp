#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_TASKS = 100;
const int MAX_CATEGORIES = 4;

enum Priority { LOW = 1, MEDIUM, HIGH };
enum TaskType { WORK = 1, PERSONAL, STUDY, OTHER };

struct User {
    string name;
    string forname;
    int id;
    string email;
};

struct Task {
    string title;
    string description;
    string deadline;
    Priority priority;
    bool done;
    TaskType type;
    User user;
};

struct Category {
    TaskType type;
    Task tasks[MAX_TASKS];
    int count = 0;
};

Task allTasks[MAX_TASKS];
int taskCount = 0;
Category categories[MAX_CATEGORIES];

void initCategories() {
    for (int i = 0; i < MAX_CATEGORIES; i++) {
        categories[i].type = (TaskType)(i + 1);
        categories[i].count = 0;
    }
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        cout << "Cannot add more tasks.\n";
        return;
    }

    Task t;
    cout << "Title: ";
    getline(cin, t.title);
    cout << "Description: ";
    getline(cin, t.description);
    cout << "Deadline (yyyy-mm-dd): ";
    getline(cin, t.deadline);

    int pr;
    cout << "Priority (1-LOW,2-MEDIUM,3-HIGH): ";
    cin >> pr;
    cin.ignore();
    t.priority = (Priority)pr;

    int tp;
    cout << "Type (1-WORK,2-PERSONAL,3-STUDY,4-OTHER): ";
    cin >> tp;
    cin.ignore();
    t.type = (TaskType)tp;

    cout << "User ID: ";
    cin >> t.user.id;
    cin.ignore();
    cout << "First name: ";
    getline(cin, t.user.name);
    cout << "Last name: ";
    getline(cin, t.user.forname);
    cout << "Email: ";
    getline(cin, t.user.email);

    t.done = false;

    allTasks[taskCount++] = t;

    int idx = t.type - 1;
    categories[idx].tasks[categories[idx].count++] = t;

    cout << "Task added.\n";
}

void deleteTask() {
    cout << "Enter task title to delete: ";
    string title;
    getline(cin, title);

    bool found = false;
    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].title == title) {
            for (int j = i; j < taskCount - 1; j++) {
                allTasks[j] = allTasks[j + 1];
            }
            taskCount--;
            found = true;
            break;
        }
    }

    for (int c = 0; c < MAX_CATEGORIES; c++) {
        for (int i = 0; i < categories[c].count; i++) {
            if (categories[c].tasks[i].title == title) {
                for (int j = i; j < categories[c].count - 1; j++) {
                    categories[c].tasks[j] = categories[c].tasks[j + 1];
                }
                categories[c].count--;
                break;
            }
        }
    }

    if (found)
        cout << "Task deleted.\n";
    else
        cout << "Not found.\n";
}

void markAsDone() {
    cout << "Enter task title: ";
    string title;
    getline(cin, title);

    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].title == title) {
            allTasks[i].done = true;
            cout << "Marked as done.\n";
            return;
        }
    }
    cout << "Not found.\n";
}

void showUserTasks() {
    cout << "User ID: ";
    int uid;
    cin >> uid;
    cin.ignore();

    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].user.id == uid) {
            cout << allTasks[i].title << " - " << (allTasks[i].done ? "Done" : "Not done") << "\n";
        }
    }
}

void searchByPriority() {
    cout << "Priority (1-LOW,2-MEDIUM,3-HIGH): ";
    int pr;
    cin >> pr;
    cin.ignore();
    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].priority == pr) {
            cout << allTasks[i].title << "\n";
        }
    }
}

void searchByDeadline() {
    cout << "Deadline: ";
    string d;
    getline(cin, d);
    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].deadline == d) {
            cout << allTasks[i].title << "\n";
        }
    }
}

void searchByUser() {
    cout << "User ID: ";
    int uid;
    cin >> uid;
    cin.ignore();
    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].user.id == uid) {
            cout << allTasks[i].title << "\n";
        }
    }
}

void saveAllTasks() {
    ofstream f("tasks.txt");
    for (int i = 0; i < taskCount; i++) {
        Task t = allTasks[i];
        f << i + 1 << ". "
            << t.title << " "
            << t.deadline << " "
            << t.priority << " "
            << t.done << "\n";
    }
    f.close();
    cout << "Saved to tasks.txt\n";
}

void saveUserTasks() {
    cout << "User ID: ";
    int uid;
    cin >> uid;
    cin.ignore();

    string filename = "";
    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].user.id == uid) {
            filename = allTasks[i].user.name + "_" + allTasks[i].user.forname + ".txt";
            break;
        }
    }

    if (filename == "") {
        cout << "User not found.\n";
        return;
    }

    ofstream f(filename);
    int count = 1;
    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].user.id == uid) {
            Task t = allTasks[i];
            f << count << ". "
                << t.title << " "
                << t.deadline << " "
                << t.priority << " "
                << t.done << "\n";
            count++;
        }
    }
    f.close();
    cout << "Saved to " << filename << "\n";
}

void saveByCategory() {
    cout << "Type (1-WORK,2-PERSONAL,3-STUDY,4-OTHER): ";
    int tp;
    cin >> tp;
    cin.ignore();

    string fname;
    if (tp == 1) fname = "work.txt";
    else if (tp == 2) fname = "personal.txt";
    else if (tp == 3) fname = "study.txt";
    else if (tp == 4) fname = "other.txt";
    else {
        cout << "Invalid type.\n";
        return;
    }

    int idx = tp - 1;
    ofstream f(fname);
    int count = 1;
    for (int i = 0; i < categories[idx].count; i++) {
        Task t = categories[idx].tasks[i];
        f << count << ". "
            << t.title << " "
            << t.deadline << " "
            << t.priority << " "
            << t.done << "\n";
        count++;
    }
    f.close();
    cout << "Saved to " << fname << "\n";
}

int main() {
    setlocale(LC_ALL, "English");

    initCategories();
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add\n2. Delete\n3. Mark as done\n4. Show user tasks\n";
        cout << "5. Save all\n6. Save user\n7. Save category\n";
        cout << "8. Search by priority\n9. Search by deadline\n10. Search by user\n11. Exit\n";
        cout << "Your choice: ";
        int c;
        cin >> c;
        cin.ignore();
        if (c == 1) addTask();
        else if (c == 2) deleteTask();
        else if (c == 3) markAsDone();
        else if (c == 4) showUserTasks();
        else if (c == 5) saveAllTasks();
        else if (c == 6) saveUserTasks();
        else if (c == 7) saveByCategory();
        else if (c == 8) searchByPriority();
        else if (c == 9) searchByDeadline();
        else if (c == 10) searchByUser();
        else if (c == 11) break;
        else cout << "Invalid.\n";
    }
    return 0;
}
