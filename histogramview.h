#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QLinearGradient>
#include <QList>
#include <QVector>
#include <QFontMetrics>
#include <QMouseEvent>

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

protected:
    //for internal use only
    //stores plot data
    class Plot
    {
    public:
        Plot(const HistogramView* parent, const HistogramData* data, int key);

        operator QPolygonF();

        bool operator<(const Plot& other);

        void enable(bool toEnable);
        void setType(PlotType type);
        void setBrush(const QBrush& brush);
        void setZPos(int newZ);

        int         key() const;
        bool        isEnabled() const;
        bool        isValid() const;
        PlotType    type() const;

        quint64             getScaleMax() const;
        const QPolygonF*    getPolygon() const;
        const QBrush*       getBrush() const;

        int zpos() const;

        void update();

    private:
        const int   _key;
        bool        visible;
        bool        valid;
        PlotType    _type;
        quint64     scaleMax;

        QPolygonF*  polygon;
        QBrush      brush;
        int         z_order;

        const HistogramData* data;
        const HistogramView* parent;
    };
    //end of class plot

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

    void setFrontPlot(int key);

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent* event);

    void drawGrid();
    void drawScale();
    void drawPlot();

    void updateMax();
    void updatePlots();

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

    QHash<int,Plot*> plotMap;    //for access by key
    QList<Plot*>    plotList;    //ordered by z-pos (highest is front)
    Plot*           frontPlot;   //Plot in front, ignores invisible plots

    quint64 largestBinSize;
};

#endif // HISTOGRAMVIEW_H
