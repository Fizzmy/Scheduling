#include "login.h"
#include "ui_login.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlTableModel>
#include <QSqlRecord>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");
    ui->password->setEchoMode(QLineEdit::Password);
    connect(ui->loginButton,SIGNAL(clicked()),this,SLOT(userLogin()));
}

login::~login()
{
    delete ui;
}

void login::userLogin()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1"); //数据库服务器IP
    db.setUserName(ui->userName->text()); //数据库用户名
    db.setPassword(ui->password->text()); //数据库密码
    db.setDatabaseName(ui->databaseName->text());
    if( !db.open() ){ //数据库打开失败
       QMessageBox::warning(this,"错误",db.lastError().text());
       return;
    }
    else
    {
        QSqlQuery query;
        query.exec("create table timedata(id tinytext,name tinytext,周一1 tinytext,周一2 tinytext,周一3 tinytext,周一4 tinytext,周二1 tinytext,周二2 tinytext,周二3 tinytext,周二4 tinytext,周三1 tinytext,周三2 tinytext,周三3 tinytext,周三4 tinytext,周四1 tinytext,周四2 tinytext,周四3 tinytext,周四4 tinytext,周五1 tinytext,周五2 tinytext,周五3 tinytext,周五4 tinytext,周六1 tinytext,周六2 tinytext,周六3 tinytext,周六4 tinytext,周日1 tinytext,周日2 tinytext,周日3 tinytext,周日4 tinytext,tag int)");
        this->hide();
        emit showMainPage();
    }
}

