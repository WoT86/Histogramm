#include "histogramdata.h"

HistogramData::HistogramData()
{
}

int HistogramData::getNumberOfBins() const
{
    return this->numberOfBins;
}

int HistogramData::getBinSize() const
{
    return this->binSize;
}

unsigned int HistogramData::getBinMax(int key) const
{
    return this->binMax[key];
}

const HistogramData::Bins *HistogramData::getBins(int key) const
{
    return this->binMap.value(key);
}
