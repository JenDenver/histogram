#include <QCoreApplication>
#include <QFile>
#include "statistic.h"
#include <iomanip>
#include <QTime>
#include <QDebug>

void fileRead(QString path,QList<qint32> *data)//test function, not optimized
{
    assert(data!=nullptr);
    QFile file(path);
    assert(file.open(QIODevice::ReadOnly));
    QTextStream stream(&file);
    while(!file.atEnd())
    {
            int buf;
            QStringList list = stream.readAll().split("\n");
            QListIterator<QString> li(list);
            while(li.hasNext())
            {
                buf=li.next().toInt();
                data->append(buf);
            }
    }
    file.close();
}
int main(int argc, char *argv[])                //test
{
    QCoreApplication a(argc, argv);
    QString p1 = QCoreApplication::applicationDirPath() + "/input.txt";     //for small tests use input.txt
    //QString p2 = QCoreApplication::applicationDirPath() + "/output.txt";
    QList<qint32> data;
    fileRead(p1,&data);
    int num_of_bins{10};
    int num_of_sigmas{1};
    Statistic *stat = new Statistic(num_of_sigmas,num_of_bins, &data);
    stat->make_histogram();
    qDebug() << "average: " << stat->average();
    qDebug() << "standard deviation: " << stat->std_deviation();
    for (int i=1;i<=num_of_bins;i++)
    {
        qDebug()<< i << ": " << stat->histogram->at(i) << "\n";
    }
    stat->filtered_histogram();
    qDebug() << "filtered average: " << stat->filteredAverage();
    qDebug() << "filtered std deviation: " << stat->filtered_std_deviation();
    delete stat;
    return a.exec();
}

