#include "rgbhistogramdata.h"

RGBHistogramData::RGBHistogramData(const QImage *image):
    image(image),
    valid(false)
{
    this->binSize = 0;
    this->binMap.insert(RED,new Bins);
    this->binMap.insert(GREEN,new Bins);
    this->binMap.insert(BLUE,new Bins);

    this->binMax.insert(RED,0);
    this->binMax.insert(GREEN,0);
    this->binMax.insert(BLUE,0);
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
    int colorDepth = qPow(2,this->image->depth());

    if(this->image)
    {
        if(numberOfBins % 2 == 0 && numberOfBins <= colorDepth)
        {
            this->numberOfBins = numberOfBins;
            this->binMap[RED]->fill(0,this->numberOfBins);
            this->binMap[GREEN]->fill(0,this->numberOfBins);
            this->binMap[BLUE]->fill(0,this->numberOfBins);

            this->binSize = colorDepth / this->numberOfBins;

            int red = 0,green = 0,blue = 0;

            for(int x = 0;x < this->image->width();x++)
            {
                for(int y = 0;y < this->image->height();y++)
                {
                    QColor rgb(this->image->pixel(x,y));
                    red = rgb.red() / this->binSize;
                    blue = rgb.blue() / this->binSize;
                    green = rgb.green() / this->binSize;

                    this->binMap[RED][red]+=1;
                    this->binMap[RED][blue]+=1;
                    this->binMap[RED][green]+=1;
                }
            }

            this->valid = true;
            return;
        }
    }

    this->valid = false;
}
