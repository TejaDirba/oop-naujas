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
        auto stop = Clock::now();
        auto us = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        cout << name << " užtruko " << us << " μs";
        if (us > 1000)  cout << " (" << fixed << setprecision(2) << (us/1000.0) << " ms)";
        if (us > 1'000'000) cout << " (" << fixed << setprecision(3) << (us/1e6) << " s)";
        cout << "\n";
    }
};
vector<Student> FileManager::readStudentsFromFile(const string& filename) {
    Timer t("Skaitymas [" + filename + "]");
    vector<Student> students;
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Nepavyko atidaryti failo: " + filename);
    }

    string line;
    if (!getline(file, line)) {
        throw runtime_error("Failas tuščias arba nepavyko perskaityti antraštės");
    }

    int count = 0;
    auto calcStart = Clock::now();
    while (getline(file, line)) {
        if (line.empty()) continue;
        try {
            Student s;
            parseStudentLine(line, s);
            s.calculateFinalGrades();
            students.push_back(move(s));
            ++count;
        } catch (const exception& e) {
            cerr << "  Klaida skaitant eilutę: `" << line << "` → " << e.what() << "\n";
            continue;
        }
    }
    auto calcStop = Clock::now();
    auto us = chrono::duration_cast<chrono::microseconds>(calcStop - calcStart).count();
    cout << "Galutinių pažymių skaičiavimas " << count << " studentų užtruko " << us << " μs\n";

    if (students.empty()) {
        throw runtime_error("Faile nerasta nei vieno galiojančio studento įrašo.");
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
void FileManager::writeResultsToFile(
    const vector<Student>& students,
    const string& filename
) {
    Timer t("Rašymas [" + filename + "]");
    ofstream out(filename);
    if (!out) {
        throw runtime_error("Nepavyko sukurti failo rašymui: " + filename);
    }
    out << left 
        << setw(15) << "Pavardė"
        << setw(15) << "Vardas"
        << setw(15) << "Galutinis(Vid)"
        << setw(15) << "Galutinis(Med)"
        << "\n"
        << string(60, '-') << "\n";
    for (auto const& s : students) {
        out << s << "\n";
    }
    out.close();
    cout << "Rezultatai įrašyti į: " << filename << "\n";
}
void FileManager::printHeader() {
    cout << left 
         << setw(15) << "Pavardė"
         << setw(15) << "Vardas"
         << setw(15) << "Galutinis(Vid)"
         << setw(15) << "Galutinis(Med)"
         << "\n"
         << string(60, '-') << "\n";
}

void FileManager::printResults(const vector<Student>& students) {
    printHeader();
    for (auto const& s : students) {
        cout << s << "\n";
    }
}

void FileManager::splitAndWriteVector(
    const vector<Student>& allStudents,
    const string& baseFilename
) {
    Timer t("Skirstymas (vector) + įrašymas [" + baseFilename + "]");
    vector<Student> failList, passList;
    for (auto const& s : allStudents) {
        if (s.getGalutinisVid() < 5.0) failList.push_back(s);
        else passList.push_back(s);
    }
    string nameNoExt = baseFilename.substr(0, baseFilename.find_last_of('.'));
    writeResultsToFile(failList, nameNoExt + "_fail.txt");
    writeResultsToFile(passList, nameNoExt + "_pass.txt");
}

void FileManager::splitAndWriteList(
    const list<Student>& allStudents,
    const string& baseFilename
) {
    Timer t("Skirstymas (list) + įrašymas [" + baseFilename + "]");
    list<Student> failList, passList;
    for (auto const& s : allStudents) {
        if (s.getGalutinisVid() < 5.0) failList.push_back(s);
        else passList.push_back(s);
    }
    vector<Student> failVec(failList.begin(), failList.end());
    vector<Student> passVec(passList.begin(), passList.end());
    string nameNoExt = baseFilename.substr(0, baseFilename.find_last_of('.'));
    writeResultsToFile(failVec, nameNoExt + "_fail.txt");
    writeResultsToFile(passVec, nameNoExt + "_pass.txt");
}

void FileManager::splitAndWriteDeque(
    const deque<Student>& allStudents,
    const string& baseFilename
) {
    Timer t("Skirstymas (deque) + įrašymas [" + baseFilename + "]");
    deque<Student> failList, passList;
    for (auto const& s : allStudents) {
        if (s.getGalutinisVid() < 5.0) failList.push_back(s);
        else passList.push_back(s);
    }
    vector<Student> failVec(failList.begin(), failList.end());
    vector<Student> passVec(passList.begin(), passList.end());
    string nameNoExt = baseFilename.substr(0, baseFilename.find_last_of('.'));
    writeResultsToFile(failVec, nameNoExt + "_fail.txt");
    writeResultsToFile(passVec, nameNoExt + "_pass.txt");
}

template <typename Container>
void FileManager::readIntoContainer(
    const string& filename,
    Container& outContainer
) {
    Timer t("Skaitymas [" + filename + "] into container");
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Nepavyko atidaryti failo: " + filename);
    }

    string line;
    if (!getline(file, line)) {
        throw runtime_error("Failas tuščias arba neįmanoma perskaityti antraštės");
    }

    int count = 0;
    auto calcStart = Clock::now();
    while (getline(file, line)) {
        if (line.empty()) continue;
        try {
            Student s;
            parseStudentLine(line, s);
            s.calculateFinalGrades();
            outContainer.push_back(move(s));
            ++count;
        } catch (const exception& e) {
            cerr << "  Klaida skaitant eilutę: `" << line << "` → " << e.what() << "\n";
            continue;
        }
    }
    auto calcStop = Clock::now();
    auto us = chrono::duration_cast<chrono::microseconds>(calcStop - calcStart).count();
    cout << "Galutinių pažymių skaičiavimas " << count << " studentų užtruko " << us << " μs\n";

    if (count == 0) {
        throw runtime_error("Faile nerasta nei vieno validžaus studento");
    }
}
template void FileManager::readIntoContainer<vector<Student>>(const string&, vector<Student>&);
template void FileManager::readIntoContainer<list<Student>>(const string&, list<Student>&);
template void FileManager::readIntoContainer<deque<Student>>(const string&, deque<Student>&);

void FileManager::splitStrategy1_Vector(
    const vector<Student>& data,
    vector<Student>& failList,
    vector<Student>& passList
) {
    failList.clear();
    passList.clear();
    for (auto const& s : data) {
        if (s.getGalutinisVid() < 5.0) failList.push_back(s);
        else passList.push_back(s);
    }
}
void FileManager::splitStrategy2_Vector(
    vector<Student>& data,
    vector<Student>& failList
) {
    auto it = std::stable_partition(data.begin(), data.end(),
        [](const Student& s){ return s.getGalutinisVid() < 5.0; }
    );
    failList.assign(data.begin(), it);
    data.erase(data.begin(), it);
}
void FileManager::splitStrategy1_List(
    const list<Student>& data,
    list<Student>& failList,
    list<Student>& passList
) {
    failList.clear();
    passList.clear();
    for (auto const& s : data) {
        if (s.getGalutinisVid() < 5.0) failList.push_back(s);
        else passList.push_back(s);
    }
}

void FileManager::splitStrategy2_List(
    list<Student>& data,
    list<Student>& failList
) {
    failList.clear();
    for (auto it = data.begin(); it != data.end(); ) {
        if (it->getGalutinisVid() < 5.0) {
            auto toMove = it++;
            failList.splice(failList.end(), data, toMove);
        } else {
            ++it;
        }
    }
}

void FileManager::splitStrategy1_Deque(
    const deque<Student>& data,
    deque<Student>& failList,
    deque<Student>& passList
) {
    failList.clear();
    passList.clear();
    for (auto const& s : data) {
        if (s.getGalutinisVid() < 5.0) failList.push_back(s);
        else passList.push_back(s);
    }
}
void FileManager::splitStrategy2_Deque(
    deque<Student>& data,
    deque<Student>& failList
) {
    failList.clear();
    deque<Student> kept; 
    kept.reserve(data.size());
    for (auto const& s : data) {
        if (s.getGalutinisVid() < 5.0) {
            failList.push_back(s);
        } else {
            kept.push_back(s);
        }
    }
    data.swap(kept);
}
void FileManager::generateTestFiles() {
    static const vector<size_t> sizes = {
        1'000,
        10'000,
        100'000,
        1'000'000,
        10'000'000
    };
    mt19937_64 rng( chrono::system_clock::now().time_since_epoch().count() );
    auto randGrade = [&]( ) { return uniform_int_distribution<int>(0,10)(rng); };
    auto randNDcount = [&]( ) { return uniform_int_distribution<int>(3,10)(rng); };

    for (size_t N : sizes) {
        string fname = "test_" + to_string(N) + ".txt";
        Timer t("Failo kūrimas [" + fname + "]");

        ofstream out(fname);
        if (!out) {
            cerr << "  Klaida: nepavyko sukurti " << fname << "\n";
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
