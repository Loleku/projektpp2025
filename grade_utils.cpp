#include "grade_utils.h"
#include <sstream>
#include <algorithm>
#include <numeric>
#include <stdexcept>

GradeData parseGrades(const std::string &s)
{
    GradeData result;
    if (s.rfind("[[", 0) == 0 && s.size() >= 4)
    {
        result.isWeighted = true;
        std::string inner = s.substr(2, s.size() - 4);
        size_t pos = 0;
        while (pos < inner.size())
        {
            size_t next = inner.find("],[", pos);
            std::string pair = (next == std::string::npos)
                                   ? inner.substr(pos)
                                   : inner.substr(pos, next - pos);
            std::stringstream ss(pair);
            int grade, weight;
            char sep;
            if (ss >> grade >> sep >> weight && sep == ',')
            {
                result.weightedGrades.emplace_back(grade, weight);
            }
            else
            {
                throw std::runtime_error("Błędny format ocen ważonych: " + pair);
            }
            if (next == std::string::npos)
                break;
            pos = next + 3;
        }
    }
    else if (s.rfind("[", 0) == 0 && s.back() == ']')
    {
        result.isWeighted = false;
        std::string inner = s.substr(1, s.size() - 2);
        std::stringstream ss(inner);
        std::string token;
        while (std::getline(ss, token, ','))
        {
            try
            {
                result.grades.push_back(std::stoi(token));
            }
            catch (...)
            {
                throw std::runtime_error("Błędny format ocen: " + token);
            }
        }
    }
    else
    {
        throw std::runtime_error("Format ocen powinien być [[ocena,waga],] lub [ocena,]");
    }
    return result;
}

double calculateAverage(const GradeData &data)
{
    if (data.isWeighted)
    {
        int sum = 0, count = 0;
        for (const auto &p : data.weightedGrades)
        {
            sum += p.first * p.second;
            count += p.second;
        }
        return count > 0 ? static_cast<double>(sum) / count : 0.0;
    }
    else
    {
        if (data.grades.empty())
            return 0.0;
        return std::accumulate(data.grades.begin(), data.grades.end(), 0.0) / data.grades.size();
    }
}

int suggestGrade(double avg)
{
    if (avg >= 5.5)
        return 6;
    if (avg >= 4.5)
        return 5;
    if (avg >= 3.5)
        return 4;
    if (avg >= 2.5)
        return 3;
    if (avg >= 1.5)
        return 2;
    return 1;
}
