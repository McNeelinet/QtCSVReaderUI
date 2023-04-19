#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct CSVReader
{
    std::string filename;
    std::ifstream file;
    int columnsCount;
    int regionColumn;
};

struct CSVHelperOutput
{
    std::vector<std::string> row;
    bool status;
};

struct CSVHelperInput
{
    CSVReader* reader = nullptr;
    std::vector<double> columnFloats;
};

enum ACTIONS {
    READER_ENABLE,
    READER_RESET,
    READER_DISABLE,
    READER_GETROW,
    METRICS_MAXIMUM,
    METRICS_MINIMUM,
    METRICS_MEDIAN
};


int findColumnsCount(std::string line)
{
    return std::count(line.cbegin(), line.cend(), ',') + 1;
}

int findColumnByPos(std::string line, size_t pos)
{
    return std::count(line.cbegin(), line.cbegin() + pos, ',');
}

int findRegionColumn(std::string line)
{
    int number = -1;

    size_t pos = line.find("region");
    if (pos == std::string::npos)
        throw std::runtime_error("Нет колонки региона");

    number = findColumnByPos(line, pos);
    return number;
}

std::string trimString(std::string string)
{
    std::string::iterator start = string.begin();
    std::string::iterator end = string.end();

    while (start != end && std::isspace(*start))
        start++;

    do
        end--;
    while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

void CSVReaderEnable(CSVReader& reader, CSVHelperOutput& output)
{
    reader.file.open(reader.filename);
    if (!reader.file.is_open())
        throw std::runtime_error("Файл не открылся");

    std::string line;
    if (!std::getline(reader.file, line))
        throw std::runtime_error("Файл пуст");

    reader.columnsCount = findColumnsCount(line);
    reader.regionColumn = findRegionColumn(line);

    reader.file.seekg(0);
    output.status = true;
}

void CSVReaderReset(CSVReader& reader, CSVHelperOutput& output)
{
    reader.file.seekg(0);
    output.status = true;
}

void CSVReaderDisable(CSVReader& reader, CSVHelperOutput& output)
{
    reader.file.close();
    output.status = true;
}

void CSVReaderGetRow(CSVReader& reader, CSVHelperOutput& output)
{
    if (!reader.file.is_open())
        throw std::runtime_error("Файл не был открыт");

    std::string line;
    if (std::getline(reader.file, line)) { // не факт, что ошибка чтения - это конец файла
        if (findColumnsCount(line) != reader.columnsCount)
            throw std::runtime_error("Поврежденный файл");

        std::stringstream lineStream(line);
        std::vector<std::string> lineArray(reader.columnsCount);
        for (int i = 0; i <= reader.columnsCount - 1; i++) {
            std::string temp;
            std::getline(lineStream, temp, ',');
            temp = trimString(temp);
            lineArray[i] = temp;
        }

        output.row = lineArray;
        output.status = true;
    }
}

CSVHelperOutput CSVHelperFrontController(CSVHelperInput& input, int action)
{
    CSVHelperOutput output = {.status = false};

    switch (action) {
    case ACTIONS::READER_ENABLE:
        if (input.reader != nullptr)
            CSVReaderEnable(*(input.reader), output);
        break;
    case ACTIONS::READER_RESET:
        if (input.reader != nullptr)
            CSVReaderReset(*(input.reader), output);
        break;
    case ACTIONS::READER_DISABLE:
        if (input.reader != nullptr)
            CSVReaderDisable(*(input.reader), output);
        break;
    case ACTIONS::READER_GETROW:
        if (input.reader != nullptr)
            CSVReaderGetRow(*(input.reader), output);
        break;
    default:
        throw std::runtime_error("Неизвестное действие");
    }

    return output;
}
