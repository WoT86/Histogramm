#ifndef HISTOGRAMDATA_H
#define HISTOGRAMDATA_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QMap>

class HistogramData : public QObject
{
    Q_OBJECT
public:
    typedef QVector<quint64> Bins;
    typedef QMap<int,Bins*> BinMap;
public:
    HistogramData(QObject* parent = 0);
    virtual ~HistogramData(){};

    virtual bool isValid() = 0;

    quint32 getNumberOfBins() const;
    quint64 getBinSize(int key) const;
    quint64 getBinMax(int key) const;
    quint64 getNumberOfSamples() const;

    const Bins* getBins(int key) const;

    QList<int> getKeys() const;

public slots:
    virtual void calculate(int numberOfBins) = 0;

signals:
    void dataUpdated();

protected:
    QMap<int,Bins*> binMap;
    QMap<int,quint64> binMax;
    QMap<int,quint64> binSize;

    quint64 numberOfSamples;
    quint32 numberOfBins;
};

#endif // HISTOGRAMDATA_H
