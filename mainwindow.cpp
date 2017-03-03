#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cscCleanerForm = new CSCCleanerForm(this);
    this->setCentralWidget(cscCleanerForm);
}

MainWindow::~MainWindow()
{
    delete ui;
}
