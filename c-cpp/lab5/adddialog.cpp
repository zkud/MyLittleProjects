#include "adddialog.h"
#include "ui_adddialog.h"
#include "queue.h"
#include <QObject>
#include <QDateTime>
#include <QLineEdit>

AddDialog::AddDialog(Data* data_ptr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    this->_data_ptr = data_ptr;
}


// override accept slot
void AddDialog:: accept() {
    // variables:
    QLineEdit* number = this->ui->numberLine;
    QLineEdit* aim = this->ui->aimLine;
    QLineEdit* cost = this->ui->costLine;
    QDateTimeEdit* date_time = this->ui->dateTimeEdit;

    // check if all LineEdits are modified and check size of aim line
    if((number->isModified() && aim->isModified()
       && cost->isModified()) == false
       || aim->text().size() >= MAX_SIZE)
        return;

    // set data struct by input (from dialog form)
    this->_data_ptr->number = atoi(number->text().toStdString().c_str());
    this->_data_ptr->cost = atof(cost->text().toStdString().c_str());
    this->_data_ptr->date.day = date_time->date().day();
    this->_data_ptr->date.month = date_time->date().month();
    this->_data_ptr->date.year = date_time->date().year();
    this->_data_ptr->time.hour = date_time->time().hour();
    this->_data_ptr->time.min = date_time->time().minute();
    this->_data_ptr->time.sec = date_time->time().second();
    strncpy(this->_data_ptr->aim, aim->text().toStdString().c_str(), MAX_SIZE);

    this->done(QDialog::Accepted); // exit, exec returns true
}

// override reject slot
void AddDialog:: reject() {
    this->done(QDialog::Rejected); // exit, exec returns false
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::changeEvent(QEvent *e)
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
