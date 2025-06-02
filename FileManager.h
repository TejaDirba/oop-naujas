#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Student.h"
#include <vector>
#include <string>

class FileManager {
public:
    static std::vector<Student> readStudentsFromFile(const std::string& filename);
    static void writeResultsToFile(const std::vector<Student>& students, 
                                   const std::string& filename = "results.txt");
    static void printResults(const std::vector<Student>& students);
    static void printHeader();

private:
    static void parseStudentLine(const std::string& line, Student& student);
    static bool isValidGrade(int grade);
};

#endif
