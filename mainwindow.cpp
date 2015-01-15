#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow),
    view(new HistogramView(this)),
    data(NULL)
{
    ui->setupUi(this);
    this->ui->verticalLayout->insertWidget(0,this->view);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->view;
}

void MainWindow::on_buttonLoad_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"Choose Image...",QString(),"Images (*.png *.jpg *.tiff *.bmp)");
    QImage image(file);

    this->ui->labelFile->setText(file);

    if(this->data)
        delete this->data;
    this->data = new RGBHistogramData(&image);
    this->data->calculate(256);

    this->view->setData(this->data);
    this->view->setKeyColor(RGBHistogramData::RED,QColor(Qt::red));
    this->view->setKeyColor(RGBHistogramData::GREEN,QColor(Qt::green));
    this->view->setKeyColor(RGBHistogramData::BLUE,QColor(Qt::blue));
}

void MainWindow::on_checkBoxRed_clicked()
{
    this->view->toggleKey(RGBHistogramData::RED);
}

void MainWindow::on_checkBoxGreen_clicked()
{
    this->view->toggleKey(RGBHistogramData::GREEN);
}

void MainWindow::on_checkBoxBlue_clicked()
{
    this->view->toggleKey(RGBHistogramData::BLUE);
}
