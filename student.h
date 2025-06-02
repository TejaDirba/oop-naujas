#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <iostream>

class Student {
private:
    std::string vardas;
    std::string pavarde;
    std::vector<int> nd;
    int egzaminas;
    double galutinisVid;
    double galutinisMed;

public:
    Student();
    Student(const std::string& vardas, const std::string& pavarde);

    std::string getVardas() const;
    std::string getPavarde() const;
    std::vector<int> getNd() const;
    int         getEgzaminas() const;
    double      getGalutinisVid() const;
    double      getGalutinisMed() const;

    void setVardas(const std::string& vardas);
    void setPavarde(const std::string& pavarde);
    void setNd(const std::vector<int>& nd);
    void setEgzaminas(int egzaminas);
    void addHomeworkGrade(int grade);

    void calculateFinalGrades(); // galutinisVid and galutinisMed

    friend std::istream& operator>>(std::istream& is, Student& student);
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
};

bool compareByName(const Student& a, const Student& b);
bool compareByAvg (const Student& a, const Student& b);
bool compareByMedian(const Student& a, const Student& b);

double calculateAverage(const std::vector<int>& nd);
double calculateMedian(std::vector<int> nd);

#endif // STUDENT_H
