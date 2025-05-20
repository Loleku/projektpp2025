#ifndef GRADE_UTILS_H
#define GRADE_UTILS_H

#include <vector>
#include <string>

struct GradeData {
    std::vector<int> grades;
    std::vector<std::pair<int, int>> weightedGrades;
    bool isWeighted = false;
};

GradeData parseGrades(const std::string& s);
double calculateAverage(const GradeData& data);
int suggestGrade(double avg);

#endif
