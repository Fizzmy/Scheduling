#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void userLogin();
signals:
    void showMainPage();
private:
    Ui::login *ui;
};

#endif // LOGIN_H
