#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidget>
#include <QString>
#include <vector>
#include "csvreader.h"

void TableClear(QTableWidget* table)
{
    table->setRowCount(0);
    table->horizontalHeader()->hide();
}

void TableFill(QTableWidget* table, FileInfo* fileinfo, std::string region)
{
    IOData io;
    FileInfo& fi = *fileinfo;
    CSVHelperFrontController(READER_RESET, &fi);
    CSVHelperFrontController(READER_GETROW, &fi, &io);

    table->setColumnCount(fi.columns);
    for (int i = 0; i <= fi.columns - 1; i++)
        table->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromStdString(io.rowValues[i]), 0));
    table->horizontalHeader()->show();

    while (CSVHelperFrontController(READER_GETROW, &fi, &io)) {
        if (io.rowValues[fi.colRegNumber] != region)
            continue;

        table->insertRow(table->rowCount());
        for (int i = 0; i < fi.columns; i++)
            table->setItem(table->rowCount() - 1, i, new QTableWidgetItem(QString::fromStdString(io.rowValues[i])));
    }
}

void TableGetColumnFloats(QTableWidget* table, IOData* inputoutput, QString column)
{
    bool isInt;
    int columnNumber = column.toInt(&isInt);
    if (!isInt)
        throw std::runtime_error("Номер колонки - не число");

    int columnCount = table->columnCount();
    if (!(columnNumber >= 0 && columnNumber < columnCount))
        throw std::runtime_error("Выход за пределы диапазона");
    int rowCount = table->rowCount();

    std::vector<double> columnF;
    for (int i = 0; i <= rowCount - 1; i++) {
        bool isDouble;
        double number = table->item(i, columnNumber)->text().toDouble(&isDouble);
        if (isDouble)
            columnF.push_back(number);
    }

    inputoutput->rowDoubleValues = columnF;
}
