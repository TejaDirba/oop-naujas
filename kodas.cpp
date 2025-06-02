#include "Student.h"
#include "FileManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

void displayMenu() {
    std::cout << "\n=== STUDENTŲ PAŽYMIŲ SISTEMA ===" << std::endl;
    std::cout << "Pasirinkite rūšiavimo būdą:" << std::endl;
    std::cout << "1 - Rūšiuoti pagal vardą" << std::endl;
    std::cout << "2 - Rūšiuoti pagal vidurkį (didėjimo tvarka)" << std::endl;
    std::cout << "3 - Rūšiuoti pagal medianą (didėjimo tvarka)" << std::endl;
    std::cout << "Jūsų pasirinkimas: ";
}

std::string getFileName() {
    std::string filename;
    std::cout << "Įveskite failo pavadinimą (pvz., Studentai10000.txt): ";
    std::cin >> filename;
    return filename;
}

int getSortOption() {
    int option;
    displayMenu();
    
    while (!(std::cin >> option) || option < 1 || option > 3) {
        std::cout << "Netinkamas pasirinkimas! Įveskite skaičių nuo 1 iki 3: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    
    return option;
}

void sortStudents(std::vector<Student>& students, int sortOption) {
    try {
        switch (sortOption) {
            case 1:
                std::sort(students.begin(), students.end(), compareByName);
                std::cout << "Studentai surūšiuoti pagal vardą." << std::endl;
                break;
            case 2:
                std::sort(students.begin(), students.end(), compareByAvg);
                std::cout << "Studentai surūšiuoti pagal vidurkį." << std::endl;
                break;
            case 3:
                std::sort(students.begin(), students.end(), compareByMedian);
                std::cout << "Studentai surūšiuoti pagal medianą." << std::endl;
                break;
            default:
                throw std::invalid_argument("Netinkamas rūšiavimo variantas");
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Klaida rūšiuojant studentus: " + std::string(e.what()));
    }
}

int main() {
    try {
        // Gauti failo pavadinimą
        std::string filename = getFileName();
        
        // Perskaityti studentų duomenis
        std::cout << "Skaitoma iš failo: " << filename << std::endl;
        std::vector<Student> students = FileManager::readStudentsFromFile(filename);
        
        std::cout << "Sėkmingai perskaityta " << students.size() << " studentų." << std::endl;
        
        // Gauti rūšiavimo parinktį
        int sortOption = getSortOption();
        
        // Surūšiuoti studentus
        sortStudents(students, sortOption);
        
        // Išvesti rezultatus į konsolę
        std::cout << "\n=== REZULTATAI ===" << std::endl;
        FileManager::printResults(students);
        
        // Išsaugoti rezultatus į failą
        FileManager::writeResultsToFile(students, "results.txt");
        
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime klaida: " << e.what() << std::endl;
        return 1;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Netinkami argumentai: " << e.what() << std::endl;
        return 1;
    } catch (const std::logic_error& e) {
        std::cerr << "Logikos klaida: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Bendroji klaida: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Nežinoma klaida!" << std::endl;
        return 1;
    }
    
    return 0;
}
