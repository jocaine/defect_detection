#include"DataCollector.h"
#include"ResultCollector.h"
#include<QString>
#include<QDebug>
#include <QApplication>
#include"mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataCollector dc;
    dc.show();
    DataHandler handler;
    handler.start();
    ResultCollector rc(&handler);
    MainWindow window(&rc);
    QObject::connect(&dc,&DataCollector::MatPackage,&handler,&DataHandler::slInput);
    QObject::connect(&dc,&DataCollector::sgStartReading,&window,&MainWindow::showout);


    //window.show();
    return a.exec();
}
