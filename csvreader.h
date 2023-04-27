#ifndef CSVREADER_H
#define CSVREADER_H
#include <iostream>
#include <fstream>
#include <vector>

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

bool CSVHelperFrontController(int action, FileInfo* = nullptr, IOData* = nullptr);

#endif // CSVREADER_H
