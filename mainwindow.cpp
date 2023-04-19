#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "csvreader.h"
#include "tablemanager.h"
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
        this->ui->le_column->setEnabled(false);
        this->ui->btn_calc_metrics->setEnabled(false);
        TableDelete(this->ui->table_display);
    }
}

void MainWindow::loadData()
{
    CSVReader reader = {.filename = this->ui->le_filename->text().toStdString()};
    CSVHelperInput input = {.reader = &reader};
    CSVHelperFrontController(input, READER_ENABLE);

    TableClear(ui->table_display);
    TableFill(ui->table_display, reader, this->ui->le_region->text().toStdString());

    // Если количество рядов 0, то...

    this->ui->le_column->setEnabled(true);
    this->ui->le_maximum->clear();
    this->ui->le_minimum->clear();
    this->ui->le_median->clear();
    this->ui->btn_calc_metrics->setEnabled(true);
    CSVHelperFrontController(input, READER_DISABLE);
}

void MainWindow::calcMetrics()
{
    std::vector<double> columnFloats = TableGetColumnFloats(this->ui->table_display, this->ui->le_column->text());
    CSVHelperInput input = {.columnFloats = columnFloats};

    CSVHelperOutput output;

    output = CSVHelperFrontController(input, METRICS_MAXIMUM);
    if (output.status)
        this->ui->le_maximum->setText(QString::number(output.metricResult));
    else
        this->ui->le_maximum->setText("Failure");

    output = CSVHelperFrontController(input, METRICS_MINIMUM);
    if (output.status)
        this->ui->le_minimum->setText(QString::number(output.metricResult));
    else
        this->ui->le_minimum->setText("Failure");

    output = CSVHelperFrontController(input, METRICS_MEDIAN);
    if (output.status)
        this->ui->le_median->setText(QString::number(output.metricResult));
    else
        this->ui->le_median->setText("Failure");
}

MainWindow::~MainWindow()
{
    delete ui;
}

