#include"DataCollector.h"
#include"ResultCollector.h"
#include <QApplication>
#include"mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataCollector dc(FS);
    DataHandler hanler;
    dc.get_handler(&hanler);
	dc.start();
    ResultCollector rc(&hanler);
    MainWindow window(&rc);
    window.show();
    return a.exec();
}
