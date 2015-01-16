#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow),
    data(NULL),
    image(NULL)
{
    ui->setupUi(this);

    QStringList str;

    for(short n = 1;n < 9;n++)
        str << QString::number(qPow(2,n));

    this->ui->comboBoxNoB->addItems(str);
    this->ui->comboBoxNoB->setCurrentText("256");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonLoad_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"Choose Image...",QString(),"Images (*.png *.jpg *.tiff *.bmp)");

    if(this->image)
        delete this->image;

    this->image = new QImage(file);

    this->ui->labelFile->setText(file);
    this->ui->checkBoxBlue->setChecked(true);
    this->ui->checkBoxRed->setChecked(true);
    this->ui->checkBoxGreen->setChecked(true);

    if(this->data)
        delete this->data;
    this->data = new RGBHistogramData(this->image);
    this->data->calculate(this->ui->comboBoxNoB->currentText().toInt());

    this->ui->histogramView->setData(this->data);
    this->ui->histogramView->setKeyPlotStyle(RGBHistogramData::RED,QColor(Qt::red));
    this->ui->histogramView->setKeyPlotStyle(RGBHistogramData::GREEN,QColor(Qt::green));
    this->ui->histogramView->setKeyPlotStyle(RGBHistogramData::BLUE,QColor(Qt::blue));
}

void MainWindow::on_checkBoxRed_clicked()
{
    this->ui->histogramView->toggleKey(RGBHistogramData::RED);
}

void MainWindow::on_checkBoxGreen_clicked()
{
    this->ui->histogramView->toggleKey(RGBHistogramData::GREEN);
}

void MainWindow::on_checkBoxBlue_clicked()
{
    this->ui->histogramView->toggleKey(RGBHistogramData::BLUE);
}


void MainWindow::on_radioButtonRel_clicked()
{
    this->ui->radioButtonAbs->setChecked(false);
    this->ui->radioButtonRelEach->setChecked(false);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::RED,HistogramView::RELATIVE);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::GREEN,HistogramView::RELATIVE);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::BLUE,HistogramView::RELATIVE);
}

void MainWindow::on_radioButtonAbs_clicked()
{
    this->ui->radioButtonRel->setChecked(false);
    this->ui->radioButtonRelEach->setChecked(false);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::RED,HistogramView::ABSOLUTE);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::GREEN,HistogramView::ABSOLUTE);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::BLUE,HistogramView::ABSOLUTE);
}

void MainWindow::on_radioButtonRelEach_clicked()
{
    this->ui->radioButtonRel->setChecked(false);
    this->ui->radioButtonAbs->setChecked(false);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::RED,HistogramView::RELATIVEEACHKEY);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::GREEN,HistogramView::RELATIVEEACHKEY);
    this->ui->histogramView->setKeyPlotType(RGBHistogramData::BLUE,HistogramView::RELATIVEEACHKEY);
}

void MainWindow::on_buttonCalc_clicked()
{
    this->ui->buttonCalc->setEnabled(false);
    if(this->data)
        this->data->calculate(this->ui->comboBoxNoB->currentText().toInt());
    this->ui->buttonCalc->setEnabled(true);
}
