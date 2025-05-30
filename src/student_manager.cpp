#include "student_manager.h"
#include "validation.h"
#include "file_operations.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>

void showMenu() {
    std::cout << "\n=== Система учета спортивных нормативов ===\n";
    std::cout << "1. Добавить студента\n";
    std::cout << "2. Показать всех студентов\n";
    std::cout << "3. Список задолжников по нормативу\n";
    std::cout << "4. Сортировка групп по среднему баллу\n";
    std::cout << "5. Удалить студента\n";
    std::cout << "0. Выход\n";
}

void addStudent(std::vector<Student>& students) {
    Student s;
    std::string input;

    clearInputBuffer();
    do {
        std::cout << "Фамилия: ";
        std::getline(std::cin, input);
        if (!isValidName(input)) {
            std::cout << "Ошибка: Фамилия не должна содержать управляющие символы\n";
        }
    } while (!isValidName(input));
    s.surname = input;

    do {
        std::cout << "Инициалы (например, И.И.): ";
        std::getline(std::cin, input);
        if (!isValidName(input)) {
            std::cout << "Ошибка: Инициалы не должны содержать управляющие символы\n";
        }
    } while (!isValidName(input));
    s.initials = input;

    do {
        std::cout << "Год поступления (2000-2025): ";
        if (!(std::cin >> s.admissionYear)) {
            std::cout << "Ошибка: Введите число\n";
            clearInputBuffer();
            continue;
        }
        if (!isValidYear(s.admissionYear)) {
            std::cout << "Ошибка: Год должен быть между 2000 и 2025\n";
        }
    } while (!isValidYear(s.admissionYear));

    clearInputBuffer();
    do {
        std::cout << "Группа: ";
        std::getline(std::cin, input);
        if (!isValidGroupName(input)) {
            std::cout << "Ошибка: Название группы должно содержать только кириллические буквы, цифры, пробелы, дефис или точку\n";
        }
    } while (!isValidGroupName(input));
    s.groupName = input;

    do {
        std::cout << "Номер зачетной книжки: ";
        std::getline(std::cin, input);
        if (input.length() > 20) {
            std::cout << "Ошибка: Номер зачетной книжки слишком длинный\n";
        }
    } while (input.length() > 20);
    s.recordBook = input;

    int count;
    do {
        std::cout << "Сколько нормативов добавить? (0-10): ";
        if (!(std::cin >> count) || count < 0 || count > 10) {
            std::cout << "Ошибка: Введите число от 0 до 10\n";
            clearInputBuffer();
            continue;
        }
        break;
    } while (true);

    clearInputBuffer();
    for (int i = 0; i < count; ++i) {
        int type;
        std::string date;
        double result;
        int grade;

        do {
            std::cout << "\n1. Бег 100м\n2. Бег 1000м\n3. Подтягивания\n4. Плавание\nТип норматива: ";
            if (!(std::cin >> type) || type < 1 || type > 4) {
                std::cout << "Ошибка: Введите число от 1 до 4\n";
                clearInputBuffer();
                continue;
            }
            break;
        } while (true);

        clearInputBuffer();
        do {
            std::cout << "Дата (ДД.ММ.ГГГГ): ";
            std::getline(std::cin, input);
            if (!isValidDate(input)) {
                std::cout << "Ошибка: Введите дату в формате ДД.ММ.ГГГГ\n";
            }
        } while (!isValidDate(input));
        date = input;

        do {
            std::cout << "Результат: ";
            if (!(std::cin >> result) || result < 0) {
                std::cout << "Ошибка: Результат должен быть положительным числом\n";
                clearInputBuffer();
                continue;
            }
            break;
        } while (true);

        do {
            std::cout << "Оценка (1-5): ";
            if (!(std::cin >> grade) || grade < 1 || grade > 5) {
                std::cout << "Ошибка: Оценка должна быть от 1 до 5\n";
                clearInputBuffer();
                continue;
            }
            break;
        } while (true);

        switch (type) {
            case 1: s.tests.push_back(std::make_shared<Run100>(date, result, grade)); break;
            case 2: s.tests.push_back(std::make_shared<Run1000>(date, result, grade)); break;
            case 3: s.tests.push_back(std::make_shared<PullUps>(date, result, grade)); break;
            case 4: s.tests.push_back(std::make_shared<Swim>(date, result, grade)); break;
        }
        clearInputBuffer();
    }
    students.push_back(std::move(s));
    saveToFile(students);
    std::cout << "Студент успешно добавлен\n";
}

void deleteStudent(std::vector<Student>& students) {
    std::string recordBook;
    std::cout << "Введите номер зачетной книжки студента для удаления: ";
    clearInputBuffer();
    std::getline(std::cin, recordBook);

    auto it = std::find_if(students.begin(), students.end(),
        [&recordBook](const Student& s) { return s.recordBook == recordBook; });

    if (it != students.end()) {
        std::cout << "Найден студент: " << it->surname << " " << it->initials << "\n";
        std::cout << "Вы уверены, что хотите удалить? (y/n): ";
        char confirm;
        std::cin >> confirm;
        if (confirm == 'y' || confirm == 'Y') {
            students.erase(it);
            saveToFile(students);
            std::cout << "Студент успешно удален\n";
        } else {
            std::cout << "Удаление отменено\n";
        }
    } else {
        std::cout << "Студент с таким номером зачетной книжки не найден\n";
    }
}

void printAll(const std::vector<Student>& students) {
    if (students.empty()) {
        std::cout << "Список студентов пуст\n";
        return;
    }

    Student::printTableHeader();
    for (const auto& s : students) {
        s.printTableRow();
    }

    std::cout << "\nПодробная информация о нормативных результатах:\n";
    for (const auto& s : students) {
        std::cout << "\nСтудент: " << s.surname << " " << s.initials << "\n";
        if (s.tests.empty()) {
            std::cout << "Нет зарегистрированных нормативов\n";
        } else {
            for (const auto& test : s.tests) {
                test->print();
            }
        }
    }
}

void printDebtorsByNorm(const std::vector<Student>& students) {
    std::string norm;
    std::cout << "Введите название норматива (Бег 100м / Бег 1000м / Подтягивания / Плавание): ";
    clearInputBuffer();
    std::getline(std::cin, norm);

    bool found = false;
    for (const auto& s : students) {
        if (s.hasDebt(norm)) {
            s.print();
            found = true;
        }
    }
    if (!found) {
        std::cout << "Задолжников по нормативу '" << norm << "' не найдено\n";
    }
}

void sortGroupsByAvgGrade(const std::vector<Student>& students) {
    std::string norm;
    std::cout << "Введите название норматива для анализа (или пустую строку для всех нормативов): ";
    clearInputBuffer();
    std::getline(std::cin, norm);

    std::map<std::string, std::pair<double, int>> groupStats;
    for (const auto& s : students) {
        auto& [sum, count] = groupStats[s.groupName];
        double avg = s.averageGrade(norm);
        if (avg > 0) {
            sum += avg;
            count++;
        }
    }

    std::vector<std::pair<std::string, double>> sorted;
    for (const auto& [group, val] : groupStats) {
        sorted.emplace_back(group, val.second ? val.first / val.second : 0);
    }

    if (sorted.empty()) {
        std::cout << "Нет данных для сортировки\n";
        return;
    }

    std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    std::cout << "\nРейтинг групп по среднему баллу:\n";
    std::cout << std::left << std::setw(20) << "Группа" << "Средний балл\n";
    std::cout << std::string(40, '-') << "\n";
    for (const auto& [group, avg] : sorted) {
        std::cout << std::left << std::setw(20) << group << std::fixed << std::setprecision(2) << avg << "\n";
    }
}