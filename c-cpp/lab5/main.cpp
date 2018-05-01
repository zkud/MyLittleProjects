#include "mainwindow.h"
#include "mainhandler.h"
#include "errordialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // create application
    QApplication application(argc, argv);

    // create main window and main handler
    MainWindow main_window;
    MainHandler main_handler(&main_window);

    // try to link them with each other
    if(main_window.linkWithMainHandler(&main_handler) == false) {
        ErrorDialog errorDialog("Error in main, linking has failed");
        errorDialog.exec();
        return application.exec();
    }

    main_window.show();

    return application.exec();
}
