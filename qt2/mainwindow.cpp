#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>

#include<QVBoxLayout>
#include<QHBoxLayout>
#include <QApplication>

#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      chartView(new QChartView()),
      widget(new QWidget()),
     ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    list = new QList<int>();

    chartView->show();

    for (int i=0; i<20;i++){
       chartData[i] = 0;
    }

    statistics = new Statistics();
    statistics->setWindowTitle("Statistics");

    stats = new QPushButton("Statistics", this);
    clear = new QPushButton("Clear", this);

    stats->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    clear->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(3);
    vbox->addStretch(1);
    vbox->addWidget(stats);
    vbox->addWidget(clear);
    vbox->addStretch(1);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(chartView);
    hbox->addSpacing(15);
    hbox->addLayout(vbox);

    connect(stats, &QPushButton::clicked, statistics, &QWidget::show);
    connect(clear, &QPushButton::clicked, this, &MainWindow::_clear);

    widget->setLayout(hbox);
    setCentralWidget(widget);

    sk = new QUdpSocket();
    sk->bind(QHostAddress::Any,65222);

    connect(sk,&QUdpSocket::readyRead,this, &MainWindow::readData);

}

void MainWindow::readData(){

    QByteArray buffer;
    buffer.resize(static_cast<int>(sk->pendingDatagramSize()));

    QHostAddress sender = QHostAddress::Any;
    quint16 senderPort=65111;

    qDebug()<<sk->readDatagram(buffer.data(), buffer.size(),&sender, &senderPort);

    QDataStream stream( &buffer, QIODevice::ReadWrite );

    int size;
    stream >> size;

    count+=size;

    statistics->updateCount(count);

    QBarSeries *series = new QBarSeries();

    for(int i = 0; i < size; ++i){

       QListWidgetItem item;
       stream >> item;

       if (item.text().toInt()>max)
           max = item.text().toInt();

       if (item.text().toInt()<min)
            min = item.text().toInt();

       sum+=item.text().toInt();
       chartData[(item.text().toInt()+RANGE)*10/RANGE] +=1;
       list->append(item.text().toInt());
    }

    statistics->updateAverage(sum/count);
    long double errors = 0;

    foreach (auto e, *list){
        errors+= pow(e-(sum/count), 2);
    }

    statistics->updateVariance(sqrt(errors/count));

    statistics->updateMin(min);
    statistics->updateMax(max);


    QBarSet *bar_set = new QBarSet("");

    for (int i=0; i<20;i++){
       *bar_set << chartData[i];
    }

    series->append(bar_set);

    series->setVisible(true);

    QColor col = QColor(Qt::green);
    QList<QBarSet *> sets = series->barSets();
    foreach(auto b, sets){
        b->setColor(col);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);

    QStringList categories;
    for (int i=-10; i<10; i++){
        categories << QString::number(i);
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,1000);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->hide();

    chartView->setChart(chart);
    chartView->update();

}

MainWindow::~MainWindow() = default;

void MainWindow::_clear(){

    statistics->updateCount(0);
    statistics->updateAverage(0);
    statistics->updateVariance(0);
    statistics->updateMin(0);
    statistics->updateMax(0);

    sum = 0;
    count = 0;
    min = 0;
    max = 0;

    list->clear();
    delete chartView->chart();
    chartView->setChart(new QChart());
    chartView->update();
    for (int i=0; i<20;i++){
       chartData[i] = 0;
    }
}

