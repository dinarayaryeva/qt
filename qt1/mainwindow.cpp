#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QUdpSocket>
#include <QDebug>
#include <QString>
#include <QMessageBox>

#define LIST_SIZE 1000
#define RANGE 10000

void MainWindow::generateData(){

    for (int i=0; i<LIST_SIZE; i++){
        int k = rand()%(2*RANGE)-RANGE;
        listWidget->addItem(QString::number(k));
    }
    listWidget->show();
}

void MainWindow::sendData(){

    emit sendStarted();

    qDebug()<<"Forwarding started";

    QUdpSocket sk;
    QByteArray d;
    QDataStream stream(&d, QIODevice::ReadWrite);

    stream << listWidget->count();

    for(int i = 0; i < listWidget->count(); ++i)
    {
        QListWidgetItem* item = listWidget->item(i);
        stream << *item;
    }

    sk.writeDatagram(d, QHostAddress::Any, 65222);

    qDebug()<<"Forwarding ended";

    emit sendFinished();
}

void MainWindow::resume(){

    connect(generate, &QPushButton::clicked, this, &MainWindow::generateData);
    connect(send, &QPushButton::clicked, this, &MainWindow::sendData);

    connect(fileSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(fileLoad, &QAction::triggered, this, &MainWindow::getFile);
    qDebug()<<"Interaction is resumed";

}

void MainWindow::pause(){

    disconnect(generate, &QPushButton::clicked, this, &MainWindow::generateData);
    disconnect(send, &QPushButton::clicked, this, &MainWindow::sendData);

    disconnect(fileSave, &QAction::triggered, this, &MainWindow::saveFile);
    disconnect(fileLoad, &QAction::triggered, this, &MainWindow::getFile);
    qDebug()<<"Interaction is paused";

}

void MainWindow::saveFile(){

    QString str = QFileDialog::getSaveFileName(0, "Set a new file name or override an existing one");

    QFile file( str );
    QDataStream stream( &file );

    if (!file.open( QIODevice::WriteOnly)) {

        qDebug()<<"Cannot open the file "<< str;
        QMessageBox::warning(this, "Error","Cannot open file for writing");
        return;

    }

    stream << listWidget->count();
    for(int i = 0; i < listWidget->count(); ++i){
        QListWidgetItem* item = listWidget->item(i);
        stream << *item;
    }
        file.close();

    qDebug()<<"Data is saved in file "<< str;

}

void MainWindow::getFile(){

    QString str = QFileDialog::getOpenFileName(0, "Choose the file");

    QFile file( str );
    QDataStream stream( &file );

    if (!file.open( QIODevice::ReadOnly )) {

        qDebug()<<"Cannot open the file "<< str;
        QMessageBox::warning(this, "Error","Cannot open the file!");
        return;

    }

    int size;
    stream >> size;
    listWidget->clear();

    for(int i = 0; i < size; ++i)
    {
        QListWidgetItem item;
        stream >> item;
        listWidget->addItem(item.text());
    }

    listWidget->update();

    file.close();
    qDebug()<<"Data is captured  from file "<< str;

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , widget(new QWidget())
    , ui(new Ui::MainWindow)
{

    srand(time(0));

    ui->setupUi(this);

    generate = new QPushButton("Generate", this);
    send = new QPushButton("Send", this);
    listWidget = new QListWidget(this);

    generate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    send->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    listWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    listWidget->show();

    fileSave = new QAction("&Save into a file", this);
    fileLoad = new QAction("&Load from a file", this);

    QMenu *file;
    file = menuBar()->addMenu("&File");
    file->addAction(fileSave);
    file->addAction(fileLoad);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(3);
    vbox->addStretch(1);
    vbox->addWidget(generate);
    vbox->addWidget(send);
    vbox->addStretch(1);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(listWidget);
    hbox->addSpacing(15);
    hbox->addLayout(vbox);

    this->resume();
    connect(this, &MainWindow::sendStarted, this, &MainWindow::pause);
    connect(this, &MainWindow::sendFinished, this, &MainWindow::resume);

    widget->setLayout(hbox);
    setCentralWidget(widget);

}

MainWindow::~MainWindow() = default;

