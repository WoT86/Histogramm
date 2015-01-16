#include "histogramview.h"

HistogramView::HistogramView(QWidget *parent) :
    QWidget(parent),
    backgroundColor(Qt::white),
    gridPen(Qt::gray,1,Qt::DashLine),
    gridSize(20),
    gridEnabled(true),
    scaleEnabled(true),
    scaleHeight(10),
    scaleType(COLORCODE),
    data(NULL),
    largestBinSize(0)
{
}

HistogramView::~HistogramView()
{
}

void HistogramView::setBackgroundColor(const QColor &color)
{
    this->backgroundColor = color;
    this->update();
}

void HistogramView::setGridEnabled(bool enable)
{
    this->gridEnabled = enable;
    this->update();
}

void HistogramView::setGridPen(const QPen &pen)
{
    this->gridPen = pen;
    this->update();
}

void HistogramView::setGridSquareSize(int size)
{
    this->gridSize = size;
    this->update();
}

void HistogramView::setScaleEnabled(bool enabled)
{
    this->scaleEnabled = enabled;
    this->update();
}

void HistogramView::setScaleHeight(int height)
{
    this->scaleHeight = height;
    this->update();
}

void HistogramView::setScaleType(HistogramView::ScaleType type)
{
    this->scaleType = type;
    this->update();
}

void HistogramView::setData(HistogramData *data)
{
    if(data)
    {
        if(this->data)
            disconnect(this->data,SIGNAL(updated()),this,SLOT(dataUpdated()));

        this->data = data;
        this->keys = data->getKeys();
        this->keyState.clear();
        this->keyState.fill(true,this->keys.size());
        this->keyBrush.clear();
        this->keyPlotType.clear();

        int i = 4;

        for(int k = 0; k < this->keys.size();k++)
        {
            QColor color(i);
            color.setAlpha(200);
            this->keyBrush.append(QBrush(color));
            this->keyPlotType.append(RELATIVE);
            i++;
            if(i == 19)
                i = 4;
        }

        connect(data,SIGNAL(updated()),this,SLOT(dataUpdated()));
    }
    this->dataUpdated();
}

void HistogramView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    //Background
    painter.setBrush(QBrush(this->backgroundColor,Qt::SolidPattern));
    painter.drawRect(this->rect());

    //Grid
    if(this->gridEnabled)
        this->drawGrid();

    //Scale
    if(this->scaleEnabled)
        this->drawScale();

    //Plot
    this->drawPlot();
}

void HistogramView::drawGrid()
{
    QPainter painter(this);
    painter.setPen(this->gridPen);

    //Vertical Lines
    for(int x = 1;x <= this->rect().width();x += this->gridSize)
    {
        if(x <= this->rect().width())
            painter.drawLine(x,1,x,this->rect().height()-1);
    }

    //Horizontal Lines
    for(int y = 1;y <= this->rect().height();y += this->gridSize)
    {
        if(y <= this->rect().height())
            painter.drawLine(1,y,this->rect().width()-1,y);
    }
}

void HistogramView::drawScale()
{
    QPainter painter(this);

    if(this->scaleType == NUMERIC)
    {
        //TODO
    }
    else
    {
        if(this->scaleType == COLORCODE)
        {
            QLinearGradient gradient(0,0,this->rect().width(),0);

            gradient.setColorAt(0,Qt::white);
            gradient.setColorAt(1,Qt::black);
            painter.setBrush(gradient);
            painter.drawRect(0,this->rect().height()-this->scaleHeight,this->rect().width(),this->scaleHeight);
        }
    }
}

void HistogramView::drawPlot()
{
    if(this->data)
    {
        if(this->data->isValid())
        {
            qreal binWidth = (qreal) this->rect().width() / (qreal) this->data->getNumberOfBins();
            int i = 0; //iterates through keyState

            foreach (int key, this->keys)
            {
                if(this->keyState[i]) //if plot enabled proceed
                {
                    QPolygonF poly;
                    quint64 div = 0;

                    int viewHeight = this->rect().height() - this->scaleHeight;
                    const HistogramData::Bins* bins = this->data->getBins(key);
                    int x = 0;

                    switch(this->keyPlotType[i])
                    {
                    case ABSOLUTE:
                        div = this->data->getNumberOfSamples();
                        break;
                    case RELATIVE:
                        div = this->largestBinSize;
                        break;
                    case RELATIVEEACHKEY:
                        div = this->data->getBinMax(key);
                        break;
                    }

                    if(bins->first() != 0)      //otherwise the polygon looks strange
                        poly << QPointF(0,viewHeight);

                    foreach (unsigned int y, *bins)
                    {
                        poly << QPointF(x*binWidth,viewHeight - (y*viewHeight/div));
                        x++;
                    }

                    if(bins->last() != 0) //otherwise the polygon looks strange
                        poly << QPointF(this->rect().width(),viewHeight);

                    QPainter paintPlot(this);
                    paintPlot.setBrush(this->keyBrush[i]);
                    paintPlot.drawPolygon(poly);
                }

                i++;
            }
        }
    }
}

void HistogramView::dataUpdated()
{
    this->largestBinSize = 0;

    if(this->data)
    {
        foreach(int key,this->keys) //determins which key got the largest bin
        {
            this->largestBinSize = (this->largestBinSize < this->data->getBinMax(key)) ? this->data->getBinMax(key) : this->largestBinSize;
        }
    }

    this->update();
}

bool HistogramView::toggleKey(int key)
{
    int i = this->keys.indexOf(key);

    if(i > -1)
    {
        this->keyState[i] = !(this->keyState[i]);
        this->update();
        return true;
    }

    return false;
}

bool HistogramView::setKeyPlotStyle(int key, const QColor &color)
{
    int i = this->keys.indexOf(key);

    if(i > -1)
    {
        this->keyBrush[i] = QBrush(color);
        this->update();
        return true;
    }

    return false;
}

bool HistogramView::setKeyPlotStyle(int key, const QBrush& brush)
{
    int i = this->keys.indexOf(key);

    if(i > -1)
    {
        this->keyBrush[i] = brush;
        this->update();
        return true;
    }

    return false;
}

bool HistogramView::setKeyPlotType(int key, HistogramView::PlotType type)
{
    int i = this->keys.indexOf(key);

    if(i > -1)
    {
        this->keyPlotType[i] = type;
        this->update();
        return true;
    }

    return false;
}
