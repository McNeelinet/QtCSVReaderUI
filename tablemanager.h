#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H
#include <QTableWidget>
#include <QString>
#include "csvreader.h"

void TableClear(QTableWidget* table);
void TableDelete(QTableWidget* table);
void TableFill(QTableWidget* table, CSVReader& reader, std::string region);
std::vector<double> TableGetColumnFloats(QTableWidget* table, QString column);

#endif // TABLEMANAGER_H
