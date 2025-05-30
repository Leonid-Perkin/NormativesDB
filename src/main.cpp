#include <iostream>
#include <locale>
#include "student_manager.h"
#include "file_operations.h"
#include "validation.h"

std::vector<Student> students;

int main() {
    try {
        std::locale::global(std::locale("ru_RU.UTF-8"));
        std::cout.imbue(std::locale());
        std::cin.imbue(std::locale());
    } catch (const std::runtime_error& e) {
        std::cerr << "Ошибка установки локали: " << e.what() << ". Продолжаем с локалью по умолчанию.\n";
    }

    loadFromFile(students);
    int choice;
    do {
        showMenu();
        std::cout << "Ваш выбор: ";
        if (!(std::cin >> choice)) {
            std::cout << "Ошибка: Введите число\n";
            clearInputBuffer();
            continue;
        }
        switch (choice) {
            case 1: addStudent(students); break;
            case 2: printAll(students); break;
            case 3: printDebtorsByNorm(students); break;
            case 4: sortGroupsByAvgGrade(students); break;
            case 5: deleteStudent(students); break;
            case 0: saveToFile(students); break;
            default: std::cout << "Неверный выбор! Введите число от 0 до 5\n";
        }
    } while (choice != 0);

    return 0;
}