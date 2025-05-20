#include "file_utils.h"

bool isStudentMode(const std::vector<std::string> &headers)
{
    return headers.size() == 2 && headers[0] == "przedmiot" && headers[1] == "oceny";
}

bool isTeacherMode(const std::vector<std::string> &headers)
{
    return headers.size() == 3 && headers[0] == "imie" && headers[1] == "nazwisko" && headers[2] == "oceny";
}
