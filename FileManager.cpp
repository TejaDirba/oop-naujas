#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

std::vector<Student> FileManager::readStudentsFromFile(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream file(filename);
    
    try {
        if (!file) {
            throw std::runtime_error("Nepavyko atidaryti failo: " + filename);
        }

        std::string line;
        if (!std::getline(file, line)) {
            throw std::runtime_error("Failas tuščias arba nepavyko perskaityti antraštės");
        }

        while (std::getline(file, line)) {
            try {
                if (line.empty()) continue;
                
                Student student;
                parseStudentLine(line, student);
                student.calculateFinalGrades();
                students.push_back(student);
            } catch (const std::exception& e) {
                std::cerr << "Klaida skaitant eilutę: " << line << std::endl;
                std::cerr << "Klaidos pranešimas: " << e.what() << std::endl;
                // Tęsiame skaityti kitą eilutę
                continue;
            }
        }

        if (students.empty()) {
            throw std::runtime_error("Faile nerasta tinkamų studentų duomenų");
        }

    } catch (const std::exception& e) {
        file.close();
        throw; // Perduodame klaidą aukštesniam lygiui
    }

    file.close();
    return students;
}

void FileManager::parseStudentLine(const std::string& line, Student& student) {
    std::stringstream ss(line);
    std::string vardas, pavarde;
    
    if (!(ss >> vardas >> pavarde)) {
        throw std::invalid_argument("Nepavyko perskaityti vardo ir pavardės");
    }
    
    student.setVardas(vardas);
    student.setPavarde(pavarde);
    
    std::vector<int> grades;
    int grade;
    
    while (ss >> grade) {
        if (!isValidGrade(grade)) {
            throw std::invalid_argument("Netinkamas pažymys: " + std::to_string(grade));
        }
        grades.push_back(grade);
    }
    
    if (grades.empty()) {
        throw std::invalid_argument("Nerasta pažymių");
    }
    
    // Paskutinis pažymys yra egzaminas
    int egzaminas = grades.back();
    grades.pop_back();
    
    student.setEgzaminas(egzaminas);
    
    if (!grades.empty()) {
        student.setNd(grades);
    } else {
        throw std::invalid_argument("Nėra namų darbų pažymių");
    }
}

bool FileManager::isValidGrade(int grade) {
    return grade >= 0 && grade <= 10;
}

void FileManager::writeResultsToFile(const std::vector<Student>& students, const std::string& filename) {
    try {
        std::ofstream outputFile(filename);
        if (!outputFile) {
            throw std::runtime_error("Nepavyko sukurti rezultatų failo: " + filename);
        }

        // Rašome antraštę
        outputFile << std::left << std::setw(15) << "Pavarde" 
                   << std::setw(15) << "Vardas" 
                   << std::setw(15) << "Galutinis (Vid.)"
                   << std::setw(15) << "Galutinis (Med.)"
                   << "\n-----------------------------------------------------------\n";

        // Rašome studentų duomenis
        for (const auto& student : students) {
            outputFile << student << std::endl;
        }

        outputFile.close();
        std::cout << "Rezultatai išsaugoti faile: " << filename << std::endl;

    } catch (const std::exception& e) {
        throw std::runtime_error("Klaida rašant į failą: " + std::string(e.what()));
    }
}

void FileManager::printResults(const std::vector<Student>& students) {
    printHeader();
    for (const auto& student : students) {
        std::cout << student << std::endl;
    }
}

void FileManager::printHeader() {
    std::cout << std::left << std::setw(15) << "Pavarde" 
              << std::setw(15) << "Vardas" 
              << std::setw(15) << "Galutinis (Vid.)"
              << std::setw(15) << "Galutinis (Med.)"
              << "\n-----------------------------------------------------------\n";
}
