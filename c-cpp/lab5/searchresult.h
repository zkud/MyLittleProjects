#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QDialog>

namespace Ui {
class SearchResult;
}

class SearchResult : public QDialog
{
    Q_OBJECT

public:
    explicit SearchResult(QString result_string = "<empty>", QWidget *parent = 0);
    ~SearchResult();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SearchResult *ui;
};

#endif // SEARCHRESULT_H
