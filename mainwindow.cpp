#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
    connect(ui->le_column, SIGNAL(textChanged(QString)), this, SLOT(clearMetrics()));
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
    FileInfo fi = {.filename = this->ui->le_filename->text().toStdString()};
    TableClear(this->ui->table_display);
    bool success = true;

    try {
        CSVHelperFrontController(READER_ENABLE, &fi);
        TableFill(this->ui->table_display, &fi, this->ui->le_region->text().toStdString());
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

    CSVHelperFrontController(READER_DISABLE, &fi);
}

void MainWindow::calcMetrics()
{
    IOData io;

    try {
        TableGetColumnFloats(this->ui->table_display, &io,  this->ui->le_column->text());
    }
    catch (std::exception& e) {
        QMessageBox::critical(this, "Расчет не удался", e.what());
    }

    for (int i : std::vector<int>{METRICS_MAXIMUM, METRICS_MINIMUM, METRICS_MEDIAN}) {
        QLineEdit* le = qobject_cast<QLineEdit*>(this->ui->hl_results->itemAt(i)->widget());
        if (CSVHelperFrontController(i, nullptr, &io))
            le->setText(QString::number(io.metricResult));
        else
            le->clear();
    }
}

void MainWindow::clearMetrics()
{
    this->ui->le_maximum->clear();
    this->ui->le_minimum->clear();
    this->ui->le_median->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

