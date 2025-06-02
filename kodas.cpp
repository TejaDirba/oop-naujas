#include "Student.h"
#include "FileManager.h"
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <exception>
#include <chrono>

using namespace std;
using Clock = chrono::high_resolution_clock;
void printMainMenu() {
    cout << "\n=== STUDENTŲ PAŽYMIŲ SISTEMA v1.0 ===\n"
         << "1) Interactive mode (choose container + strategy + single file)\n"
         << "2) Batch mode (process all 5 test_*.txt with all containers & strategies)\n"
         << "3) Generate test files (1k, 10k, 100k, 1M, 10M)\n"
         << "4) Exit\n"
         << "Pasirinkimas (1-4): ";
}

int askIntInRange(const string& prompt, int lo, int hi) {
    int x;
    while (true) {
        cout << prompt;
        if ( (cin >> x) && (x >= lo && x <= hi) ) {
            return x;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  → Netinkamas pasirinkimas! Bandykite dar kartą.\n";
    }
}

string askFileName() {
    cout << "Įveskite failo pavadinimą: ";
    string s; 
    cin >> s;
    return s;
}
template <typename Container>
void runInteractiveOnce(int splitStrategy, const string& filename) {
    Container container;
    FileManager::readIntoContainer<Container>(filename, container);

    auto tSortStart = Clock::now();
    if constexpr ( std::is_same_v<Container, vector<Student>> ) {
        sort(container.begin(), container.end(), compareByName);
    }
    else if constexpr ( std::is_same_v<Container, deque<Student>> ) {
        sort(container.begin(), container.end(), compareByName);
    }
    else {
        container.sort(compareByName);
    }
    auto tSortStop = Clock::now();
    auto sortUs = chrono::duration_cast<chrono::microseconds>(tSortStop - tSortStart).count();
    cout << "Rūšiavimas užtruko " << sortUs << " μs\n";

    if constexpr ( std::is_same_v<Container, vector<Student>> ) {
        if (splitStrategy == 1) {
            vector<Student> failList, passList;
            auto tSplitStart = Clock::now();
            FileManager::splitStrategy1_Vector(container, failList, passList);
            auto tSplitStop = Clock::now();
            auto splitUs = chrono::duration_cast<chrono::microseconds>(tSplitStop - tSplitStart).count();
            cout << "Split (strategy1) užtruko " << splitUs << " μs\n";

            string baseNoExt = filename.substr(0, filename.find_last_of('.'));
            FileManager::writeResultsToFile(failList, baseNoExt + "_fail.txt");
            FileManager::writeResultsToFile(passList, baseNoExt + "_pass.txt");
        }
        else {
            vector<Student> failList;
            auto tSplitStart = Clock::now();
            FileManager::splitStrategy2_Vector(container, failList);
            auto tSplitStop = Clock::now();
            auto splitUs = chrono::duration_cast<chrono::microseconds>(tSplitStop - tSplitStart).count();
            cout << "Split (strategy2) užtruko " << splitUs << " μs\n";

            string baseNoExt = filename.substr(0, filename.find_last_of('.'));
            FileManager::writeResultsToFile(failList, baseNoExt + "_fail.txt");
            FileManager::writeResultsToFile(container, baseNoExt + "_pass.txt");
        }
    }
    else if constexpr ( std::is_same_v<Container, list<Student>> ) {
        if (splitStrategy == 1) {
            list<Student> failList, passList;
            auto tSplitStart = Clock::now();
            FileManager::splitStrategy1_List(container, failList, passList);
            auto tSplitStop = Clock::now();
            auto splitUs = chrono::duration_cast<chrono::microseconds>(tSplitStop - tSplitStart).count();
            cout << "Split (strategy1) užtruko " << splitUs << " μs\n";

            vector<Student> failVec(failList.begin(), failList.end());
            vector<Student> passVec(passList.begin(), passList.end());
            string baseNoExt = filename.substr(0, filename.find_last_of('.'));
            FileManager::writeResultsToFile(failVec, baseNoExt + "_fail.txt");
            FileManager::writeResultsToFile(passVec, baseNoExt + "_pass.txt");
        } else {
            list<Student> failList;
            auto tSplitStart = Clock::now();
            FileManager::splitStrategy2_List(container, failList);
            auto tSplitStop = Clock::now();
            auto splitUs = chrono::duration_cast<chrono::microseconds>(tSplitStop - tSplitStart).count();
            cout << "Split (strategy2) užtruko " << splitUs << " μs\n";

            vector<Student> failVec(failList.begin(), failList.end());
            vector<Student> passVec(container.begin(), container.end());
            string baseNoExt = filename.substr(0, filename.find_last_of('.'));
            FileManager::writeResultsToFile(failVec, baseNoExt + "_fail.txt");
            FileManager::writeResultsToFile(passVec, baseNoExt + "_pass.txt");
        }
    }
    else { 
        if (splitStrategy == 1) {
            deque<Student> failList, passList;
            auto tSplitStart = Clock::now();
            FileManager::splitStrategy1_Deque(container, failList, passList);
            auto tSplitStop = Clock::now();
            auto splitUs = chrono::duration_cast<chrono::microseconds>(tSplitStop - tSplitStart).count();
            cout << "Split (strategy1) užtruko " << splitUs << " μs\n";

            vector<Student> failVec(failList.begin(), failList.end());
            vector<Student> passVec(passList.begin(), passList.end());
            string baseNoExt = filename.substr(0, filename.find_last_of('.'));
            FileManager::writeResultsToFile(failVec, baseNoExt + "_fail.txt");
            FileManager::writeResultsToFile(passVec, baseNoExt + "_pass.txt");
        } else {
            deque<Student> failList;
            auto tSplitStart = Clock::now();
            FileManager::splitStrategy2_Deque(container, failList);
            auto tSplitStop = Clock::now();
            auto splitUs = chrono::duration_cast<chrono::microseconds>(tSplitStop - tSplitStart).count();
            cout << "Split (strategy2) užtruko " << splitUs << " μs\n";

            vector<Student> failVec(failList.begin(), failList.end());
            vector<Student> passVec(container.begin(), container.end());
            string baseNoExt = filename.substr(0, filename.find_last_of('.'));
            FileManager::writeResultsToFile(failVec, baseNoExt + "_fail.txt");
            FileManager::writeResultsToFile(passVec, baseNoExt + "_pass.txt");
        }
    }
}
void runBatchEverything() {
    static const vector<string> testFiles = {
        "test_1000.txt",
        "test_10000.txt",
        "test_100000.txt",
        "test_1000000.txt",
        "test_10000000.txt"
    };
    double totalRuns = 0;
    double sumAllRuns = 0;

    for (int containerType = 1; containerType <= 3; ++containerType) {
        for (int strategy = 1; strategy <= 2; ++strategy) {
            cout << "\n>>>> ContainerType=" << containerType
                 << " | Strategy=" << strategy << "\n";
            for (size_t i = 0; i < testFiles.size(); ++i) {
                const string& fn = testFiles[i];
                auto runStart = Clock::now();

                try {
                    if (containerType == 1) {
                        vector<Student> data;
                        FileManager::readIntoContainer<vector<Student>>(fn, data);
                        if (!data.empty())
                            sort(data.begin(), data.end(), compareByName);
                        if (strategy == 1) {
                            vector<Student> fails, passes;
                            FileManager::splitStrategy1_Vector(data, fails, passes);
                            string baseNoExt = fn.substr(0, fn.find_last_of('.'));
                            FileManager::writeResultsToFile(fails, baseNoExt + "_fail.txt");
                            FileManager::writeResultsToFile(passes, baseNoExt + "_pass.txt");
                        } else {
                            vector<Student> fails;
                            FileManager::splitStrategy2_Vector(data, fails);
                            string baseNoExt = fn.substr(0, fn.find_last_of('.'));
                            FileManager::writeResultsToFile(fails, baseNoExt + "_fail.txt");
                            FileManager::writeResultsToFile(data, baseNoExt + "_pass.txt");
                        }
                    }
                    else if (containerType == 2) {
                        list<Student> data;
                        FileManager::readIntoContainer<list<Student>>(fn, data);
                        if (!data.empty())
                            data.sort(compareByName);
                        if (strategy == 1) {
                            list<Student> fails, passes;
                            FileManager::splitStrategy1_List(data, fails, passes);
                            vector<Student> failsVec(fails.begin(), fails.end());
                            vector<Student> passesVec(passes.begin(), passes.end());
                            string baseNoExt = fn.substr(0, fn.find_last_of('.'));
                            FileManager::writeResultsToFile(failsVec, baseNoExt + "_fail.txt");
                            FileManager::writeResultsToFile(passesVec, baseNoExt + "_pass.txt");
                        } else {
                            list<Student> fails;
                            FileManager::splitStrategy2_List(data, fails);
                            vector<Student> failsVec(fails.begin(), fails.end());
                            vector<Student> passesVec(data.begin(), data.end());
                            string baseNoExt = fn.substr(0, fn.find_last_of('.'));
                            FileManager::writeResultsToFile(failsVec, baseNoExt + "_fail.txt");
                            FileManager::writeResultsToFile(passesVec, baseNoExt + "_pass.txt");
                        }
                    }
                    else {
                        deque<Student> data;
                        FileManager::readIntoContainer<deque<Student>>(fn, data);
                        if (!data.empty())
                            sort(data.begin(), data.end(), compareByName);
                        if (strategy == 1) {
                            deque<Student> fails, passes;
                            FileManager::splitStrategy1_Deque(data, fails, passes);
                            vector<Student> failsVec(fails.begin(), fails.end());
                            vector<Student> passesVec(passes.begin(), passes.end());
                            string baseNoExt = fn.substr(0, fn.find_last_of('.'));
                            FileManager::writeResultsToFile(failsVec, baseNoExt + "_fail.txt");
                            FileManager::writeResultsToFile(passesVec, baseNoExt + "_pass.txt");
                        } else {
                            deque<Student> fails;
                            FileManager::splitStrategy2_Deque(data, fails);
                            vector<Student> failsVec(fails.begin(), fails.end());
                            vector<Student> passesVec(data.begin(), data.end());
                            string baseNoExt = fn.substr(0, fn.find_last_of('.'));
                            FileManager::writeResultsToFile(failsVec, baseNoExt + "_fail.txt");
                            FileManager::writeResultsToFile(passesVec, baseNoExt + "_pass.txt");
                        }
                    }
                }
                catch (const exception& e) {
                    cerr << "Klaida apdorojant `" << fn << "`: " << e.what() << "\n";
                    continue;
                }

                auto runStop = Clock::now();
                auto us = chrono::duration_cast<chrono::microseconds>(runStop - runStart).count();
                cout << "`" << fn << "` užtruko " << us << " μs\n";

                sumAllRuns += us;
                ++totalRuns;
            }
            cout << "––––––––––––––––––––––––––––––––––––\n";
        }
    }

    if (totalRuns > 0) {
        double avgUs = sumAllRuns / totalRuns;
        cout << "\n==>>> Vidutinis laikas per visus apdorojimus: "
             << (long long)avgUs << " μs ("
             << fixed << setprecision(3) << (avgUs/1000.0) << " ms)\n";
    }
}

void generateFilesOnce() {
    FileManager::generateTestFiles();
    cout << "\nTestiniai failai (1k, 10k, 100k, 1M, 10M) sugeneruoti.\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while (true) {
        printMainMenu();
        int mainChoice;
        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (mainChoice == 1) {
            int contType = askIntInRange("Pasirinkite konteinerį (1=vector, 2=list, 3=deque): ", 1, 3);
            int strat = askIntInRange("Pasirinkite strategiją (1 = two‐container copy, 2 = in‐place remove/partition): ", 1, 2);
            string fname = askFileName();
            cout << "\n→ Veikia “Container=" << contType << ", Strategy=" << strat << ", File=`" << fname << "`\n";

            if (contType == 1) {
                runInteractiveOnce<vector<Student>>(strat, fname);
            }
            else if (contType == 2) {
                runInteractiveOnce<list<Student>>(strat, fname);
            }
            else {
                runInteractiveOnce<deque<Student>>(strat, fname);
            }
        }
        else if (mainChoice == 2) {
            cout << "\n--- BATCH MODE: visi 5 failai × 3 konteineriai × 2 strategijos ---\n";
            runBatchEverything();
        }
        else if (mainChoice == 3) {
            cout << "\n--- GENERUOJAME 5 TESTINIUS FAILUS ---\n";
            generateFilesOnce();
        }
        else if (mainChoice == 4) {
            cout << "Programa baigta.\n";
            return 0;
        }
        else {
            cout << "Neteisinga parinktis (1-4). Bandykite dar kartą.\n";
        }
    }

    return 0;
}
