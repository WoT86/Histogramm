#include "histogramdata.h"

HistogramData::HistogramData(QObject *parent):
    QObject(parent),
    numberOfSamples(0)
{
}

quint32 HistogramData::getNumberOfBins() const
{
    return this->numberOfBins;
}

quint64 HistogramData::getBinSize(int key) const
{
    return this->binSize[key];
}

quint64 HistogramData::getBinMax(int key) const
{
    return this->binMax[key];
}

quint64 HistogramData::getBinScale(int key) const
{
    return this->binScale[key];
}

quint64 HistogramData::getNumberOfSamples() const
{
    return this->numberOfSamples;
}

const HistogramData::Bins *HistogramData::getBins(int key) const
{
    return this->binMap.value(key);
}

QList<int> HistogramData::getKeys() const
{
    return this->binMap.keys();
}
