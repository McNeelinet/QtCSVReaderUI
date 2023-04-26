#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "csvreader.h"
#include "tablemanager.h"
#include <QMessageBox>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_open_file, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(ui->btn_load_data, SIGNAL(clicked()), this, SLOT(loadData()));
    connect(ui->btn_calc_metrics, SIGNAL(clicked()), this, SLOT(calcMetrics()));
}

void MainWindow::openFileDialog()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "CSV таблица (*.csv)");

    if (!filename.isEmpty()) {
        this->ui->le_filename->setText(filename);
        this->ui->le_region->setEnabled(true);
        this->ui->btn_load_data->setEnabled(true);
        this->ui->le_column->clear();
        this->ui->le_column->setEnabled(false);
        this->ui->btn_calc_metrics->setEnabled(false);
        this->ui->le_maximum->clear();
        this->ui->le_minimum->clear();
        this->ui->le_median->clear();
        TableClear(this->ui->table_display);
    }
}

void MainWindow::loadData()
{
    CSVReader reader = {.filename = this->ui->le_filename->text().toStdString()};
    CSVHelperInput input = {.reader = &reader};
    TableClear(this->ui->table_display);
    bool success = true;

    try {
        CSVHelperFrontController(input, READER_ENABLE);
        TableFill(this->ui->table_display, reader, this->ui->le_region->text().toStdString());
    }
    catch(std::runtime_error& e) {
        TableClear(this->ui->table_display);
        QMessageBox::critical(this, "Ошибка чтения", e.what());
        success = false;
    }

    this->ui->le_column->setEnabled(success);
    this->ui->btn_calc_metrics->setEnabled(success);
    this->ui->le_column->clear();
    this->ui->le_maximum->clear();
    this->ui->le_minimum->clear();
    this->ui->le_median->clear();

    CSVHelperFrontController(input, READER_DISABLE);
}

void MainWindow::calcMetrics()
{
    CSVHelperInput input;
    CSVHelperOutput output;
    bool success = true;

    try {
        std::vector<double> columnFloats = TableGetColumnFloats(this->ui->table_display, this->ui->le_column->text());
        input = {.columnFloats = columnFloats};
    }
    catch (std::exception& e) {
        QMessageBox::critical(this, "Расчет не удался", e.what());
        success = false;
    }

    for (int i : std::vector<int>{METRICS_MAXIMUM, METRICS_MINIMUM, METRICS_MEDIAN}) {
        output = CSVHelperFrontController(input, i);
        QLineEdit* le = qobject_cast<QLineEdit*>(this->ui->hl_results->itemAt(i)->widget());
        if (output.status)
            le->setText(QString::number(output.metricResult));
        else
            le->setText("Failure");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

