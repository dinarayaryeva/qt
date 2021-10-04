#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class Statistics;
}

class Statistics : public QWidget
{
    Q_OBJECT

    QLabel* count;
    QLabel* average;
    QLabel* variance;
    QLabel* max;
    QLabel* min;
    Ui::Statistics *ui;

public:
    void updateCount(int i);
    void updateAverage(double d);
    void updateVariance(double d);
    void updateMin(int i);
    void updateMax(int i);

    explicit Statistics(QWidget *parent = 0);
    ~Statistics();

private:
};


#endif // STATISTICS_H
