#include "file_operations.h"
#include <fstream>
#include <iostream>
#include "normative.h"
#include "crypto_utils.h"

const std::string TEMP_DATA_FILE = "data_raw.bin";
const std::string ENCRYPTED_FILE = "data.bin.enc";
const std::string ENCRYPTION_PASSWORD = "CyberOwl-CTF";

void saveToFile(const std::vector<Student>& students) {
    std::ofstream out(TEMP_DATA_FILE, std::ios::binary);
    if (!out) {
        std::cerr << "Ошибка при открытии временного файла для записи\n";
        return;
    }

    try {
        size_t size = students.size();
        out.write(reinterpret_cast<char*>(&size), sizeof(size));
        for (const auto& s : students) {
            size_t len;

            len = s.surname.size();
            out.write(reinterpret_cast<char*>(&len), sizeof(len));
            out.write(s.surname.c_str(), len);

            len = s.initials.size();
            out.write(reinterpret_cast<char*>(&len), sizeof(len));
            out.write(s.initials.c_str(), len);

            out.write(reinterpret_cast<const char*>(&s.admissionYear), sizeof(s.admissionYear));

            len = s.groupName.size();
            out.write(reinterpret_cast<char*>(&len), sizeof(len));
            out.write(s.groupName.c_str(), len);

            len = s.recordBook.size();
            out.write(reinterpret_cast<char*>(&len), sizeof(len));
            out.write(s.recordBook.c_str(), len);

            size_t testCount = s.tests.size();
            out.write(reinterpret_cast<char*>(&testCount), sizeof(testCount));
            for (const auto& t : s.tests) {
                len = t->name.size();
                out.write(reinterpret_cast<char*>(&len), sizeof(len));
                out.write(t->name.c_str(), len);

                len = t->date.size();
                out.write(reinterpret_cast<char*>(&len), sizeof(len));
                out.write(t->date.c_str(), len);

                out.write(reinterpret_cast<const char*>(&t->result), sizeof(t->result));
                out.write(reinterpret_cast<const char*>(&t->grade), sizeof(t->grade));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при записи в файл: " << e.what() << "\n";
    }
    out.close();

    if (!encryptFile(TEMP_DATA_FILE, ENCRYPTED_FILE, ENCRYPTION_PASSWORD)) {
        std::cerr << "Ошибка при шифровании файла\n";
    } else {
        std::remove(TEMP_DATA_FILE.c_str());
    }
}

void loadFromFile(std::vector<Student>& students) {
    if (!decryptFile(ENCRYPTED_FILE, TEMP_DATA_FILE, ENCRYPTION_PASSWORD)) {
        std::cout << "Файл не найден или не удалось расшифровать. Начинаем с пустой базы\n";
        return;
    }

    std::ifstream in(TEMP_DATA_FILE, std::ios::binary);
    if (!in) {
        std::cout << "Ошибка при открытии временного файла\n";
        return;
    }

    try {
        size_t size;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (!in) return;

        students.clear();
        for (size_t i = 0; i < size; ++i) {
            Student s;
            size_t len;
            char buffer[256];

            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (len > sizeof(buffer)) throw std::runtime_error("Слишком длинная строка");
            in.read(buffer, len); s.surname = std::string(buffer, len);

            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (len > sizeof(buffer)) throw std::runtime_error("Слишком длинная строка");
            in.read(buffer, len); s.initials = std::string(buffer, len);

            in.read(reinterpret_cast<char*>(&s.admissionYear), sizeof(s.admissionYear));

            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (len > sizeof(buffer)) throw std::runtime_error("Слишком длинная строка");
            in.read(buffer, len); s.groupName = std::string(buffer, len);

            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (len > sizeof(buffer)) throw std::runtime_error("Слишком длинная строка");
            in.read(buffer, len); s.recordBook = std::string(buffer, len);

            size_t testCount;
            in.read(reinterpret_cast<char*>(&testCount), sizeof(testCount));
            for (size_t j = 0; j < testCount; ++j) {
                std::string name, date;
                double result;
                int grade;

                in.read(reinterpret_cast<char*>(&len), sizeof(len));
                if (len > sizeof(buffer)) throw std::runtime_error("Слишком длинная строка");
                in.read(buffer, len); name = std::string(buffer, len);

                in.read(reinterpret_cast<char*>(&len), sizeof(len));
                if (len > sizeof(buffer)) throw std::runtime_error("Слишком длинная строка");
                in.read(buffer, len); date = std::string(buffer, len);

                in.read(reinterpret_cast<char*>(&result), sizeof(result));
                in.read(reinterpret_cast<char*>(&grade), sizeof(grade));

                if (name == "Бег 100м")
                    s.tests.push_back(std::make_shared<Run100>(date, result, grade));
                else if (name == "Бег 1000м")
                    s.tests.push_back(std::make_shared<Run1000>(date, result, grade));
                else if (name == "Подтягивания")
                    s.tests.push_back(std::make_shared<PullUps>(date, result, grade));
                else if (name == "Плавание")
                    s.tests.push_back(std::make_shared<Swim>(date, result, grade));
            }
            students.push_back(std::move(s));
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при чтении файла: " << e.what() << "\n";
        students.clear();
    }
    in.close();
    std::remove(TEMP_DATA_FILE.c_str());
}