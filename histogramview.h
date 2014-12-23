#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QLinearGradient>

class HistogramView : public QWidget
{
    Q_OBJECT
public:
    enum ScaleType
    {
        NUMERIC,COLORCODE
    };

public:
    explicit HistogramView(QWidget *parent = 0);
    ~HistogramView();

    void setBackgroundColor(const QColor& color);

    void setGridEnabled(bool enable);
    void setGridPen(const QPen& pen);
    void setGridSquareSize(int size);

    void setScaleEnabled(bool enabled);
    void setScaleHeight(int height);
    void setScaleType(ScaleType type);

protected:
    void paintEvent(QPaintEvent* event);

    void drawGrid();
    void drawScale();

signals:

public slots:

protected:
    QColor  backgroundColor;

    QPen    gridPen;
    int     gridSize;
    bool    gridEnabled;

    bool        scaleEnabled;
    int         scaleHeight;
    ScaleType   scaleType;
};

#endif // HISTOGRAMVIEW_H
