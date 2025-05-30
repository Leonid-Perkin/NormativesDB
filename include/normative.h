#ifndef NORMATIVE_H
#define NORMATIVE_H

#include <string>
#include <memory>
#include <iostream>

struct Normative {
    std::string name;
    std::string date;
    double result;
    int grade;

    Normative() = default;
    Normative(const std::string& name_, const std::string& date_, double result_, int grade_)
        : name(name_), date(date_), result(result_), grade(grade_) {}

    virtual ~Normative() = default;
    virtual void print() const = 0;
    virtual std::shared_ptr<Normative> clone() const = 0;
};

struct Run100 : public Normative {
    Run100() { name = "Бег 100м"; }
    Run100(const std::string& date_, double result_, int grade_) {
        name = "Бег 100м";
        date = date_;
        result = result_;
        grade = grade_;
    }
    void print() const override {
        std::cout << name << " | Дата: " << date << " | Результат: " << result << " | Оценка: " << grade << '\n';
    }
    std::shared_ptr<Normative> clone() const override {
        return std::make_shared<Run100>(*this);
    }
};

struct Run1000 : public Normative {
    Run1000() { name = "Бег 1000м"; }
    Run1000(const std::string& date_, double result_, int grade_) {
        name = "Бег 1000м";
        date = date_;
        result = result_;
        grade = grade_;
    }
    void print() const override {
        std::cout << name << " | Дата: " << date << " | Результат: " << result << " | Оценка: " << grade << '\n';
    }
    std::shared_ptr<Normative> clone() const override {
        return std::make_shared<Run1000>(*this);
    }
};

struct PullUps : public Normative {
    PullUps() { name = "Подтягивания"; }
    PullUps(const std::string& date_, double result_, int grade_) {
        name = "Подтягивания";
        date = date_;
        result = result_;
        grade = grade_;
    }
    void print() const override {
        std::cout << name << " | Дата: " << date << " | Результат: " << result << " | Оценка: " << grade << '\n';
    }
    std::shared_ptr<Normative> clone() const override {
        return std::make_shared<PullUps>(*this);
    }
};

struct Swim : public Normative {
    Swim() { name = "Плавание"; }
    Swim(const std::string& date_, double result_, int grade_) {
        name = "Плавание";
        date = date_;
        result = result_;
        grade = grade_;
    }
    void print() const override {
        std::cout << name << " | Дата: " << date << " | Результат: " << result << " | Оценка: " << grade << '\n';
    }
    std::shared_ptr<Normative> clone() const override {
        return std::make_shared<Swim>(*this);
    }
};

#endif