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
    READER_GETROW
};

CSVHelperOutput CSVHelperFrontController(CSVHelperInput& input, int action);

#endif // CSVREADER_H
