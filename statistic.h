#ifndef STATISTIC_H
#define STATISTIC_H
#include <QtGlobal>
#include <QObject>
#include <QMap>
class Statistic : public QObject
{
    Q_OBJECT
public:
    Statistic(qint32, qint32, QList<qint32>*);
                                        //1 arg - num of sigmas for confidence interval[1..3](filtration)
                                        //2 arg - num of bins(intervals) for histogram
                                        //3 arg - input data
    ~Statistic();
    std::map<qint32,qint32> *histogram; //store histogram

    float average();                    //find average
    float filteredAverage();            //filter input and find average
    float std_deviation();              //stdDev for input
    float filtered_std_deviation();     //stdDev for filtered input
    void  make_histogram();             //build histogram from input
    void  filtered_histogram();         //build histogram from filtered input
    float getBinWidth();                //default binwidth = 0

private:
    void minimax();                     //find min and max
    void findConfInterval();            //find confidence interval
    void histogram_add(qint32);         //for building histogram
    void f_histogram_add(qint32);       //for building filtered histogram
    void dispersion();                  //for internal use
    void f_dispersion();                //for internal use
    void f_hist();                      //for internal use

    qint32 confidence_interval, num_of_intervals, temp, filtered_size;
    float min, max, middle, _dispersion, stdDev, bin_width = 0, lower_bound, upper_bound;
    QList<qint32> *input;
    std::map<qint32,qint32>::iterator iter;

signals:
    void started();
    void finished();
};

#endif // STATISTIC_H
