#include <QCoreApplication>
#include <QFile>
#include "statistic.h"
#include <iomanip>
#include <QTime>

void HistToFile(QString path, Statistic *s) //test function, only for small inputs
{
    QFile file(path);
    assert(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&file);
    float low{s->getMin()}, up{low+s->getBinWidth()};
    for(auto iter = s->histogram->begin(); iter != s->histogram->end(); ++iter)
    {
        out.setFieldWidth(6);
        out << QString::number(low, 'f', 2) << "--" << QString::number(up, 'f', 2);
        out.setFieldWidth(0);
        out << ": " << QString(iter->second, QChar('#')) << "\n";
        low = up;
        up+=s->getBinWidth();
    }
    file.close();
}
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
    QString p1 = QCoreApplication::applicationDirPath() + "/pings.txt";
    //QString p2 = QCoreApplication::applicationDirPath() + "/output.txt";
    QList<qint32> data;
    fileRead(p1,&data);
    int num_of_bins{10};
    int num_of_sigmas{1};
    Statistic *stat = new Statistic(num_of_sigmas,num_of_bins, &data);
    qDebug() << "avg";
    for (int i=0;i<50;i++)
        stat->average();
    /*for (int i=1;i<=num_of_bins;i++)
    {
        qDebug()<< i << ": " << stat->histogram->at(i) << "\n";
    }
    HistToFile(p2, stat);*/

    delete stat;
    return a.exec();
}

