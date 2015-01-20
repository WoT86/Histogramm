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
    qDeleteAll(this->plotList);
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
            disconnect(this->data,SIGNAL(dataUpdated()),this,SLOT(dataUpdated()));

        this->data = data;
        this->plotList.clear();

        int i = 4, z = 0;

        foreach(int key, this->data->getKeys())
        {
            QColor color(i);
            color.setAlpha(200);

            Plot* nP = new Plot(this,this->data,key);
            this->plotList.append(nP);
            this->plotMap.insert(key,nP);
            nP->setBrush(QBrush(color));
            nP->setZPos(z);

            i++;
            z++;
            if(i == 19)
                i = 4;
        }

        qSort(this->plotList);

        this->frontPlot = this->plotList.last();

        connect(data,SIGNAL(dataUpdated()),this,SLOT(dataUpdated()));
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
    this->drawPlot();
}

void HistogramView::resizeEvent(QResizeEvent *event)
{
    this->updatePlots();

    QWidget::resizeEvent(event);
}

void HistogramView::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = event->localPos();

    foreach(Plot* plot,this->plotList)
    {
        if(plot->getPolygon()->containsPoint(pos,Qt::OddEvenFill))
            this->setFrontPlot(plot->key());
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
        QFont font;
        QString max, half;
        font.setPixelSize(this->scaleHeight);
        QFontMetrics fm(font);

        painter.setFont(font);
        painter.setBrush(QBrush(Qt::white));

        painter.drawRect(0,this->rect().height()-this->scaleHeight,this->rect().width(),this->scaleHeight);

        if(this->plotList.isEmpty())
        {
            painter.setPen(QPen(Qt::black));
            max = "n";
        }
        else
        {
            painter.setPen(QPen(this->frontPlot->getBrush()->color()));
            half = QString::number((quint64) (this->frontPlot->getScaleMax()/2));
            max = QString::number(this->frontPlot->getScaleMax()-1);
        }

        painter.drawText(1,this->rect().height()-1,"0");
        painter.drawText(((int) (this->rect().width()/2))-((int)fm.width(max)/2),this->rect().height()-1,half);
        painter.drawText(this->rect().width()-fm.width(max)-1,this->rect().height()-1,max);

    }
    else
    {
        if(this->scaleType == COLORCODE)
        {
            QLinearGradient gradient(0,0,this->rect().width(),0);

            gradient.setColorAt(0,Qt::white);

            if(this->plotList.isEmpty())
                gradient.setColorAt(1,Qt::black);
            else
                gradient.setColorAt(1,this->frontPlot->getBrush()->color());

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
            foreach (Plot *plot, this->plotList)
            {
                if(plot->isEnabled() && plot->isValid())
                {
                    QPainter paintPlot(this);
                    paintPlot.setBrush(*(plot->getBrush()));
                    paintPlot.drawPolygon(*plot);
                }
            }
        }
    }
}

void HistogramView::updateMax()
{
    this->largestBinSize = 0;

    if(this->data)
    {
        foreach(int key,this->data->getKeys()) //determins which key got the largest bin
        {
            this->largestBinSize = (this->largestBinSize < this->data->getBinMax(key)) ? this->data->getBinMax(key) : this->largestBinSize;
        }
    }
}

void HistogramView::updatePlots()
{
    foreach(Plot* plot, this->plotList)
    {
        plot->update();
    }
}

void HistogramView::dataUpdated()
{  
    this->updateMax();
    this->updatePlots();
    this->update();
}

bool HistogramView::toggleKey(int key)
{
    if(this->plotMap.contains(key))
    {
        Plot* plot = this->plotMap[key];
        bool old = plot->isEnabled();
        plot->enable(!(old));

        for(int i = this->plotList.size()-1;i > 0;i--) //updates the front plot pointer, otherwise drawScale works with wrong limits
        {
            if(this->plotList[i]->isEnabled())
            {
                this->frontPlot = this->plotList[i];
                break;
            }
        }

        this->update();
        return true;
    }

    return false;
}

bool HistogramView::setKeyPlotStyle(int key, const QColor &color)
{
    if(this->plotMap.contains(key))
    {
        this->plotMap[key]->setBrush(QBrush(color));
        this->update();
        return true;
    }

    return false;
}

bool HistogramView::setKeyPlotStyle(int key, const QBrush& brush)
{
    if(this->plotMap.contains(key))
    {
        this->plotMap[key]->setBrush(brush);
        this->update();
        return true;
    }

    return false;
}

bool HistogramView::setKeyPlotType(int key, HistogramView::PlotType type)
{
    if(this->plotMap.contains(key))
    {
        this->plotMap[key]->setType(type);
        this->updatePlots();
        this->update();
        return true;
    }

    return false;
}

void HistogramView::setFrontPlot(int key)
{
    if(this->plotMap.contains(key) && this->plotList.size() > 1)
    {
        Plot* keyP = this->plotMap[key], *swapP = this->plotList.last();
        int old_pos = keyP->zpos(), new_pos = swapP->zpos(), old_index = this->plotList.indexOf(keyP);

        keyP->setZPos(new_pos);
        swapP->setZPos(old_pos);

        this->plotList.swap(this->plotList.size()-1,old_index);

        this->frontPlot = keyP;

        this->update();
    }
}

HistogramView::Plot::Plot(const HistogramView *parent, const HistogramData *data, int key):
    _key(key),
    visible(true),
    valid(false),
    _type(RELATIVE),
    scaleMax(0),
    polygon(0),
    brush(),
    data(data),
    parent(parent)
{

}

bool HistogramView::Plot::operator <(const Plot& other)
{
    return this->z_order < other.z_order;
}

HistogramView::Plot::operator QPolygonF()
{
    return (*this->polygon);
}

void HistogramView::Plot::enable(bool toEnable)
{
    this->visible = toEnable;
}

void HistogramView::Plot::setType(HistogramView::PlotType type)
{
    this->_type = type;
}

void HistogramView::Plot::setBrush(const QBrush &brush)
{
    this->brush = brush;
}

void HistogramView::Plot::setZPos(int newZ)
{
    this->z_order = newZ;
}

int HistogramView::Plot::key() const
{
    return this->_key;
}

bool HistogramView::Plot::isEnabled() const
{
    return this->visible;
}

bool HistogramView::Plot::isValid() const
{
    return this->valid;
}

HistogramView::PlotType HistogramView::Plot::type() const
{
    return this->_type;
}

quint64 HistogramView::Plot::getScaleMax() const
{
    return this->scaleMax;
}

const QPolygonF *HistogramView::Plot::getPolygon() const
{
    return this->polygon;
}

const QBrush *HistogramView::Plot::getBrush() const
{
    return (&this->brush);
}

int HistogramView::Plot::zpos() const
{
    return this->z_order;
}

void HistogramView::Plot::update()
{
    if(this->data)
    {
        if(this->data->isValid())
        {
            QRect widgetRect = this->parent->rect();

            int   viewHeight    = widgetRect.height() - this->parent->scaleHeight;
            qreal binWidth      = (qreal) widgetRect.width() / (qreal) this->data->getNumberOfBins();

            quint64 div = 0;

            const HistogramData::Bins* bins = this->data->getBins(this->_key);

            int x = 0;

            switch(this->_type)
            {
            case ABSOLUTE:
                div = this->data->getNumberOfSamples();
                break;
            case RELATIVE:
                if(this->parent->largestBinSize == 0)
                {
                    this->valid = false;
                    return;
                }

                div = this->parent->largestBinSize;
                break;
            case RELATIVEEACHKEY:
                div = this->data->getBinMax(this->_key);
                break;
            }

            if(this->polygon)
                delete this->polygon;

            this->polygon = new QPolygonF();

            if(bins->first() != 0)      //otherwise the polygon looks strange
                (*this->polygon) << QPointF(0,viewHeight);

            foreach (unsigned int y, *bins)
            {
                (*this->polygon) << QPointF(x*binWidth,viewHeight - (y*viewHeight/div));
                x++;
            }

            if(bins->last() != 0) //otherwise the polygon looks strange
                (*this->polygon) << QPointF(widgetRect.width(),viewHeight);

            this->scaleMax = this->data->getBinScale(this->key());
            this->valid = true;
        }
    }

}
