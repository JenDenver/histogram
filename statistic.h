#ifndef STATISTIC_H
#define STATISTIC_H
#include <QtGlobal>
#include <QObject>
#include <QMap>
class Statistic : public QObject
{
    Q_OBJECT
public:
    Statistic(QList<qint32>*,qint32, qint32);
    ~Statistic();
    int make_histogram();
    float getMiddle();
    float getStdDeviation();
    std::map<qint32,qint32> *histogram;
private:
    QList<qint32> *input;
    float min, max, middle, dispersion, std_deviation, bin_width, lower_bound, upper_bound;
    qint32 confidence_interval, num_of_intervals, temp, size, filtered_size;
    std::map<qint32,qint32>::iterator iter;
    void histogram_add(qint32);

signals:
    void started();
    void finished();
};

#endif // STATISTIC_H
