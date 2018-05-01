#include "searchresult.h"
#include "ui_searchresult.h"

SearchResult::SearchResult(QString result_string, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchResult)
{
    ui->setupUi(this);
    this->ui->result_label->setText(result_string);
}

SearchResult::~SearchResult()
{
    delete ui;
}

void SearchResult::changeEvent(QEvent *e)
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
