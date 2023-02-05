#ifndef PAGEMECHANISM_H
#define PAGEMECHANISM_H
#include <QWidget>


class PageMechanism: public QWidget
{
    Q_OBJECT
public:
    PageMechanism(QWidget *parent = nullptr);
signals:
    void signal_changeToPageStart(); //測試中發現qt自動產生程式碼中有this物件引數所以無法使用static
    void signal_changeToPageChar();
    void signal_changeToPageGame();
    void signal_exitGame();
};

#endif // PAGEMECHANISM_H
