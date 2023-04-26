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

void TableFill(QTableWidget* table, CSVReader& reader, std::string region)
{
    CSVHelperInput input = {.reader = &reader};
    CSVHelperFrontController(input, READER_RESET);
    CSVHelperOutput output = CSVHelperFrontController(input, READER_GETROW);

    table->setColumnCount(reader.columnsCount);
    for (int i = 0; i <= reader.columnsCount - 1; i++)
        table->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromStdString(output.row[i]), 0));
    table->horizontalHeader()->show();

    while ((output = CSVHelperFrontController(input, READER_GETROW)).status) {
        if (output.row[reader.regionColumn] != region)
            continue;

        table->insertRow(table->rowCount());
        for (int i = 0; i < reader.columnsCount; i++)
            table->setItem(table->rowCount() - 1, i, new QTableWidgetItem(QString::fromStdString(output.row[i])));
    }
}

std::vector<double> TableGetColumnFloats(QTableWidget* table, QString columnNumberString)
{
    bool isInt;
    int columnNumber = columnNumberString.toInt(&isInt);
    if (!isInt)
        throw std::runtime_error("Номер колонки - не число");

    int columnCount = table->columnCount();
    if (!(columnNumber >= 0 && columnNumber < columnCount))
        throw std::runtime_error("Выход за пределы диапазона");
    int rowCount = table->rowCount();

    std::vector<double> columnF;
    for (int i = 0; i <= rowCount - 1; i++) {
        bool isDouble;
        QString elemText = table->item(i, columnNumber)->text();
        double number = elemText.toDouble(&isDouble);
        if (isDouble)
            columnF.push_back(number);
        else
            throw std::runtime_error("Одно из значений в колонке - не число");
    }

    return columnF;
}
