#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "mainhandler.h"

namespace Ui {
class MainWindow;
}

class MainHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    // try link buttons from main window with theirs handler
    // if something bad happens method will return false
    bool linkWithMainHandler(MainHandler *mainHandler);
    QTextEdit* getMainText();
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
