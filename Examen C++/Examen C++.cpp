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

void markAsDone() {
    cout << "Введіть назву завдання: ";
    string title;
    getline(cin, title);

    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].title == title) {
            allTasks[i].done = true;
            cout << "Позначено як виконане.\n";
            return;
        }
    }
    cout << "Не знайдено.\n";
}

void showUserTasks() {
    cout << "ID користувача: ";
    int uid;
    cin >> uid;
    cin.ignore();

    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].user.id == uid) {
            cout << allTasks[i].title << " - " << (allTasks[i].done ? "Виконане" : "Не виконане") << "\n";
        }
    }
}

void searchByPriority() {
    cout << "Пріоритет (1-LOW,2-MEDIUM,3-HIGH): ";
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
    cout << "Дедлайн: ";
    string d;
    getline(cin, d);
    for (int i = 0; i < taskCount; i++) {
        if (allTasks[i].deadline == d) {
            cout << allTasks[i].title << "\n";
        }
    }
}

void searchByUser() {
    cout << "ID користувача: ";
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
    cout << "Збережено у tasks.txt\n";
}

void saveUserTasks() {
    cout << "ID користувача: ";
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
        cout << "Не знайдено користувача.\n";
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
    cout << "Збережено у " << filename << "\n";
}

void saveByCategory() {
    cout << "Тип (1-WORK,2-PERSONAL,3-STUDY,4-OTHER): ";
    int tp;
    cin >> tp;
    cin.ignore();

    string fname;
    if (tp == 1) fname = "work.txt";
    else if (tp == 2) fname = "personal.txt";
    else if (tp == 3) fname = "study.txt";
    else if (tp == 4) fname = "other.txt";
    else {
        cout << "Невірний тип.\n";
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
    cout << "Збережено у " << fname << "\n";
}

int main() {
    setlocale(LC_ALL, "Ukrainian");

    initCategories();
    while (true) {
        cout << "\nМеню:\n";
        cout << "1. Додати\n2. Видалити\n3. Позначити виконаним\n4. Показати завдання користувача\n";
        cout << "5. Зберегти усі\n6. Зберегти користувача\n7. Зберегти категорію\n";
        cout << "8. Пошук по пріоритету\n9. Пошук по дедлайну\n10. Пошук по юзеру\n11. Вийти\n";
        cout << "Ваш вибір: ";
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
        else cout << "Невірно.\n";
    }
    return 0;
}
