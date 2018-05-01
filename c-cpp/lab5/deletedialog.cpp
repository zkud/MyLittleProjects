#include "deletedialog.h"
#include "ui_deletedialog.h"

DeleteDialog::DeleteDialog(int* number_pointer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteDialog)
{
    ui->setupUi(this);
    this->_number_pointer = number_pointer;
}

DeleteDialog::~DeleteDialog()
{
    delete ui;
}

// override default accept slot
void DeleteDialog:: accept() {
    // set number and exit
    *(this->_number_pointer) = this->ui->choice->value();
    this->done(QDialog::Accepted);
}

void DeleteDialog:: reject() {
    // exit
    this->done(QDialog::Rejected);
}

void DeleteDialog::changeEvent(QEvent *e)
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
