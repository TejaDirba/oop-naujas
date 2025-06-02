#include "Student.h"
#include "FileManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <chrono>

using namespace std;
using Clock = chrono::high_resolution_clock;

void showMainMenu() {
    cout << "\n=== studentai_v0.4 – PASIRINKITE ===\n";
    cout << "1) Generuoti testinius failus (N = 1k, 10k, 100k, 1M, 10M)\n";
    cout << "2) Apdoroti vieną studentų failą (skirstymas + įrašymas)\n";
    cout << "3) Vykdyti visus 5 testinius failus ir parodyti summary\n";
    cout << "4) Išeiti\n";
    cout << "Pasirinkite (1–4): ";
}

string askFilename() {
    cout << "Įveskite failo pavadinimą: ";
    string s; 
    cin >> s;
    return s;
}
void processSingleFile() {
    string fn = askFilename();
    try {
        auto v = FileManager::readStudentsFromFile(fn);
        cout << "Perskaityta studentų: " << v.size() << "\n";

        FileManager::splitAndWrite(v, fn);
    } 
    catch (const exception& e) {
        cerr << "Klaida: " << e.what() << "\n";
    }
}

void runAllTests() {
    static const vector<string> testFiles = {
        "test_1000.txt",
        "test_10000.txt",
        "test_100000.txt",
        "test_1000000.txt",
        "test_10000000.txt"
    };

    vector<long long> timesUs;
    timesUs.reserve(testFiles.size());

    cout << "\n>>> Pradedamas masinis testavimas:\n";
    for (auto const& fn : testFiles) {
        auto t0 = Clock::now();
        try {
            auto v = FileManager::readStudentsFromFile(fn);
            FileManager::splitAndWrite(v, fn);
        }
        catch (const exception& e) {
            cerr << "Klaida su failu `" << fn << "`: " << e.what() << "\n";
            timesUs.push_back(-1);
            continue;
        }
        auto t1 = Clock::now();
        auto durUs = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        timesUs.push_back(durUs);

        cout << "Visas apdorojimas `" << fn << "` užtruko " 
             << durUs << " μs ("
             << fixed << setprecision(3) << (durUs / 1000.0) << " ms)\n";
    }

    long long sum = 0; 
    int count = 0;
    for (auto x : timesUs) {
        if (x >= 0) { sum += x; count++; }
    }
    if (count > 0) {
        double avgUs = double(sum) / count;
        cout << "\n--- Vidutinis laikas sėkmingiems failams: " 
             << (long long)avgUs << " μs (" 
             << fixed << setprecision(3) << (avgUs / 1000.0) << " ms)\n";
    } else {
        cout << "\nNebuvo sėkmingų apdorojimų, vidurkis neaišku.\n";
    }
}

void generateFiles() {
    FileManager::generateTestFiles();
    cout << "\nTestiniai failai sugeneruoti.\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while (true) {
        showMainMenu();
        int choice; 
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                generateFiles();
                break;
            case 2:
                processSingleFile();
                break;
            case 3:
                runAllTests();
                break;
            case 4:
                cout << "Programa baigta.\n";
                return 0;
            default:
                cout << "Pasirinkite teisingą (1–4).\n";
        }
    }
    return 0;
}
