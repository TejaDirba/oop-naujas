#include "Student.h"
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <stdexcept>

// Constructors
Student::Student() : egzaminas(0), galutinisVid(0.0), galutinisMed(0.0) {}

Student::Student(const std::string& vardas, const std::string& pavarde) 
    : vardas(vardas), pavarde(pavarde), egzaminas(0), galutinisVid(0.0), galutinisMed(0.0) {}

// Getters
std::string Student::getVardas() const { return vardas; }
std::string Student::getPavarde() const { return pavarde; }
std::vector<int> Student::getNd() const { return nd; }
int Student::getEgzaminas() const { return egzaminas; }
double Student::getGalutinisVid() const { return galutinisVid; }
double Student::getGalutinisMed() const { return galutinisMed; }

// Setters
void Student::setVardas(const std::string& vardas) { 
    if (vardas.empty()) {
        throw std::invalid_argument("Vardas negali būti tuščias");
    }
    this->vardas = vardas; 
}

void Student::setPavarde(const std::string& pavarde) { 
    if (pavarde.empty()) {
        throw std::invalid_argument("Pavardė negali būti tuščia");
    }
    this->pavarde = pavarde; 
}

void Student::setNd(const std::vector<int>& nd) { 
    for (int grade : nd) {
        if (grade < 0 || grade > 10) {
            throw std::invalid_argument("Namų darbų pažymys turi būti tarp 0 ir 10");
        }
    }
    this->nd = nd; 
}

void Student::setEgzaminas(int egzaminas) { 
    if (egzaminas < 0 || egzaminas > 10) {
        throw std::invalid_argument("Egzamino pažymys turi būti tarp 0 ir 10");
    }
    this->egzaminas = egzaminas; 
}

void Student::addHomeworkGrade(int grade) {
    if (grade < 0 || grade > 10) {
        throw std::invalid_argument("Pažymys turi būti tarp 0 ir 10");
    }
    nd.push_back(grade);
}

void Student::calculateFinalGrades() {
    try {
        if (nd.empty()) {
            throw std::logic_error("Nėra namų darbų pažymių");
        }
        galutinisVid = 0.4 * calculateAverage(nd) + 0.6 * egzaminas;
        galutinisMed = 0.4 * calculateMedian(nd) + 0.6 * egzaminas;
    } catch (const std::exception& e) {
        throw std::runtime_error("Klaida skaičiuojant galutinius pažymius: " + std::string(e.what()));
    }
}

// Utility functions
double calculateAverage(const std::vector<int>& nd) {
    if (nd.empty()) {
        throw std::invalid_argument("Tuščias pažymių vektorius");
    }
    return std::accumulate(nd.begin(), nd.end(), 0.0) / nd.size();
}

double calculateMedian(std::vector<int> nd) {
    if (nd.empty()) {
        throw std::invalid_argument("Tuščias pažymių vektorius");
    }
    
    std::sort(nd.begin(), nd.end());
    size_t size = nd.size();
    if (size % 2 == 0) 
        return (nd[size / 2 - 1] + nd[size / 2]) / 2.0;
    else 
        return nd[size / 2];
}

// Comparison functions
bool compareByName(const Student& a, const Student& b) {
    return a.getVardas() < b.getVardas();
}

bool compareByAvg(const Student& a, const Student& b) {
    return a.getGalutinisVid() > b.getGalutinisVid();
}

bool compareByMedian(const Student& a, const Student& b) {
    return a.getGalutinisMed() > b.getGalutinisMed();
}

// Input/Output operators
std::istream& operator>>(std::istream& is, Student& student) {
    std::string vardas, pavarde;
    if (!(is >> vardas >> pavarde)) {
        throw std::runtime_error("Klaida skaitant studento vardą ir pavardę");
    }
    
    student.setVardas(vardas);
    student.setPavarde(pavarde);
    
    return is;
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
    os << std::left << std::setw(15) << student.getPavarde()
       << std::setw(15) << student.getVardas()
       << std::setw(15) << std::fixed << std::setprecision(2) << student.getGalutinisVid()
       << std::setw(15) << std::fixed << std::setprecision(2) << student.getGalutinisMed();
    return os;
}
