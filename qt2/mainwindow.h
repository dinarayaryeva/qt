#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QUdpSocket>

#include "statistics.h"

#define LIST_SIZE 1000
#define RANGE 10000

using namespace QtCharts;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QChartView *chartView;
    QUdpSocket* sk;
    QWidget *widget;
    QPushButton* stats;
    QPushButton* clear;
    Statistics* statistics;
    Ui::MainWindow *ui;

    QList<int> *list;
    int chartData[20];

    double sum = 0;
    int count = 0;
    int max = -RANGE;
    int min = RANGE;

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void readData();

private:

    void _clear();
};

#endif // MAINWINDOW_H
