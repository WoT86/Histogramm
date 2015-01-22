#include "rgbhistogramdata.h"

RGBHistogramData::RGBHistogramData(const QImage *image, QObject *parent):
    HistogramData(parent),
    image(image),
    valid(false),
    busy(false)
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

    switch(this->image->depth())
    {
    case 8:
        this->binScale[RED] = qPow(2,3);
        this->binScale[GREEN] = qPow(2,3);
        this->binScale[BLUE] = qPow(2,2);
        break;
    case 16:
        this->binScale[RED] = qPow(2,5);
        this->binScale[GREEN] = qPow(2,6);
        this->binScale[BLUE] = qPow(2,5);
        break;
    case 24:
        this->binScale[RED] = qPow(2,8);
        this->binScale[GREEN] = qPow(2,8);
        this->binScale[BLUE] = qPow(2,8);
        break;
    case 32:
        this->binScale[RED] = qPow(2,8);
        this->binScale[GREEN] = qPow(2,8);
        this->binScale[BLUE] = qPow(2,8);
        break;
    }

    this->numberOfSamples = image->height() * image->width();
    this->workerThread.start();
}

RGBHistogramData::~RGBHistogramData()
{
    delete this->binMap.value(RED);
    delete this->binMap.value(BLUE);
    delete this->binMap.value(GREEN);

    this->workerThread.quit();
    this->workerThread.wait();
}

bool RGBHistogramData::isValid() const
{
    return this->valid;
}

bool RGBHistogramData::isBusy() const
{
    return this->busy;
}

void RGBHistogramData::calculate(int numberOfBins)
{
    this->busy = true;
    RGBWorker *worker = new RGBWorker(this->image);
    worker->moveToThread(&(this->workerThread));

    qRegisterMetaType<HistogramData::BinMap>("HistogramData::BinMap");
    qRegisterMetaType<QMap<int,quint64> >("QMap<int,quint64>");

    connect(worker,SIGNAL(dataReady(HistogramData::BinMap,QMap<int,quint64>,QMap<int,quint64>,quint32)),this,SLOT(copyData(HistogramData::BinMap,QMap<int,quint64>,QMap<int,quint64>,quint32)));
    connect(&(this->workerThread),SIGNAL(finished()),worker,SLOT(deleteLater()));

    QMetaObject::invokeMethod(worker,"doWork", Qt::QueuedConnection, Q_ARG(quint32,numberOfBins));
}

void RGBHistogramData::copyData(BinMap data, QMap<int, quint64> max, QMap<int, quint64> size, quint32 numberOfBins)
{
    if(this->binMap.first()) //deletes old vectors
    {
        delete this->binMap[RED];
        delete this->binMap[GREEN];
        delete this->binMap[BLUE];
    }

    this->numberOfBins = numberOfBins;
    this->binMap = data;
    this->binMax = max;
    this->binSize = size;

    this->valid = true;
    this->busy = false;

    emit this->dataUpdated();
}


RGBWorker::RGBWorker(const QImage* img, QObject *parent):
    QObject(parent),
    img(img)
{
    this->binSize[RGBHistogramData::RED] = 0;
    this->binSize[RGBHistogramData::GREEN] = 0;
    this->binSize[RGBHistogramData::BLUE] = 0;
    this->binMap.insert(RGBHistogramData::RED,new HistogramData::Bins);
    this->binMap.insert(RGBHistogramData::GREEN,new HistogramData::Bins);
    this->binMap.insert(RGBHistogramData::BLUE,new HistogramData::Bins);

    this->binMax.insert(RGBHistogramData::RED,0);
    this->binMax.insert(RGBHistogramData::GREEN,0);
    this->binMax.insert(RGBHistogramData::BLUE,0);
}

RGBWorker::~RGBWorker()
{
}

void RGBWorker::doWork(quint32 NumberOfBins)
{
    quint64 cDRed = 0,cDGreen = 0,cDBlue = 0;

    if(this->img)
    {
        switch(this->img->depth())
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

        if(cDBlue < NumberOfBins)           //number of bins is limited to the lowest color depth of all channels
            NumberOfBins = cDBlue;

        if(this->img->depth() == 1) // 1Bit is ignored
        {
            return;
        }

        if(NumberOfBins % 2 == 0)
        {
            HistogramData::Bins* bMRed = this->binMap[RGBHistogramData::RED], *bMGreen = this->binMap[RGBHistogramData::GREEN], *bMBlue = this->binMap[RGBHistogramData::BLUE];
            bMRed->fill(0,NumberOfBins);
            bMGreen->fill(0,NumberOfBins);
            bMBlue->fill(0,NumberOfBins);

            quint64 bSizeRed = cDRed / NumberOfBins, bSizeGreen = cDGreen / NumberOfBins, bSizeBlue = cDBlue / NumberOfBins;

            this->binSize[RGBHistogramData::RED] = bSizeRed;
            this->binSize[RGBHistogramData::GREEN] = bSizeGreen;
            this->binSize[RGBHistogramData::BLUE] = bSizeBlue;

            int red = 0,green = 0,blue = 0;
            quint64 redMax = 0, greenMax = 0, blueMax = 0;

            for(int y = 0;y < this->img->height();y++)
            {
                QRgb *line = (QRgb*) this->img->scanLine(y);

                for(int x = 0;x < this->img->width();x++)
                {
                    red = qRed(*line) / bSizeRed;
                    blue = qBlue(*line) / bSizeBlue;
                    green = qGreen(*line) / bSizeGreen;

                    (*bMRed)[red]+=1;

                    (*bMBlue)[blue]+=1;

                    (*bMGreen)[green]+=1;

                    line++;
                }
            }

            foreach(quint64 value,*bMRed)
            {
                redMax = (value > redMax) ? value : redMax;
            }

            foreach(quint64 value,*bMGreen)
            {
                greenMax = (value > greenMax) ? value : greenMax;
            }

            foreach(quint64 value,*bMBlue)
            {
                blueMax = (value > blueMax) ? value : blueMax;
            }

            this->binMax[RGBHistogramData::RED] = redMax;
            this->binMax[RGBHistogramData::BLUE] = blueMax;
            this->binMax[RGBHistogramData::GREEN] = greenMax;

            emit this->dataReady(this->binMap,this->binMax,this->binSize,NumberOfBins);
            return;
        }
    }
}
