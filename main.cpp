#include "widget.h"
#include <detail.h>
#include <add.h>
#include <generate.h>
#include <login.h>
#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlTableModel>
#include <QSqlRecord>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    add addPage;
    detail detailPage;
    generate generatePage;
    login loginPage;
    loginPage.show();
    QObject::connect(&loginPage,SIGNAL(showMainPage()),&w,SLOT(showMainPage()));
    QObject::connect(&w, SIGNAL(showDetailPage()),
                         &detailPage,SLOT(showDetailPage()));
    QObject::connect(&w,SIGNAL(showGeneratePage()),&generatePage,SLOT(showGeneratePage()));
    QObject::connect(&generatePage,SIGNAL(showMainPage()),&w,SLOT(showMainPage()));
    QObject::connect(&detailPage,SIGNAL(showMainPage()),&w,SLOT(showMainPage()));
    QObject::connect(&detailPage,SIGNAL(showAddPage(int)),&addPage,SLOT(showAddPage(int)));
    QObject::connect(&addPage,SIGNAL(showDetailPage()),&detailPage,SLOT(showDetailPage()));

    QObject::connect(&addPage,SIGNAL(sendData(QStringList)),&detailPage,SLOT(getData(QStringList)));
    return a.exec();
}
