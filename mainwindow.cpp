#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow),
    view(new HistogramView(this))
{
    ui->setupUi(this);
    this->ui->verticalLayout->insertWidget(0,this->view);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->view;
}
