#include "Student.h"
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <stdexcept>

Student::Student() 
    : vardas(""), pavarde(""), nd(), egzaminas(0), galutinisVid(0.0), galutinisMed(0.0) {}

Student::Student(const std::string& v, const std::string& p)
    : vardas(v), pavarde(p), nd(), egzaminas(0), galutinisVid(0.0), galutinisMed(0.0) {}

std::string Student::getVardas() const { return vardas; }
std::string Student::getPavarde() const { return pavarde; }
std::vector<int> Student::getNd() const    { return nd; }
int Student::getEgzaminas()      const     { return egzaminas; }
double Student::getGalutinisVid() const    { return galutinisVid; }
double Student::getGalutinisMed() const    { return galutinisMed; }

void Student::setVardas(const std::string& v) {
    if (v.empty()) 
        throw std::invalid_argument("Vardas negali būti tuščias");
    vardas = v;
}
void Student::setPavarde(const std::string& p) {
    if (p.empty()) 
        throw std::invalid_argument("Pavardė negali būti tuščia");
    pavarde = p;
}
void Student::setNd(const std::vector<int>& grades) {
    for (int g : grades) {
        if (g < 0 || g > 10)
            throw std::invalid_argument("Namų darbų pažymys turi būti tarp 0 ir 10");
    }
    nd = grades;
}
void Student::setEgzaminas(int e) {
    if (e < 0 || e > 10)
        throw std::invalid_argument("Egzamino pažymys turi būti tarp 0 ir 10");
    egzaminas = e;
}
void Student::addHomeworkGrade(int grade) {
    if (grade < 0 || grade > 10)
        throw std::invalid_argument("Pažymys turi būti tarp 0 ir 10");
    nd.push_back(grade);
}

void Student::calculateFinalGrades() {
    if (nd.empty())
        throw std::logic_error("Nėra namų darbų pažymių siūlyti finalo skaičiavimui");
    

    double avg = calculateAverage(nd);
    double med = calculateMedian(nd);
    galutinisVid = 0.4 * avg + 0.6 * egzaminas;
    galutinisMed = 0.4 * med + 0.6 * egzaminas;
}

double calculateAverage(const std::vector<int>& v) {
    if (v.empty())
        throw std::invalid_argument("Tuščias pažymių vektorius");
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}

double calculateMedian(std::vector<int> v) {
    if (v.empty())
        throw std::invalid_argument("Tuščias pažymių vektorius");
    std::sort(v.begin(), v.end());
    size_t n = v.size();
    if (n % 2 == 0) 
        return (v[n/2 - 1] + v[n/2]) / 2.0;
    else
        return v[n/2];
}

bool compareByName(const Student& a, const Student& b) {
    return a.getVardas() < b.getVardas();
}
bool compareByAvg(const Student& a, const Student& b) {
    return a.getGalutinisVid() > b.getGalutinisVid();
}
bool compareByMedian(const Student& a, const Student& b) {
    return a.getGalutinisMed() > b.getGalutinisMed();
}

std::istream& operator>>(std::istream& is, Student& s) {
    std::string v, p;
    if (!(is >> v >> p))
        throw std::runtime_error("Klaida skaitant vardą ir pavardę");
    s.setVardas(v);
    s.setPavarde(p);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Student& s) {

    os << std::left << std::setw(15) << s.getPavarde()
       << std::setw(15) << s.getVardas()
       << std::setw(15) << std::fixed << std::setprecision(2) << s.getGalutinisVid()
       << std::setw(15) << std::fixed << std::setprecision(2) << s.getGalutinisMed();
    return os;
}
