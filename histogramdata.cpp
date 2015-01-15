#include "histogramdata.h"

HistogramData::HistogramData()
{
}

int HistogramData::getNumberOfBins() const
{
    return this->numberOfBins;
}

int HistogramData::getBinSize(int key) const
{
    return this->binSize[key];
}

unsigned int HistogramData::getBinMax(int key) const
{
    return this->binMax[key];
}

const HistogramData::Bins *HistogramData::getBins(int key) const
{
    return this->binMap.value(key);
}

QList<int> HistogramData::getKeys() const
{
    return this->binMap.keys();
}
