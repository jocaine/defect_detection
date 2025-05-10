#include"DataCollector.h"
#include"ResultCollector.h"
#include <QApplication>
#include"mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DataCollector dc(FS);
    DataHandler handler;
    QObject::connect(&dc,&DataCollector::MatPackage,&handler,&DataHandler::slInput);
    handler.start();
    dc.start();
    ResultCollector rc(&handler);
    MainWindow window(&rc);

    window.show();
    return a.exec();
}
