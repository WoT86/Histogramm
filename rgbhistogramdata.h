#ifndef RGBHISTOGRAMDATA_H
#define RGBHISTOGRAMDATA_H

#include <QImage>
#include <QColor>
#include <qmath.h>

#include "histogramdata.h"

class RGBHistogramData : public HistogramData
{
public:
    enum Channel
    {
        RED = 1, BLUE = 2, GREEN = 3
    };

public:
    RGBHistogramData(const QImage* image);
    ~RGBHistogramData();

    bool isValid();

    virtual void calculate(int numberOfBins);

protected:
    const QImage* image;
    bool valid;
};

#endif // RGBHISTOGRAMDATA_H
