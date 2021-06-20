#include "widget.h"
#include "ui_widget.h"
#include "detail.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <iostream>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("排班系统");
    this->setStyleSheet("border-image: url(./new/C:/Users/倦倚/Pictures/picture.png);");
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(navigateToDetail()));
    connect(ui->generate,SIGNAL(clicked()),this,SLOT(navigateToGenerate()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::navigateToDetail()
{

    this->hide();
    emit showDetailPage();
}
void Widget::showMainPage()
{
    this->show();
}
void Widget::navigateToGenerate()
{
    this->hide();
    emit showGeneratePage();
}


