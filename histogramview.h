#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QLinearGradient>
#include <QList>
#include <QVector>

#include "histogramdata.h"

class HistogramView : public QWidget
{
    Q_OBJECT
public:
    enum ScaleType
    {
        NUMERIC,COLORCODE
    };

    enum PlotType
    {
        ABSOLUTE,RELATIVE, RELATIVEEACHKEY
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

    void setData(HistogramData* data);

    bool toggleKey(int key);

    bool setKeyPlotStyle(int key, const QColor& color);
    bool setKeyPlotStyle(int key, const QBrush& brush);

    bool setKeyPlotType(int key, PlotType type);

protected:
    void paintEvent(QPaintEvent* event);

    void drawGrid();
    void drawScale();
    void drawPlot();

signals:

protected slots:
    void dataUpdated();

protected:
    QColor  backgroundColor;

    QPen    gridPen;
    int     gridSize;
    bool    gridEnabled;

    bool        scaleEnabled;
    int         scaleHeight;
    ScaleType   scaleType;

    HistogramData* data;

    QList<int>      keys;
    QVector<bool>   keyState;
    QVector<QBrush> keyBrush;
    QVector<PlotType> keyPlotType;

    quint64 largestBinSize;
};

#endif // HISTOGRAMVIEW_H
