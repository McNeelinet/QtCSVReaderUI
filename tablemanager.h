#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H
#include <QTableWidget>
#include <QString>
#include "csvreader.h"

void TableClear(QTableWidget* table);
void TableDelete(QTableWidget* table);
void TableFill(QTableWidget* table, FileInfo* fi, std::string region);
void TableGetColumnFloats(QTableWidget* table, IOData* io, QString column);

#endif // TABLEMANAGER_H
