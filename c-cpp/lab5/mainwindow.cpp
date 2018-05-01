#include "mainwindow.h"
#include <QTextEdit>
#include "ui_mainwindow.h"
#include "mainhandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->textEdit->setReadOnly(true);
}

QTextEdit* MainWindow:: getMainText() {
    return this->ui->textEdit;
}

bool MainWindow:: linkWithMainHandler(MainHandler *mainHandler) {
    // try to link all buttons
    try {
        QObject:: connect(this->ui->refresh_button, SIGNAL(clicked(bool)), mainHandler,
                          SLOT(refresh_clicked()));
        QObject:: connect(this->ui->add_button, SIGNAL(clicked(bool)), mainHandler,
                          SLOT(add_clicked()));
        QObject:: connect(this->ui->delete_button, SIGNAL(clicked()), mainHandler,
                          SLOT(delete_clicked()));
        QObject:: connect(this->ui->search_button, SIGNAL(clicked()), mainHandler,
                          SLOT(search_clicked()));
        QObject:: connect(this->ui->save_button, SIGNAL(clicked()), mainHandler,
                          SLOT(save_clicked()));
        QObject:: connect(this->ui->read_button, SIGNAL(clicked(bool)), mainHandler,
                          SLOT(read_clicked()));

    }
    // if something happens, return false
    catch(...) {
        return false;
    }

    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
