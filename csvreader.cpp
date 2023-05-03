#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct FileInfo
{
    std::string filename;
    std::ifstream file;
    int columns;
    int colRegNumber;
};

struct IOData
{
    std::vector<std::string> rowValues;
    std::vector<double> rowDoubleValues;
    double metricResult;
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


////
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
////

bool CSVReaderEnable(FileInfo& fi)
{
    fi.file.open(fi.filename);
    if (!fi.file.is_open())
        throw std::runtime_error("Не удалось открыть файл.");

    std::string line;
    if (!std::getline(fi.file, line))
        throw std::runtime_error("Не удалось прочитать файл.");

    fi.columns = findColumnsCount(line);
    fi.colRegNumber = findRegionColumn(line);

    fi.file.seekg(0);
    return true;
}

bool CSVReaderReset(FileInfo& fi)
{
    fi.file.seekg(0);
    return true;
}

bool CSVReaderDisable(FileInfo& fi)
{
    if (fi.file.is_open())
        fi.file.close();
    return true;
}

bool CSVReaderGetRow(FileInfo& fi, IOData& io)
{
    if (!fi.file.is_open())
        throw std::runtime_error("Файл не был открыт");

    std::string line;
    if (std::getline(fi.file, line)) {
        if (findColumnsCount(line) != fi.columns)
            throw std::runtime_error("Поврежденный файл");

        std::stringstream lineStream(line);
        std::vector<std::string> lineArray(fi.columns);
        for (int i = 0; i <= fi.columns - 1; i++) {
            std::string temp;
            std::getline(lineStream, temp, ',');
            temp = trimString(temp);
            lineArray[i] = temp;
        }

        io.rowValues = lineArray;
        return true;
    }

    return false;
}

bool MetricsCalcMaximum(IOData& io)
{
    std::vector<double>& values = io.rowDoubleValues;
    double& result = io.metricResult;

    if (!values.empty()) {
        result = values[0];

        int size = values.size();
        for (int i = 0; i <= size - 1; i++)
            if (values[i] > result)
                result = values[i];

        return true;
    }

    return false;
}

bool MetricsCalcMinimum(IOData& io)
{
    std::vector<double>& values = io.rowDoubleValues;
    double& result = io.metricResult;

    if (!values.empty()) {
        result = values[0];

        int size = values.size();
        for (int i = 0; i <= size - 1; i++)
            if (values[i] < result)
                result = values[i];

        return true;
    }

    return false;
}

bool MetricsCalcMedian(IOData& io)
{
    std::vector<double>& values = io.rowDoubleValues;
    double& result = io.metricResult;

    if (!values.empty()) {
        result = values[0];
        sortVector(values, values.size());

        if (values.size() % 2 == 1)
            result = values[values.size() / 2];
        else
            result = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;

        return true;
    }

    return false;
}

bool CSVHelperFrontController(int action, FileInfo* fi = nullptr, IOData* io = nullptr)
{
    bool status = false;

    if (action == READER_ENABLE && !(fi == nullptr))
        status = CSVReaderEnable(*fi);
    else if (action == READER_RESET && !(fi == nullptr))
        status = CSVReaderReset(*fi);
    else if (action == READER_DISABLE && !(fi == nullptr))
        status = CSVReaderDisable(*fi);
    else if (action == READER_GETROW && !(fi == nullptr) && !(io == nullptr))
        status = CSVReaderGetRow(*fi, *io);
    else if (action == METRICS_MAXIMUM && !(io == nullptr))
        status = MetricsCalcMaximum(*io);
    else if (action == METRICS_MINIMUM && !(io == nullptr))
        status = MetricsCalcMinimum(*io);
    else if (action == METRICS_MEDIAN && !(io == nullptr))
        status = MetricsCalcMedian(*io);
    else
        throw std::runtime_error("Не удается выполнить операцию.");

    return status;
}
