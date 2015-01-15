#ifndef HISTOGRAMDATA_H
#define HISTOGRAMDATA_H

#include <QVector>
#include <QList>
#include <QMap>

class HistogramData
{
public:
    typedef QVector<unsigned int> Bins;
public:
    HistogramData();
    virtual ~HistogramData(){};

    virtual bool isValid() = 0;

    virtual void calculate(int numberOfBins) = 0;

    int getNumberOfBins() const;
    int getBinSize(int key) const;
    unsigned int getBinMax(int key) const;

    const Bins* getBins(int key) const;

    QList<int> getKeys() const;
protected:
    QMap<int,Bins*> binMap;
    QMap<int,unsigned int> binMax;
    QMap<int,int> binSize;
    int numberOfBins;
};

#endif // HISTOGRAMDATA_H
