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
    data(NULL)
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
        this->data = data;
        this->keys = data->getKeys();
        this->keyState.clear();
        this->keyState.fill(true,this->keys.size());
        this->keyColor.clear();

        int i = 4;

        for(int k = 0; k < this->keys.size();k++)
        {
            this->keyColor.append(QColor(i));
            i++;
            if(i == 19)
                i = 4;
        }
    }
    this->update();
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
    if(this->data)
    {
        if(this->data->isValid())
        {
            qreal binWidth = (qreal) this->rect().width() / (qreal) this->data->getNumberOfBins();
            int i = 0; //iterates through keyState
            foreach (int key, this->keys)
            {
                if(this->keyState[i])
                {
                    QPolygonF poly;
                    unsigned int max = this->data->getBinMax(key);
                    const HistogramData::Bins* bins = this->data->getBins(key);

                    int viewHeight = this->rect().height() - this->scaleHeight;

                    int x = 0;

                    if(bins->first() != 0)      //otherwise the polygon looks strange
                        poly << QPointF(0,viewHeight);

                    foreach (unsigned int y, *bins)
                    {
                        poly << QPointF(x*binWidth,viewHeight - (y*viewHeight/max));
                        x++;
                    }

                    if(bins->last() != 0) //otherwise the polygon looks strange
                        poly << QPointF(this->rect().width(),viewHeight);

                    QPainter paintPlot(this);
                    paintPlot.setBrush(QBrush(this->keyColor[i],Qt::SolidPattern));
                    paintPlot.drawPolygon(poly);
                }

                i++;
            }
        }
    }

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

    if(this->scaleType == COLORCODE)
    {
        QLinearGradient gradient(0,0,this->rect().width(),0);

        gradient.setColorAt(0,Qt::white);
        gradient.setColorAt(1,Qt::black);
        painter.setBrush(gradient);
        painter.drawRect(0,this->rect().height()-this->scaleHeight,this->rect().width(),this->scaleHeight);
    }
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

bool HistogramView::setKeyColor(int key, const QColor &color)
{
    int i = this->keys.indexOf(key);

    if(i > -1)
    {
        QColor alpha = color;
        alpha.setAlpha(200);
        this->keyColor[i] = alpha;
        this->update();
        return true;
    }

    return false;
}
