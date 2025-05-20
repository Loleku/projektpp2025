#include "main.h"

using namespace App;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

    if (argc < 2 || argc > 3)
    {
        std::cerr << "Błąd: Niepoprawne użycie\nPrzykładowe użycie: " << argv[0] << " <dane.csv> <Opcjonalnie: wynik.csv>\n";
        return 1;
    }

    std::ifstream inFile(argv[1]);
    if (!inFile.is_open())
    {
        std::cerr << "Błąd: Nie można otworzyć pliku: " << argv[1] << '\n';
        return 1;
    }

    std::ostream *outStream;
    std::ofstream outFile;
    if (argc == 3)
    {
        outFile.open(argv[2]);
        if (!outFile.is_open())
        {
            std::cerr << "Błąd: Nie można utworzyć pliku: " << argv[2] << '\n';
            return 1;
        }
        outStream = &outFile;
    }
    else
    {
        outStream = &std::cout;
    }

    std::string includeExtras;
    std::cout << "Czy dodać dodatkowe informacje (Zagrożenia, Średnia, Najlepsze)? (T - tak, inne - nie): ";
    std::getline(std::cin, includeExtras);

    std::string sortAvg, sortPred;
    std::cout << "Sortowanie po średniej? (r - rosnąco, m - malejąco, nic - bez zmian): ";
    std::getline(std::cin, sortAvg);
    std::cout << "Sortowanie po przewidywanej ocenie? (r - rosnąco, m - malejąco, nic - bez zmian): ";
    std::getline(std::cin, sortPred);

    std::string headerLine;
    std::getline(inFile, headerLine);
    std::stringstream headerStream(headerLine);
    std::vector<std::string> headers;
    std::string header;
    while (std::getline(headerStream, header, ','))
        headers.push_back(header);

    *outStream << std::fixed << std::setprecision(2);

    if (isStudentMode(headers))
    {
        *outStream << "przedmiot,oceny,srednia,ocena_przewidywana\n";
        App::StudentVec allData;
        std::string line;
        while (std::getline(inFile, line))
        {
            std::stringstream ss(line);
            std::string subject, gradesStr;
            std::getline(ss, subject, ',');
            std::getline(ss, gradesStr);
            try
            {
                GradeData data = parseGrades(gradesStr);
                double avg = calculateAverage(data);
                int pred = suggestGrade(avg);
                allData.emplace_back(subject, data, avg, pred);
            }
            catch (std::exception &e)
            {
                std::cerr << "Błąd w wierszu: " << line << " " << e.what() << '\n';
            }
        }

        App::bubbleSort(allData, [&](const auto &a, const auto &b)
                        {
                if (sortAvg == "r" && std::get<2>(a) != std::get<2>(b)) return std::get<2>(a) < std::get<2>(b);
                if (sortAvg == "m" && std::get<2>(a) != std::get<2>(b)) return std::get<2>(a) > std::get<2>(b);
                if (sortPred == "r") return std::get<3>(a) < std::get<3>(b);
                if (sortPred == "m") return std::get<3>(a) > std::get<3>(b);
                return false; });

        for (const auto &item : allData)
        {
            *outStream << std::get<0>(item) << ","
                       << formatGrades(std::get<1>(item)) << ","
                       << std::get<2>(item) << ","
                       << std::get<3>(item) << "\n";
        }

        if (includeExtras == "T")
        {
            std::vector<std::string> zagrozenia, maxOcena, minOcena;
            int maxVal = 1, minVal = 6;
            double total = 0;

            for (const auto &item : allData)
            {
                int pred = std::get<3>(item);
                total += pred;
                if (pred == 1)
                    zagrozenia.push_back(std::get<0>(item));
                if (pred > maxVal)
                    maxVal = pred;
                if (pred < minVal)
                    minVal = pred;
            }

            for (const auto &item : allData)
            {
                int pred = std::get<3>(item);
                if (pred == maxVal)
                    maxOcena.push_back(std::get<0>(item));
                if (pred == minVal)
                    minOcena.push_back(std::get<0>(item));
            }

            *outStream << "\nŚrednia z przewidywanych ocen każdego przedmiotu: "
                       << (allData.empty() ? 0 : total / allData.size()) << "\n";
            *outStream << "Zagrożenia: " << (zagrozenia.empty() ? "brak" : "");
            for (size_t i = 0; i < zagrozenia.size(); ++i)
                *outStream << (i ? ", " : "") << zagrozenia[i];
            *outStream << "\nNajniższe oceny przewidywane: ";
            for (size_t i = 0; i < minOcena.size(); ++i)
                *outStream << (i ? ", " : "") << minOcena[i];
            *outStream << "\nNajwyzsze oceny przewidywane: ";
            for (size_t i = 0; i < maxOcena.size(); ++i)
                *outStream << (i ? ", " : "") << maxOcena[i];
            *outStream << "\n";
        }
    }
    else if (isTeacherMode(headers))
    {
        bool hasNr = (headers[0] == "nr");
        if (hasNr)
            *outStream << "nr,";
        *outStream << "imie,nazwisko,oceny,srednia,ocena_przewidywana\n";

        App::TeacherVec allData;
        std::string line;
        while (std::getline(inFile, line))
        {
            std::stringstream ss(line);
            std::string imie, nazwisko, gradesStr;
            std::getline(ss, imie, ',');
            std::getline(ss, nazwisko, ',');
            std::getline(ss, gradesStr);
            try
            {
                GradeData data = parseGrades(gradesStr);
                double avg = calculateAverage(data);
                int pred = suggestGrade(avg);
                allData.emplace_back(imie, nazwisko, data, avg, pred);
            }
            catch (std::exception &e)
            {
                std::cerr << "Błąd w wierszu: " << line << " " << e.what() << '\n';
            }
        }

        App::bubbleSort(allData, [&](const auto &a, const auto &b)
                        {
                if (sortAvg == "r" && std::get<3>(a) != std::get<3>(b)) return std::get<3>(a) < std::get<3>(b);
                if (sortAvg == "m" && std::get<3>(a) != std::get<3>(b)) return std::get<3>(a) > std::get<3>(b);
                if (sortPred == "r") return std::get<4>(a) < std::get<4>(b);
                if (sortPred == "m") return std::get<4>(a) > std::get<4>(b);
                return false; });

        for (size_t i = 0; i < allData.size(); ++i)
        {
            *outStream << std::get<0>(allData[i]) << ',' << std::get<1>(allData[i]) << ','
                       << formatGrades(std::get<2>(allData[i])) << ',' << std::get<3>(allData[i])
                       << ',' << std::get<4>(allData[i]) << '\n';
        }

        if (includeExtras == "T")
        {
            std::vector<std::string> zagrozenia, najlepsze;
            int maxGrade = 1;
            double total = 0;

            for (const auto &t : allData)
            {
                int grade = std::get<4>(t);
                total += grade;
                if (grade == 1)
                    zagrozenia.push_back(std::get<0>(t) + " " + std::get<1>(t));
                if (grade > maxGrade)
                    maxGrade = grade;
            }

            for (const auto &t : allData)
            {
                int grade = std::get<4>(t);
                if (grade == maxGrade)
                    najlepsze.push_back(std::get<0>(t) + " " + std::get<1>(t) + " (" + std::to_string(grade) + ")");
            }

            *outStream << "\nZagrożenia: ";
            if (zagrozenia.empty())
                *outStream << "Brak\n";
            else
            {
                for (size_t i = 0; i < zagrozenia.size(); ++i)
                {
                    if (i > 0)
                        *outStream << ", ";
                    *outStream << zagrozenia[i];
                }
                *outStream << '\n';
            }

            *outStream << "Najlepsze Oceny: ";
            for (size_t i = 0; i < najlepsze.size(); ++i)
            {
                if (i > 0)
                    *outStream << ", ";
                *outStream << najlepsze[i];
            }
            *outStream << '\n';

            if (!allData.empty())
                *outStream << "Średnia klasowa: " << (total / allData.size()) << '\n';
        }
    }
    else
    {
        std::cerr << "Błędny format pliku wejściowego. Obsługiwane nagłówki: 'przedmiot,oceny' lub 'imie,nazwisko,oceny'\n";
        return 1;
    }

    return 0;
}
