#include "searchdialog.h"
#include "ui_searchdialog.h"
#include "queue.h"
#include "errordialog.h"
#include "stdio.h"

SearchDialog::SearchDialog(Query* query_ptr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    this->_query_pointer = query_ptr;
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

// method sets query first parameter
// returns true if all is ok and false if is not
bool SearchDialog:: _setFirstParameter() {
    // set query's first parameter
    switch(this->_query_pointer->types[0]) {
    // number
    case 0:
        this->_query_pointer->data.number = atoi(this->ui->lineEdit1->text().toStdString().c_str());
        break;

    // aim
    case 1:
        strncpy(this->_query_pointer->data.aim,
                this->ui->lineEdit1->text().toStdString().c_str(), MAX_SIZE);
        break;

    // time
    case 2:
        this->_query_pointer->data.time.hour = this->ui->dateTimeEdit1->time().hour();
        this->_query_pointer->data.time.min = this->ui->dateTimeEdit1->time().minute();
        this->_query_pointer->data.time.sec = this->ui->dateTimeEdit1->time().second();
        break;

    // date
    case 3:
        this->_query_pointer->data.date.day = this->ui->dateTimeEdit1->date().day();
        this->_query_pointer->data.date.month = this->ui->dateTimeEdit1->date().month();
        this->_query_pointer->data.date.year = this->ui->dateTimeEdit1->date().year();
        break;

    // cost
    case 4:
        this->_query_pointer->data.cost = atof(this->ui->lineEdit1->text().toStdString().c_str());
        break;

    // catch type errors
    default:
        ErrorDialog typeError("Wrong type in search dialog");
        typeError.exec();
        return false;
    }

    return true;
}

// override default accept slot
void SearchDialog:: accept() {
    // set query
    // set types in query
    this->_query_pointer->types[0] = this->ui->comboBox1->currentIndex();
    this->_query_pointer->types[1] = this->ui->comboBox2->currentIndex();

    // set the first parameter value
    if(this->_setFirstParameter() == false)
        return;

    // exit
    this->done(QDialog::Accepted);
}

// override default reject slot
void SearchDialog:: reject() {
    this->done(QDialog::Rejected);
}

void SearchDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
