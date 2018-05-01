#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include "queue.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(Query *query_ptr, QWidget *parent = 0);
    ~SearchDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SearchDialog *ui;
    Query* _query_pointer;

    // method sets query first parameter
    // returns true if all is ok and false if is not
    bool _setFirstParameter();

public slots:
    // override default accept/reject slots
    void accept();
    void reject();
};

#endif // SEARCHDIALOG_H
