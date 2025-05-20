#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <tuple>
#include <iomanip>
#include "grade_utils.h"
#include "file_utils.h"

namespace App
{
    typedef std::vector<std::tuple<std::string, GradeData, double, int>> StudentVec;
    typedef std::vector<std::tuple<std::string, std::string, GradeData, double, int>> TeacherVec;

    std::string formatGrades(const GradeData& data)
    {
        std::ostringstream oss;
        if (data.isWeighted)
        {
            oss << "[";
            for (size_t i = 0; i < data.weightedGrades.size(); ++i)
            {
                if (i > 0)
                    oss << ",";
                oss << "[" << data.weightedGrades[i].first << "," << data.weightedGrades[i].second << "]";
            }
            oss << "]";
        }
        else
        {
            oss << "[";
            for (size_t i = 0; i < data.grades.size(); ++i)
            {
                if (i > 0)
                    oss << ",";
                oss << data.grades[i];
            }
            oss << "]";
        }
        return oss.str();
    }

    template <typename T, typename Compare>
    void bubbleSort(std::vector<T>& vec, Compare comp)
    {
        for (size_t i = 0; i < vec.size(); ++i)
        {
            for (size_t j = 0; j < vec.size() - i - 1; ++j)
            {
                if (comp(vec[j + 1], vec[j]))
                {
                    std::swap(vec[j], vec[j + 1]);
                }
            }
        }
    }
}
