#include "mainhandler.h"
#include "mainwindow.h"
#include "adddialog.h"
#include <QFileDialog>
#include "deletedialog.h"
#include "errordialog.h"
#include "searchdialog.h"
#include "searchresult.h"
#include "queue.h"
#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include <QLayout>
#include <QTextEdit>
#include "mainwindow.h"

MainHandler::MainHandler(MainWindow* main_window_pointer, QObject *parent) : QObject(parent)
{
    _main_window_pointer = main_window_pointer;
    QueueInit(_main_queue);
    _main_textedit_pointer = _main_window_pointer->getMainText();
}

MainHandler:: ~MainHandler() {
    ClearAll(&this->_main_queue);
}

// prints queue from head to tail
// args: descriptor of queue and pointer on function, wich shows single data
void MainHandler::_ShowQueue(QueueDescriptor descriptor) {
    // variables:
    Flight* temporary = descriptor.head;

    // print tables head:
    this->_main_textedit_pointer->insertPlainText("Number Aim Date Time Cost \n");

    // print queue from head to tail
    while (temporary) {
        // print one element
        this->_ShowInfo(&temporary->data);

        // go to the next element
        temporary = temporary->next;
    }
}

void MainHandler:: _ShowInfo(Data* data_pointer) {
    // variables:
    QString output_string;

    // set output string
    output_string = QString::fromStdString(std::to_string(data_pointer->number) + ' '
                    + data_pointer->aim + ' '
                    + std::to_string(data_pointer->date.day) + '.'
                    + std::to_string(data_pointer->date.month) + '.'
                    + std::to_string(data_pointer->date.year) + ' '
                    + std::to_string(data_pointer->time.hour) + ':'
                    + std::to_string(data_pointer->time.min) + ':'
                    + std::to_string(data_pointer->time.sec) + ' '
                    + std::to_string(data_pointer->cost) + '\n');

    // insert output string
    this->_main_textedit_pointer->insertPlainText(output_string);
}


void MainHandler:: refresh_clicked() {
    // clear textedit of main window
    this->_main_textedit_pointer->setText("");

    // print main queue in this textedit
    _ShowQueue(this->_main_queue);
}

void MainHandler:: add_clicked(){
    // variables:
    Data input_data = {};

    // set input data, if dialog is rejected, do nothing
    AddDialog dialog(&input_data);
    if (dialog.exec() == QDialog::Rejected)
        return;

    // try to add node with input data
    if(AddItem(this->_main_queue, &input_data) == false) {
        ErrorDialog error("Error in queue adding");
        error.exec();
    }

    // refresh main windows textedit
    this->refresh_clicked();
}

void MainHandler:: delete_clicked(){
    // variables:
    int node_number;

    // try to set node; if user rejected deletion, just exit
    DeleteDialog dialog(&node_number);
    if(dialog.exec() == QDialog::Rejected)
        return;

    // try to delete selected node
    if(DeleteItem(this->_main_queue, node_number) == false) {
        ErrorDialog error("Error in queue deleting");
        error.exec();
    }

    // refresh main windows textedit
    this->refresh_clicked();
}

QString MainHandler:: _toResultString(Flight* flightPointer) {
    // set and return result string
    QString result_string = QString::fromStdString(std::to_string(flightPointer->data.number) + ' '
            + flightPointer->data.aim + ' '
            + std::to_string(flightPointer->data.date.day) + '.'
            + std::to_string(flightPointer->data.date.month) + '.'
            + std::to_string(flightPointer->data.date.year) + ' '
            + std::to_string(flightPointer->data.time.hour) + ':'
            + std::to_string(flightPointer->data.time.min) + ':'
            + std::to_string(flightPointer->data.time.sec) + ' '
            + std::to_string(flightPointer->data.cost) + '\n');

    return result_string;
}

void MainHandler:: search_clicked(){
    // variables:
    Query query;
    Flight* result_node_pointer;

    // try to set query for searching; if user rejected searching, just exit
    SearchDialog dialog(&query);
    if(dialog.exec() == QDialog::Rejected)
        return;

    // find node by parameters
    result_node_pointer = FindItem(this->_main_queue, query);

    // show result
    // if node exists
    if(result_node_pointer != nullptr) {
        // set result string and show it
        SearchResult result(this->_toResultString(result_node_pointer));
        result.exec();
    }
    // if node doesn't exist, output it
    else {
        SearchResult result("No such nodes :(");
        result.exec();
    }
}

void MainHandler:: save_clicked(){
    // get name of file
    QFileDialog dialog;
    QString fileName = dialog.getSaveFileName(nullptr, tr("save file"), "",
                                              tr("Any file (*.*)"));

    // try to save queue in file
    if(saveInfoToFile(this->_main_queue, fileName.toStdString().c_str()) != true) {
        ErrorDialog error("Error in queue saving");
        error.exec();
    }

    // refresh main windows textedit
    this->refresh_clicked();
}

void MainHandler:: read_clicked(){
    // get name of file
    QFileDialog dialog;
    QString fileName = dialog.getOpenFileName(nullptr, tr("open file"), "",
                                              tr("Any file (*.*)"));

    // try to read queue from file
    if(ReadFromFile(this->_main_queue, fileName.toStdString().c_str()) != true) {
        ErrorDialog error("Error in queue reading");
        error.exec();
    }

    // refresh main windows textedit
    this->refresh_clicked();
}
