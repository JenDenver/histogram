#include "statistic.h"


Statistic::Statistic(qint32 conf, qint32 num, QList<qint32> *data):
    confidence_interval(conf), num_of_intervals(num), input(data)
{
    assert(input!=nullptr && !(input->isEmpty()) && "empty input list");
    histogram = new std::map<qint32,qint32>();
}
Statistic::~Statistic()
{
    delete histogram;
}

//public methods
float Statistic::average()
{
    middle=0;
    for (auto x:*input)
    {
        middle+=x;
    }
    middle /= input->size();
    return middle;
}
float Statistic::filteredAverage()
{
    average();
    dispersion();
    stdDev = sqrt(_dispersion);
    findConfInterval();
    middle = 0; filtered_size = 0;
    for (auto x:*input)
    {
        if (x>=lower_bound && x<=upper_bound)
        {
            middle+=x;
            ++filtered_size;
        }
    }
    assert(filtered_size>0 && "nothing in confidence interval");
    middle/=filtered_size;
    return middle;
}
float Statistic::std_deviation()
{
    average();
    dispersion();
    stdDev = sqrt(_dispersion);
    return stdDev;
}
float Statistic::filtered_std_deviation()
{
    filteredAverage();
    f_dispersion();
    stdDev = sqrt(_dispersion);
    return stdDev;
}
void  Statistic::make_histogram()
{
    emit Statistic::started();
    minimax();
    assert(num_of_intervals>1 && "num of intervals must be >1");
    if (!histogram->empty())
        histogram->clear();
    for (int i=1;i<=num_of_intervals;i++)          //prepare map
        histogram->operator[](i)=0;
    bin_width = (max-min)/num_of_intervals;
    assert(bin_width>0 && "bin width <=0");
    for (auto x:*input)
    {
        histogram_add(x);
    }
    emit Statistic::finished();
}
void  Statistic::filtered_histogram()
{
    emit Statistic::started();
    assert(num_of_intervals>0 && "num of intervals must be >0");
    average();
    dispersion();
    stdDev = sqrt(_dispersion);
    findConfInterval();                            //find bounds
    if (!histogram->empty())
        histogram->clear();
    for (int i=1;i<=num_of_intervals;i++)          //prepare map
        histogram->operator[](i)=0;
    bin_width = (upper_bound-lower_bound)/num_of_intervals;
    assert(bin_width>0 && "bin width <=0");
    for (auto x:*input)
    {
        if (x>=lower_bound && x<=upper_bound)      //add to histogram values in confident interval
            f_histogram_add(x);
    }
    emit Statistic::finished();
}
float Statistic::getBinWidth()
{
    return bin_width;
}

//private methods
void Statistic::minimax()
{
    min = input->first();
    max = input->first();
    for (auto x:*input)
    {
        if (x<min) min = x;
        if (x>max) max = x;
    }
}
void Statistic::findConfInterval()
{
    assert(confidence_interval>0 && confidence_interval<4 && "conf. interval must be [1..3]");
    lower_bound = middle - confidence_interval*stdDev;      //3-sigma rule
    upper_bound = middle + confidence_interval*stdDev;
}
void Statistic::histogram_add(qint32 x)
{
    temp = static_cast<int>(floor((x-min)/bin_width))+1;    // find num of bin for element
    if (temp > num_of_intervals)
        temp=num_of_intervals;                              //fix for last number
    ++histogram->operator[](temp);

    //for some reason in qtCreator next line don't work
    //++histogram[temp]
    //works in VS
}
void Statistic::f_histogram_add(qint32 x)                   //version for filtered histogram
{
    temp = static_cast<int>(floor((x-lower_bound)/bin_width))+1;    // find num of bin for element
    if (temp > num_of_intervals)
        temp=num_of_intervals;                              //fix for last number
    ++histogram->operator[](temp);

    //for some reason in qtCreator next line don't work
    //++histogram[temp]
    //works in VS
}
void Statistic::dispersion()
{
    _dispersion=0;
    for (auto x:*input)
    {
        _dispersion += (x-middle)*(x-middle);
    }
    assert(input->size()>1 && "input size <=1");
    _dispersion /= input->size()-1;
}
void Statistic::f_dispersion()
{
    _dispersion = 0;
    for (auto x:*input)
    {
        if (x>=lower_bound && x<=upper_bound)
            _dispersion += (x-middle)*(x-middle);
    }
    assert(filtered_size>0 && "nothing in confidence interval");
    _dispersion /= filtered_size-1;
}
void Statistic::f_hist()                                    //fast version for filtered histogram
{
    assert(num_of_intervals>0 && "num of intervals must be >0");
    if (!histogram->empty())
        histogram->clear();
    for (int i=1;i<=num_of_intervals;i++)          //prepare map
        histogram->operator[](i)=0;
    bin_width = (upper_bound-lower_bound)/num_of_intervals;
    assert(bin_width>0 && "bin width <=0");
    for (auto x:*input)
    {
        if (x>=lower_bound && x<=upper_bound)      //add to histogram values in confident interval
            f_histogram_add(x);
    }
}








