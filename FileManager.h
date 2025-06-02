#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Student.h"
#include <vector>
#include <list>
#include <deque>
#include <string>

class FileManager {
public:
    static std::vector<Student> readStudentsFromFile(const std::string& filename);
    static void writeResultsToFile(
        const std::vector<Student>& students,
        const std::string& filename = "results.txt"
    );
    static void printResults(const std::vector<Student>& students);
    static void printHeader();
    static void splitAndWriteVector(
        const std::vector<Student>& allStudents,
        const std::string& baseFilename
    );
    static void splitAndWriteList(
        const std::list<Student>& allStudents,
        const std::string& baseFilename
    );
    static void splitAndWriteDeque(
        const std::deque<Student>& allStudents,
        const std::string& baseFilename
    );
    template <typename Container>
    static void readIntoContainer(
        const std::string& filename, 
        Container& outContainer
    );
    static void splitStrategy1_Vector(
        const std::vector<Student>& data,
        std::vector<Student>& failList,
        std::vector<Student>& passList
    );
    static void splitStrategy2_Vector(
        std::vector<Student>& data,
        std::vector<Student>& failList
    );
    // LIST version:
    static void splitStrategy1_List(
        const std::list<Student>& data,
        std::list<Student>& failList,
        std::list<Student>& passList
    );
    static void splitStrategy2_List(
        std::list<Student>& data,
        std::list<Student>& failList
    );
    static void splitStrategy1_Deque(
        const std::deque<Student>& data,
        std::deque<Student>& failList,
        std::deque<Student>& passList
    );
    static void splitStrategy2_Deque(
        std::deque<Student>& data,
        std::deque<Student>& failList
    );
    static void generateTestFiles();

private:
    static void parseStudentLine(const std::string& line, Student& student);
    static bool isValidGrade(int grade);
};

#endif // FILEMANAGER_H
