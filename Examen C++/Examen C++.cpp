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
        cout << "Більше завдань додати не можна.\n";
        return;
    }

    Task t;
    cout << "Назва: ";
    getline(cin, t.title);
    cout << "Опис: ";
    getline(cin, t.description);
    cout << "Дедлайн (рррр-мм-дд): ";
    getline(cin, t.deadline);

    int pr;
    cout << "Пріоритет (1-LOW,2-MEDIUM,3-HIGH): ";
    cin >> pr;
    cin.ignore();
    t.priority = (Priority)pr;

    int tp;
    cout << "Тип (1-WORK,2-PERSONAL,3-STUDY,4-OTHER): ";
    cin >> tp;
    cin.ignore();
    t.type = (TaskType)tp;

    cout << "ID користувача: ";
    cin >> t.user.id;
    cin.ignore();
    cout << "Ім'я: ";
    getline(cin, t.user.name);
    cout << "Прізвище: ";
    getline(cin, t.user.forname);
    cout << "Email: ";
    getline(cin, t.user.email);

    t.done = false;

    allTasks[taskCount++] = t;

    int idx = t.type - 1;
    categories[idx].tasks[categories[idx].count++] = t;

    cout << "Завдання додано.\n";
}

void deleteTask() {
    cout << "Введіть назву завдання для видалення: ";
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
        cout << "Завдання видалено.\n";
    else
        cout << "Не знайдено.\n";
}

