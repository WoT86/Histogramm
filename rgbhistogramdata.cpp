#include "rgbhistogramdata.h"

RGBHistogramData::RGBHistogramData(const QImage *image):
    image(image),
    valid(false)
{
    this->binSize[RED] = 0;
    this->binSize[GREEN] = 0;
    this->binSize[BLUE] = 0;
    this->binMap.insert(RED,new Bins);
    this->binMap.insert(GREEN,new Bins);
    this->binMap.insert(BLUE,new Bins);

    this->binMax.insert(RED,0);
    this->binMax.insert(GREEN,0);
    this->binMax.insert(BLUE,0);

    this->numberOfSamples = image->height() * image->width();
}

RGBHistogramData::~RGBHistogramData()
{
    delete this->binMap.value(RED);
    delete this->binMap.value(BLUE);
    delete this->binMap.value(GREEN);
}

bool RGBHistogramData::isValid()
{
    return this->valid;
}

void RGBHistogramData::calculate(int numberOfBins)
{
    quint64 cDRed = 0,cDGreen = 0,cDBlue = 0;
    this->numberOfBins = numberOfBins;

    switch(this->image->depth())
    {
    case 8:
        cDRed = qPow(2,3);
        cDGreen = qPow(2,3);
        cDBlue = qPow(2,2);
        break;
    case 16:
        cDRed = qPow(2,5);
        cDGreen = qPow(2,6);
        cDBlue = qPow(2,5);
        break;
    case 24:
        cDRed = qPow(2,8);
        cDGreen = qPow(2,8);
        cDBlue = qPow(2,8);
        break;
    case 32:
        cDRed = qPow(2,8);
        cDGreen = qPow(2,8);
        cDBlue = qPow(2,8);
        break;
    }

    if(cDBlue < numberOfBins)           //number of bins is limited to the lowest color depth of all channels
        this->numberOfBins = cDBlue;

    if(this->image->depth() == 1) // 1Bit is ignored
    {
        this->valid = false;
        return;
    }

    if(this->image)
    {
        if(numberOfBins % 2 == 0)
        {
            Bins* bMRed = this->binMap[RED], *bMGreen = this->binMap[GREEN], *bMBlue = this->binMap[BLUE];
            bMRed->fill(0,this->numberOfBins);
            bMGreen->fill(0,this->numberOfBins);
            bMBlue->fill(0,this->numberOfBins);

            this->binSize[RED] = cDRed / this->numberOfBins;
            this->binSize[GREEN] = cDGreen / this->numberOfBins;
            this->binSize[BLUE] = cDBlue / this->numberOfBins;

            int red = 0,green = 0,blue = 0;
            quint64 redMax = 0, greenMax = 0, blueMax = 0;

            for(int x = 0;x < this->image->width();x++)
            {
                for(int y = 0;y < this->image->height();y++)
                {
                    QColor rgb(this->image->pixel(x,y));
                    red = rgb.red() / this->binSize[RED];
                    blue = rgb.blue() / this->binSize[BLUE];
                    green = rgb.green() / this->binSize[GREEN];

                    (*bMRed)[red]+=1;
                    if(bMRed->at(red) > redMax)
                        redMax += 1;

                    (*bMBlue)[blue]+=1;
                    if(bMBlue->at(blue) > blueMax)
                        blueMax += 1;

                    (*bMGreen)[green]+=1;
                    if(bMGreen->at(green) > greenMax)
                        greenMax += 1;
                }
            }

            this->binMax[RED] = redMax;
            this->binMax[BLUE] = blueMax;
            this->binMax[GREEN] = greenMax;

            emit this->updated();
            this->valid = true;
            return;
        }
    }

    this->valid = false;
}
