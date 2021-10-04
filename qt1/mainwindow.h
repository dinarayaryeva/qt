#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow: public QMainWindow
{
    Q_OBJECT

    QListWidget *listWidget;
    QPushButton *generate;
    QPushButton *send;
    QWidget *widget;
    QAction *fileSave;
    QAction *fileLoad;
    Ui::MainWindow *ui;

signals:
    void sendFinished();
    void sendStarted();

public:
     MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void generateData();
    void sendData();
    void saveFile();
    void getFile();

private slots:
    void resume();
    void pause();

};
#endif // MAINWINDOW_H
