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
    float getBinWidth(); //
    float getMin();     //  debug
    float getMax();        //
    std::map<qint32,qint32> *histogram;
private:
    void minimax();
    void _middle();
    void hist();
    void disp();
    void stdDev();
    void _middle(float, float);
    void disp(float, float);
    void findConfInterval();
    void hist(float, float);
    QList<qint32> *input;
    float min, max, middle, dispersion, std_deviation, bin_width, lower_bound, upper_bound;
    qint32 confidence_interval, temp, size, filtered_size;
    const qint32 num_of_intervals;
    std::map<qint32,qint32>::iterator iter;
    void histogram_add(qint32);

signals:
    void started();
    void finished();
};

#endif // STATISTIC_H
