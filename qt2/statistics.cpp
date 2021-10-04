#include "statistics.h"
#include "ui_statistics.h"

#include<QVBoxLayout>
#include<QHBoxLayout>

Statistics::Statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);

    QLabel*_count = new QLabel("Quantity: ");
    QLabel*_average = new QLabel("Average: ");
    QLabel*_variance = new QLabel("Variance: ");
    QLabel*_max = new QLabel("Maximum: ");
    QLabel*_min = new QLabel("Minimum: ");

    count = new QLabel("0");
    average = new QLabel("0");
    variance = new QLabel("0");
    max = new QLabel("0");
    min = new QLabel("0");

    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(2);

    grid->addWidget(_count, 0, 0);
    grid->addWidget(count, 0, 1);
    grid->addWidget(_average, 1, 0);
    grid->addWidget(average, 1, 1);
    grid->addWidget(_variance, 2, 0);
    grid->addWidget(variance, 2, 1);
    grid->addWidget(_max, 3, 0);
    grid->addWidget(max, 3, 1);
    grid->addWidget(_min, 4, 0);
    grid->addWidget(min, 4, 1);

    setLayout(grid);

}

void Statistics::updateCount(int i){

    count->setText(QString::number(i));
    count->update();

}

void Statistics::updateAverage(double d){

    average->setText(QString::number(d,'.', 3));
    average->update();

}
void Statistics::updateVariance(double d){

    variance->setText(QString::number(d,'.', 3));
    variance->update();

}
void Statistics::updateMin(int i){

    min->setText(QString::number(i));
    min->update();

}
void Statistics::updateMax(int i){

    max->setText(QString::number(i));
    max->update();

}

Statistics::~Statistics() = default;
