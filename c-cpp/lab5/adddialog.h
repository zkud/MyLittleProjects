#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include "queue.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(Data* data_ptr, QWidget *parent = 0);
    ~AddDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AddDialog *ui;
    Data* _data_ptr;

public slots:
    // there are two buttons in dialog's forms
    // slots handle when these buttons clicked
    void accept();
    void reject();
};

#endif // ADDDIALOG_H
