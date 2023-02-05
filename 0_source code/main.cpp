#include "pagecontroller.h"
#include <QFile>
#include <QApplication>

QString readTextFile(QString path){
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        return in.readAll();
    }
    return "";
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString css = readTextFile(":/style/Style/style.css");
    if(css.length() > 0){
        a.setStyleSheet(css);
    }
    pageController w;
    w.show();
    return a.exec();
}
