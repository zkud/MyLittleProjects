#include "errordialog.h"
#include "ui_errordialog.h"
#include <QLabel>

class Qlabel;

ErrorDialog::ErrorDialog(QString error_text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
    this->ui->label_4->setText(error_text);
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::changeEvent(QEvent *e)
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
