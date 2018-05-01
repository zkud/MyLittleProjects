#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteDialog;
}

class DeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteDialog(int* number_pointer, QWidget *parent = 0);
    ~DeleteDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DeleteDialog *ui;
    int * _number_pointer; // pointer on number of node which will be deleted

public slots:
    void accept(); // override default accept slot
    void reject(); // override default reject slot
};

#endif // DELETEDIALOG_H
