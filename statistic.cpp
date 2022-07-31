#include "statistic.h"
#include <QDebug>       //del!
#include <QTime>           //del!
Statistic::Statistic(QList<qint32> *data, qint32 conf, qint32 num):
    input(data), confidence_interval(conf), num_of_intervals(num)
{                                                         //confidence_interval = num of sigmas(1-3)
    histogram = new std::map<qint32,qint32>();
    make_histogram();
}
Statistic::~Statistic()
{
    delete histogram;
}
/*int Statistic::make_histogram()
{
    emit started();
    clock_t start = clock();      //del!
    middle=0; dispersion=0; std_deviation=0;
    size = input->size();
    assert(num_of_intervals>0 && "num of intervals must be >0");
    if (input==nullptr || input->isEmpty())
        return 1;
    else
    {
        //step 1: find min, max, middle
        min = input->first();                             //prepare min and max vars
        max = input->first();
        for (auto x:*input)
        {
            middle+=x;                                    //accumulating middle
            if (x<min) min = x;                           //find maximum value
            if (x>max) max = x;                           //find minimum value
        }
        middle /= size;                                   //find middle value

        //step 2: build histogram, find dispersion
        for (int i=1;i<=num_of_intervals;i++)             //preparing map
            histogram->operator[](i)=0;
        bin_width = (max-min)/num_of_intervals;
        assert(bin_width>0 && "bin width <=0");
        for (auto x:*input)
        {
            dispersion += (x-middle)*(x-middle);          //accumulating dispersion
            histogram_add(x);                             //add element in histogram
        }
        assert(size>1 && "input size <=1");
        dispersion /= size-1;                             //find dispersion

        //step 3:find standard deviation and confidence interval
        std_deviation = sqrt(dispersion);
        assert(confidence_interval>0 && confidence_interval<4);
        lower_bound = middle - confidence_interval*std_deviation;      //3-sigma rule
        upper_bound = middle + confidence_interval*std_deviation;

        //step 4: find middle for filtered interval
        middle = 0; filtered_size = 0;
        for (auto x:*input)
        {
            if (x>=lower_bound && x<=upper_bound)
            {
                middle+=x;
                filtered_size++;                          //count ammount of elements in condfidence interval
            }
        }
        middle/=filtered_size;

        //step 5: find dispersion for filtered interval
        dispersion = 0;
        for (auto x:*input)
        {
            if (x>=lower_bound && x<=upper_bound)
                dispersion += (x-middle)*(x-middle);
        }
        dispersion /= filtered_size-1;

        //step 6: find std_deviation for filtered interval
        std_deviation = sqrt(dispersion);
        clock_t fin = clock();       //del!
        qDebug() << "seconds used: " << ((double)(fin-start)) / CLOCKS_PER_SEC; //del!
    }
    emit Statistic::finished();
    return 0;
}*/
int Statistic::make_histogram()
{
    emit started();
    clock_t start = clock();      //del!
    middle=0; dispersion=0; std_deviation=0;
    size = input->size();
    assert(num_of_intervals>0 && "num of intervals must be >0");
    if (input==nullptr || input->isEmpty())
        return 1;
    else
    {
        min = input->first();                             //prepare min and max vars
        max = input->first();
        for (auto x:*input)
        {
            if (x<min) min = x;                           //find maximum value
            if (x>max) max = x;                           //find minimum value
        }
        for (auto x:*input)
        {
            middle+=x;
        }
        middle /= size;                                   //find middle value

        for (int i=1;i<=num_of_intervals;i++)             //preparing map
            histogram->operator[](i)=0;
        bin_width = (max-min)/num_of_intervals;
        assert(bin_width>0 && "bin width <=0");
        for (auto x:*input)
        {
            histogram_add(x);
        }
        for (auto x:*input)
        {
            dispersion += (x-middle)*(x-middle);          //accumulating dispersion
        }
        assert(size>1 && "input size <=1");
        dispersion /= size-1;                             //find dispersion

        //step 3:find standard deviation and confidence interval
        std_deviation = sqrt(dispersion);
        assert(confidence_interval>0 && confidence_interval<4);
        lower_bound = middle - confidence_interval*std_deviation;      //3-sigma rule
        upper_bound = middle + confidence_interval*std_deviation;

        //step 4: find middle for filtered interval
        middle = 0; filtered_size = 0;
        for (auto x:*input)
        {
            if (x>=lower_bound && x<=upper_bound)
            {
                middle+=x;
                filtered_size++;                          //count ammount of elements in condfidence interval
            }
        }
        middle/=filtered_size;

        //step 5: find dispersion for filtered interval
        dispersion = 0;
        for (auto x:*input)
        {
            if (x>=lower_bound && x<=upper_bound)
                dispersion += (x-middle)*(x-middle);
        }
        dispersion /= filtered_size-1;

        //step 6: find std_deviation for filtered interval
        std_deviation = sqrt(dispersion);
        clock_t fin = clock();       //del!
        qDebug() << "seconds used: " << ((double)(fin-start)) / CLOCKS_PER_SEC; //del!
    }
    emit Statistic::finished();
    return 0;
}
void Statistic::histogram_add(qint32 x)
{
    temp = static_cast<int>(floor((x-min)/bin_width))+1;    // find num of bin for element
    if (temp > num_of_intervals)
        temp=num_of_intervals;
    ++histogram->operator[](temp);

    //for some reason in qtCreator next line don't work
    //++histogram[temp]
    //works in VS
}
float Statistic::getMiddle() {return middle;}
float Statistic::getStdDeviation() {return std_deviation;}
float Statistic::getBinWidth() {return bin_width;}
float Statistic::getMax(){return max;}
float Statistic::getMin(){return min;}
