#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <chrono>
#include <random>

using namespace std;
using Clock = chrono::high_resolution_clock;

struct Timer {
    string name;
    Clock::time_point start;
    Timer(const string& n) : name(n), start(Clock::now()) {}
    ~Timer() {
        auto end = Clock::now();
        auto us  = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << name << " užtruko " << us << " μs";
        if (us > 1000)  
            cout << " (" << fixed << setprecision(2) << (us / 1000.0) << " ms)";
        if (us > 1'000'000) 
            cout << " (" << fixed << setprecision(3) << (us / 1'000'000.0) << " s)";
        cout << "\n";
    }
};

vector<Student> FileManager::readStudentsFromFile(const string& filename) {
    Timer readTimer("Skaitymas [" + filename + "]");
    vector<Student> students;
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Nepavyko atidaryti failo: " + filename);
    }

    string line;

    if (!getline(file, line)) {
        throw runtime_error("Failas tuščias arba neįmanoma perskaityti antraštės");
    }

    int lineCount = 0;
    auto calcStart = Clock::now();
    while (getline(file, line)) {
        if (line.empty()) continue;
        try {
            Student s;
            parseStudentLine(line, s);
            s.calculateFinalGrades();
            students.push_back(move(s));
            lineCount++;
        } catch (const exception& e) {
            cerr << "  Klaida skaitant eilutę: `" << line << "` => " << e.what() << "\n";
            continue;
        }
    }
    auto calcEnd = Clock::now();
    auto calcUs = chrono::duration_cast<chrono::microseconds>(calcEnd - calcStart).count();
    cout << "Galutinių balų skaičiavimas " << lineCount 
         << " studentų užtruko " << calcUs << " μs\n";

    if (students.empty()) {
        throw runtime_error("Faile nerasta nei vieno tinkamo studento įrašo");
    }

    return students;
}

void FileManager::parseStudentLine(const string& line, Student& s) {
    stringstream ss(line);
    string v, p;
    if (!(ss >> v >> p)) {
        throw invalid_argument("Nepavyko perskaityti vardo/pavardės");
    }
    s.setVardas(v);
    s.setPavarde(p);

    vector<int> allGrades;
    int g;
    while (ss >> g) {
        if (!isValidGrade(g)) {
            throw invalid_argument("Netinkamas pažymys: " + to_string(g));
        }
        allGrades.push_back(g);
    }
    if (allGrades.empty()) {
        throw invalid_argument("Nerasta nei vieno pažymio");
    }
    int egz = allGrades.back();
    allGrades.pop_back();
    s.setEgzaminas(egz);

    if (allGrades.empty()) {
        throw invalid_argument("Nėra namų darbų pažymių");
    }
    s.setNd(allGrades);
}

bool FileManager::isValidGrade(int grade) {
    return (grade >= 0 && grade <= 10);
}
void FileManager::writeResultsToFile(const vector<Student>& students,
                                     const string& filename) {
    Timer writeTimer("Įrašymas [" + filename + "]");
    ofstream out(filename);
    if (!out) {
        throw runtime_error("Nepavyko atidaryti rezultato failo rašymui: " + filename);
    }

    out << left 
        << setw(15) << "Pavardė"
        << setw(15) << "Vardas"
        << setw(15) << "Galutinis(Vid)"
        << setw(15) << "Galutinis(Med)"
        << "\n"
        << string(60,'-') << "\n";

    for (auto const& s : students) {
        out << s << "\n";
    }
    out.close();
    cout << "Rezultatai išsaugoti faile: " << filename << "\n";
}

void FileManager::printHeader() {
    cout << left 
         << setw(15) << "Pavardė"
         << setw(15) << "Vardas"
         << setw(15) << "Galutinis(Vid)"
         << setw(15) << "Galutinis(Med)"
         << "\n"
         << string(60,'-') << "\n";
}

void FileManager::printResults(const vector<Student>& students) {
    printHeader();
    for (auto const& s : students) {
        cout << s << "\n";
    }
}

void FileManager::splitAndWrite(const vector<Student>& all, const string& baseFilename) {
    Timer t("Skirstymas+įrašymas [" + baseFilename + "]");
    vector<Student> pass, fail;
    pass.reserve(all.size());
    fail.reserve(all.size());

    for (auto const& s : all) {
        if (s.getGalutinisVid() < 5.0)
            fail.push_back(s);
        else
            pass.push_back(s);
    }

    string fFail = baseFilename.substr(0, baseFilename.find_last_of('.')) + "_fail.txt";
    writeResultsToFile(fail, fFail);
    string fPass = baseFilename.substr(0, baseFilename.find_last_of('.')) + "_pass.txt";
    writeResultsToFile(pass, fPass);
}

void FileManager::generateTestFiles() {
    static const vector<size_t> sizes = {
        1'000,
        10'000,
        100'000,
        1'000'000,
        10'000'000
    };

    std::mt19937_64 rng( std::chrono::system_clock::now().time_since_epoch().count() );
    auto randGrade = [&]( ) { return std::uniform_int_distribution<int>(0,10)(rng); };
    auto randNDcount = [&]( ) { return std::uniform_int_distribution<int>(3,10)(rng); };

    for (size_t N : sizes) {
        string fname = "test_" + to_string(N) + ".txt";
        Timer t("Failo kūrimas [" + fname + "]");

        ofstream out(fname);
        if (!out) {
            cerr << "Klaida: nepavyko sukurti " << fname << "\n";
            continue;
        }
        out << "vardas pavarde nd... egz\n";

        for (size_t i = 1; i <= N; ++i) {
            out << "VardasNR" << i << " PavardeNR" << i;

            int cnt = randNDcount();
            for (int k = 0; k < cnt; ++k) {
                out << " " << randGrade();
            }
            out << " " << randGrade() << "\n"; 
        }
        out.close();
    }
}
