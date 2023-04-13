#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct CSVReader
{
    std::string filename;
    std::ifstream file;
    int columns;
};

struct CSVHelperOutput
{
    std::vector<std::string> row;
    bool status;
};

enum ACTIONS {
    READER_ENABLE,
    READER_DISABLE,
    READER_GETROW
};


void CSVReaderEnable(CSVReader& reader, CSVHelperOutput& output)
{
    reader.file.open(reader.filename);
    if (!reader.file.is_open())
        throw std::runtime_error("Файл не открылся");

    std::string line;
    if(!std::getline(reader.file, line))
        throw std::runtime_error("Файл пуст");

    reader.columns = std::count(line.cbegin(), line.cend(), ',') + 1;
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
    if (!std::getline(reader.file, line))
        throw std::runtime_error("Достигнут конец файла");
    if (std::count(line.cbegin(), line.cend(), ',') + 1 != reader.columns)
        throw std::runtime_error("Поврежденный файл");

    std::stringstream lineStream(line);
    std::vector<std::string> lineArray(reader.columns);
    for (int i = 0; i <= reader.columns - 1; i++)
        std::getline(lineStream, lineArray[i], ',');

    output.row = lineArray;
    output.status = true;
}

CSVHelperOutput CSVHelperFrontController(CSVReader& reader, int action)
{
    CSVHelperOutput output = {.status = false};

    switch (action) {
    case ACTIONS::READER_ENABLE:
        CSVReaderEnable(reader, output);
        break;
    case ACTIONS::READER_DISABLE:
        CSVReaderDisable(reader, output);
        break;
    case ACTIONS::READER_GETROW:
        CSVReaderGetRow(reader, output);
        break;
    default:
        throw std::runtime_error("Неизвестное действие");
    }

    return output;
}
