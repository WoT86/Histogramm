#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QThread>

#include "histogramview.h"
#include "rgbhistogramdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_buttonLoad_clicked();

    void on_checkBoxRed_clicked();

    void on_checkBoxGreen_clicked();

    void on_checkBoxBlue_clicked();

    void on_radioButtonRel_clicked();

    void on_radioButtonAbs_clicked();

    void on_radioButtonRelEach_clicked();

    void on_buttonCalc_clicked();

    void on_radioButtonScaleColor_clicked();

    void on_radioButtonScaleNumeric_clicked();

private:
    Ui::MainWindow *ui;
    HistogramData *data;
    QImage* image;

    QThread workerThread;
};

#endif // MAINWINDOW_H
