#include <QCoreApplication>
#include <QFile>
#include "statistic.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file("E:\\Repo\\Statistic\\input.txt");
    QList<qint32> data;

    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        while(!file.atEnd())
        {
            int buf;
            QStringList list = stream.readAll().split("\n");
            QListIterator<QString> li(list);
            while(li.hasNext())
            {
                buf=li.next().toInt();
                data.append(buf);
            }
        }
    }
    Statistic *stat = new Statistic(&data,1,6);
    qDebug() << stat->getMiddle() << "\n" << stat->getStdDeviation() << "\n";
    for (int i=1;i<=6;i++)
    {
        qDebug()<< i << ": " << stat->histogram->at(i) << "\n";
    }
    delete stat;
    return a.exec();
}
