#include "histogramview.h"

HistogramView::HistogramView(QWidget *parent) :
    QWidget(parent),
    backgroundColor(Qt::white),
    gridPen(Qt::gray,1,Qt::DashLine),
    gridSize(20),
    gridEnabled(true),
    scaleEnabled(true),
    scaleHeight(10),
    scaleType(COLORCODE)
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
