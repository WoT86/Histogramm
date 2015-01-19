#ifndef RGBHISTOGRAMDATA_H
#define RGBHISTOGRAMDATA_H

#include <QImage>
#include <QColor>
#include <qmath.h>
#include <QThread>

#include "histogramdata.h"

class RGBHistogramData : public HistogramData
{
    Q_OBJECT

public:
    enum Channel
    {
        RED = 1, BLUE = 2, GREEN = 3
    };

public:
    RGBHistogramData(const QImage* image,QObject* parent = 0);
    ~RGBHistogramData();

    bool isValid();
    bool isBusy();

public slots:
    virtual void calculate(int numberOfBins);

protected slots:
    void copyData(HistogramData::BinMap data, QMap<int, quint64> max, QMap<int, quint64> size, quint32 numberOfBins);

protected:
    const QImage* image;
    bool valid;

    QThread workerThread;

    bool busy;
};

class RGBWorker : public QObject
{
    Q_OBJECT

public:
    RGBWorker(const QImage *img, QObject* parent = 0);
    ~RGBWorker();

public slots:
    void doWork(quint32 NumberOfBins);

signals:
    void dataReady(HistogramData::BinMap data,QMap<int, quint64> max, QMap<int, quint64> size,quint32 NumberOfBins);

private:
    HistogramData::BinMap binMap;
    QMap<int,quint64> binMax;
    QMap<int,quint64> binSize;

    const QImage* img;
};

#endif // RGBHISTOGRAMDATA_H
