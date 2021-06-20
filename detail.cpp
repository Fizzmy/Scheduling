#include "detail.h"
#include "ui_detail.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QFileDialog>
#include <iostream>


detail::detail(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::detail)
{
    ui->setupUi(this);
    this->setWindowTitle("排班数据");
    //qDebug()<<QSqlDatabase::drivers();
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("127.0.0.1"); //数据库服务器IP
//    db.setUserName("root"); //数据库用户名
//    db.setPassword("1234"); //数据库密码
//    db.setDatabaseName("information");
//    if (!db.isOpen())
//        if( !db.open() ){ //数据库打开失败
//            QMessageBox::warning(this,"错误",db.lastError().text());
//            return;
//        }

   //this->updateData();

    connect(ui->returnButton,SIGNAL(clicked()),this,SLOT(navigateToMain()));

    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(navigateToAdd()));

    //connect(ui->changeButton,SIGNAL(clicked()),this,SLOT(changeData()));

    connect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(deleteData()));

    connect(ui->modifyButton,SIGNAL(clicked()),this,SLOT(navigateToModify()));

    connect(ui->multiaddButton,SIGNAL(clicked()),this,SLOT(multiAddData()));
}

detail::~detail()
{
    delete ui;
}
void detail::multiAddData()
{
    QString strs;
    QStringList file_list, output_name;
    QStringList str_path_list = QFileDialog::getOpenFileNames(this, tr("选择导入文件"),"", tr("file (*.csv)"));
    if (!str_path_list.size()) return;
        for (int i = 0; i < str_path_list.size(); i++){
            QString str_path = str_path_list[i];

            QFile file(str_path);

            QFileInfo File = QFileInfo(str_path);
            QString file_name = File.fileName();

            QStringList name=file_name.split(".");
            QStringList mess=name[0].split(" ");

            QTextStream textStream(&file);
            bool ret = file.open( QIODevice::ReadOnly);
            if(!ret)
                return;

            //qDebug()<<"begin";
            //qDebug()<<textStream.readLine();
            textStream.readLine();
            QStringList final,sortFinal;
            while (!textStream.atEnd())
            {
                QStringList data=textStream.readLine().split(",");
                for (int i=1;i<8;i++)
                {
                    if (i<data.count())
                        if (data[i]!="") final<<data[i];
                        else final<<"0";
                    else final<<"0";
                }
            }
            sortFinal<<mess[1];
            sortFinal<<mess[0];
            for (int i=0;i<7;i++)
                for (int j=0;j<4;j++)
                    sortFinal<<final[j*7+i];
            QString a="INSERT INTO timedata VALUES (";
            for (int i=0;i<sortFinal.count();i++)
            {
                a.append("'");

                a.append(sortFinal[i]);
                a.append("'");
                a.append(",");

            }
            a.append("'");
            int maxn=queryModel->record(row-1).value(column).toInt()+1+i;
            a.append(QString::number(maxn,10));
            a.append("')");
            //qDebug()<<a;
            QSqlQuery query;
            query.exec(a);

        }
        this->updateData();
        QMessageBox::information(0,QObject::tr("文件导入"),QString("文件导入成功！"));
}
void detail::updateData()
{
    QSqlQuery query;
    row=0,column=0;
    QString sql=QString("select * from timedata");
    query.exec(sql);
    queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    row = queryModel->rowCount();
    ui->tableWidget->setRowCount(row);
    column = queryModel->columnCount();
    column--;
    ui->tableWidget->setColumnCount(column);

    QSqlRecord sqlRecord=queryModel->record(0);
    QStringList header;
    for (int i=0;i<sqlRecord.count();i++)
        header<<sqlRecord.fieldName(i);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    for (int i=0;i<row;i++)
    {
        sqlRecord=queryModel->record(i);
        for (int j=0;j<column;j++)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(sqlRecord.value(j).toString()));
        }
    }
}
void detail::showDetailPage()
{
    this->updateData();
    this->show();
}
void detail::navigateToMain()
{
    this->hide();
    emit showMainPage();
}
void detail::navigateToAdd()
{
    this->hide();
    emit showAddPage(0);
}
void detail::navigateToModify()
{
    int x=ui->modifyLine->text().toInt();
    lineNum=x;
    if (x>=1&&x<=row)
    {
        ui->modifyLine->setText("");
        this->hide();
        emit showAddPage(queryModel->record(x-1).value(column).toInt());
    }
    else
        QMessageBox::warning(this,"错误","数据不合法");

}
void detail::changeData()
{
    QSqlRecord sqlRecord;
    QSqlQuery query;
    for (int i=0;i<row;i++)
    {
        QString nw;
        sqlRecord=queryModel->record(i);
        int tag=sqlRecord.value(column).toInt();
        for (int j=0;j<column;j++)
        {
            nw=ui->tableWidget->item(i,j)->text();
            if (nw!=sqlRecord.value(j).toString())
            {
                query.exec(QString("update timedata set %1='%2' where tag=%3").arg(sqlRecord.fieldName(j)).arg(nw).arg(tag));
            }
            //qDebug()<<query.lastError().text();
            //ui->tableWidget->item(i,j)->text();
            //list<<sqlRecord.value(j).toString();
        }
    }
}
void detail::getData(QStringList data)
{
    QString tag=data[data.count()-1];
    if (tag=="0")
    {
        QString a="INSERT INTO timedata VALUES (";
        for (int i=0;i<data.count()-1;i++)
        {
            a.append("'");

            a.append(data[i]);
            a.append("'");
            a.append(",");

        }
        a.append("'");
        int maxn=queryModel->record(row-1).value(column).toInt()+1;
        a.append(QString::number(maxn,10));
        a.append("')");
        QSqlQuery query;
        query.exec(a);
    }
    else
    {
        QSqlQuery query;
        QSqlRecord sqlRecord;

        sqlRecord=queryModel->record(lineNum-1);
        QString nw;
        qDebug()<<tag;
        for (int j=0;j<column;j++)
        {
            nw=data[j];
            if (nw!=sqlRecord.value(j).toString())
            {
                query.exec(QString("update timedata set %1='%2' where tag=%3").arg(sqlRecord.fieldName(j)).arg(nw).arg(tag));
            }
            //qDebug()<<query.lastError().text();
            //ui->tableWidget->item(i,j)->text();
            //list<<sqlRecord.value(j).toString();
        }
    }
    this->updateData();
    this->show();
    //qDebug()<<query.lastError().text();

}

void detail::deleteData()
{
    QString del=ui->deleteLine->text();
    int pos=del.toInt();
    if (pos>=1&&pos<=row)
    {
        ui->deleteLine->setText("");
        QString a="DELETE FROM timedata WHERE tag='";
        a.append(queryModel->record(pos-1).value(column).toString());
        a.append("'");
        QSqlQuery query;
        query.exec(a);
        this->updateData();
    }
    else QMessageBox::warning(this,"错误","数据不合法");

}
