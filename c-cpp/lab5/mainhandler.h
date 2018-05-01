#ifndef MAINHANDLER_H
#define MAINHANDLER_H

#include <QObject>
#include <QTextEdit>
#include <QPushButton>
#include "mainwindow.h"
#include "queue.h"

class MainWindow;
class QString;

// class which implements all prog logic
class MainHandler : public QObject
{
    Q_OBJECT

private:
    // needed pointers
    MainWindow* _main_window_pointer;
    QueueDescriptor _main_queue;
    QTextEdit* _main_textedit_pointer;


    // prints queue from head to tail
    // args: descriptor of queue and pointer on function, wich shows single data
    void _ShowQueue(QueueDescriptor descriptor);
    // queue output function:
    void _ShowInfo(Data* data_pointer);
    // convert flights data to string
    QString _toResultString(Flight* flightPointer);

public:
    MainHandler(MainWindow* main_window_pointer, QObject *parent = nullptr);
    ~MainHandler();

signals:

public slots:
    // if *name of button of main window* clicked do:
    void refresh_clicked();
    void add_clicked();
    void delete_clicked();
    void search_clicked();
    void save_clicked();
    void read_clicked();
};

#endif // MAINHANDLER_H
