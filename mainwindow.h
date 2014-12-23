#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "histogramview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    HistogramView *view;
};

#endif // MAINWINDOW_H
