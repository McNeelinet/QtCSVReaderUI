#ifndef CSVREADER_H
#define CSVREADER_H
#include <iostream>
#include <fstream>
#include <vector>

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

CSVHelperOutput CSVHelperFrontController(CSVReader& reader, int action);

#endif // CSVREADER_H
