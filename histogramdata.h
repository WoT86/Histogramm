#ifndef HISTOGRAMDATA_H
#define HISTOGRAMDATA_H

#include <QVector>
#include <QMap>

class HistogramData
{
public:
    typedef QVector<unsigned int> Bins;
public:
    HistogramData();

    virtual bool isValid() = 0;

    virtual void calculate(int numberOfBins) = 0;

    int getNumberOfBins() const;
    int getBinSize() const;
    unsigned int getBinMax(int key) const;

    const Bins* getBins(int key) const;

protected:
    QMap<int,Bins*> binMap;
    QMap<int,unsigned int> binMax;
    int numberOfBins;
    int binSize;
};

#endif // HISTOGRAMDATA_H
