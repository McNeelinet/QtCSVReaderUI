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
    double metricResult;
};

struct CSVHelperInput
{
    CSVReader* reader = nullptr;
    std::vector<double> columnFloats;
};

enum ACTIONS {
    METRICS_MAXIMUM,
    METRICS_MINIMUM,
    METRICS_MEDIAN,
    READER_ENABLE,
    READER_RESET,
    READER_DISABLE,
    READER_GETROW
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
    if (reader.file.is_open())
        reader.file.close();
    output.status = true;
}

void CSVReaderGetRow(CSVReader& reader, CSVHelperOutput& output)
{
    if (!reader.file.is_open())
        throw std::runtime_error("Файл не был открыт");

    std::string line;

    if (std::getline(reader.file, line)) {
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

void MetricsCalcMaximum(std::vector<double> values, CSVHelperOutput& output)
{
    if (!values.empty()) {
        output.metricResult = values[0];

        int size = values.size();
        for (int i = 0; i <= size - 1; i++)
            if (values[i] > output.metricResult)
                output.metricResult = values[i];

        output.status = true;
    }
}

void MetricsCalcMinimum(std::vector<double> values, CSVHelperOutput& output)
{
    if (!values.empty()) {
        output.metricResult = values[0];

        int size = values.size();
        for (int i = 0; i <= size - 1; i++)
            if (values[i] < output.metricResult)
                output.metricResult = values[i];

        output.status = true;
    }
}

void sortVector(std::vector<double>& vector, int size)
{
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (vector[j] > vector[j + 1]) {
                int temp = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = temp;
            }
}

void MetricsCalcMedian(std::vector<double> values, CSVHelperOutput& output)
{
    if (!values.empty()) {
        output.metricResult = values[0];

        sortVector(values, values.size());

        if (values.size() % 2 == 1)
            output.metricResult = values[values.size() / 2];
        else
            output.metricResult = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;

        output.status = true;
    }
}

CSVHelperOutput CSVHelperFrontController(CSVHelperInput& input, int action)
{
    CSVHelperOutput output = {.status = false};

    if (action == ACTIONS::READER_ENABLE)
        CSVReaderEnable(*(input.reader), output);
    else if (action == ACTIONS::READER_RESET)
        CSVReaderReset(*(input.reader), output);
    else if (action == ACTIONS::READER_DISABLE)
        CSVReaderDisable(*(input.reader), output);
    else if (action == ACTIONS::READER_GETROW)
        CSVReaderGetRow(*(input.reader), output);
    else if (action == ACTIONS::METRICS_MAXIMUM)
        MetricsCalcMaximum(input.columnFloats, output);
    else if (action == ACTIONS::METRICS_MINIMUM)
        MetricsCalcMinimum(input.columnFloats, output);
    else if (action == ACTIONS::METRICS_MEDIAN)
        MetricsCalcMedian(input.columnFloats, output);
    else
        throw std::runtime_error("Неизвестное действие");

    return output;
}
