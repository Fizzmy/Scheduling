#include "add.h"
#include "ui_add.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlTableModel>
#include <QSqlRecord>

add::add(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add)
{
    ui->setupUi(this);
    this->setWindowTitle("添加数据");
    connect(ui->back,SIGNAL(clicked()),this,SLOT(navigateToDetail()));
    connect(ui->submit,SIGNAL(clicked()),this,SLOT(submitData()));
    connect(ui->reset,SIGNAL(clicked()),this,SLOT(resetData()));
}

add::~add()
{
    delete ui;
}
void add::showAddPage(int x)
{
    //qDebug()<<x;
    tag=x;
    if (x)
    {
        this->setWindowTitle("修改数据");
        ui->label->setText("修改数据");
        QSqlQuery query;
        QString sql=QString("select * from timedata where tag=%1").arg(tag);
        query.exec(sql);
        queryModel = new QSqlQueryModel();
        queryModel->setQuery(query);
        ui->name->setText(queryModel->record(0).value(1).toString());
        ui->id->setText(queryModel->record(0).value(0).toString());
        //int m = queryModel->columnCount();
        int num=2;
        //for (int j=2;j<m-1;j++)
        //    QString nw=queryModel->record(0).value(j).toString();
        //int tag=sqlRecord.value(column).toInt();
        for (int i=0;i<7;i++)
            for (int j=0;j<4;j++)
            {
                QString nw=queryModel->record(0).value(num).toString();
                if (nw!="0")ui->tableWidget->setItem(j,i,new QTableWidgetItem(nw));
                num++;
            }
    }
    else
    {
        this->setWindowTitle("添加数据");
        ui->label->setText("添加数据");
    }
    this->show();
}
void add::navigateToDetail()
{
    this->resetData();
    this->hide();
    emit showDetailPage();
}
void add::submitData()
{
    QStringList data;
    QString a;
    data<<ui->id->text();
    data<<ui->name->text();
    for (int i=0;i<7;i++)
        for (int j=0;j<4;j++)
        {
            if (ui->tableWidget->item(j,i)!=NULL&&ui->tableWidget->item(j,i)->text()!="")
                data<<ui->tableWidget->item(j,i)->text();
            else data<<"0";

        }
    data<<QString::number(tag,10);
    emit sendData(data);
    this->resetData();
    this->hide();
    //qDebug()<<data;
}
void add::resetData()
{
    ui->name->setText("");
    ui->id->setText("");
    for (int i=0;i<7;i++)
        for (int j=0;j<4;j++)
        {
            if (ui->tableWidget->item(j,i)!=NULL)
                ui->tableWidget->setItem(j,i,new QTableWidgetItem(""));
        }
}

