#ifndef CSVREADER_H
#define CSVREADER_H
#include <iostream>
#include <fstream>
#include <vector>

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
    READER_ENABLE,
    READER_RESET,
    READER_DISABLE,
    READER_GETROW,
    METRICS_MAXIMUM,
    METRICS_MINIMUM,
    METRICS_MEDIAN
};

CSVHelperOutput CSVHelperFrontController(CSVHelperInput& input, int action);

#endif // CSVREADER_H
