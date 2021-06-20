#include "generate.h"
#include "ui_generate.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlTableModel>
#include <QSqlRecord>
#include<QFileDialog>
#include<QSettings>
#include<QProcess>

generate::generate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::generate)
{

    ui->setupUi(this);
    this->setWindowTitle("值班表");
    connect(ui->quickButton,SIGNAL(clicked()),this,SLOT(work()));
    connect(ui->fullButton,SIGNAL(clicked()),this,SLOT(work2()));
    connect(ui->returnButton,SIGNAL(clicked()),this,SLOT(navigateToMain()));
    connect(ui->exportButton,SIGNAL(clicked()),this,SLOT(exportSlot()));

}

generate::~generate()
{
    delete ui;
}
void generate::exportSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"result",tr("file (*.csv)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        bool ret = file.open( QIODevice::Truncate | QIODevice::WriteOnly);
        if(!ret)
            return;

        QTextStream stream(&file);
        QString conTents;
        conTents+=",";
        QHeaderView * header1 = ui->tableWidget->horizontalHeader() ;
        if (header1)
        {
            for ( int i = 0; i < header1->count(); i++ )
            {
                QTableWidgetItem *item = ui->tableWidget->horizontalHeaderItem(i);
                if (!item)
                {
                    continue;
                }
                conTents += item->text();
                if (i!=header1->count()-1) conTents += ",";
            }
            conTents += "\n";
        }
        for ( int i = 0; i < ui->tableWidget->rowCount(); i++ )
        {
            conTents+=ui->tableWidget->verticalHeaderItem(i)->text()+",";
            for ( int j = 0; j < ui->tableWidget->columnCount(); j++ )
            {

                QTableWidgetItem* item = ui->tableWidget->item(i, j);
                if ( !item || item->text()=="")
                {

                }
                else conTents+=item->text();
                if (j!=ui->tableWidget->columnCount()-1) conTents +=",";
            }
            if (i!=ui->tableWidget->rowCount()-1) conTents += "\n";
        }
        stream << conTents;
        file.close();
        QMessageBox::information(0,QObject::tr("文件导出"),QString("文件导出成功！"));
    }
}
void generate::navigateToMain()
{
    this->hide();
    emit showMainPage();
}
void generate::showGeneratePage()
{
    this->show();
}
void generate::add(int x,int y,int v1,int v2)
{
    edge* e1= new edge;edge* e2=new edge;
    e1->to=y;
    e1->f=v1;
    e1->next=v[x];
    v[x]=e1;
    e2->to=x;
    e2->f=v2;
    e2->next=v[y];
    v[y]=e2;
    e1->rev=e2;
    e2->rev=e1;
}
bool generate::bfs()
{
    for (int i=S;i<=T;i++) dep[i]=0;
    int h=0,t=1,x,y;
    q[1]=S;dep[S]=1;
    while (h!=t)
    {
        x=q[++h];
        for (edge *i=v[x];i!=NULL;i=i->next)
        {
            y=i->to;
            if ((!dep[y])&&i->f)
            {
                dep[y]=dep[x]+1;
                q[++t]=y;
            }
        }
    }
    return dep[T]!=0;
}
int generate::dfs(int x,int flow)
{
    if (x==T) return flow;
    int nw=0,used=0;
    for (edge *i=head[x];i!=NULL;i=i->next)
    {
        int y=i->to;
        if (i->f&&dep[y]==dep[x]+1)
        {
            nw=dfs(y,qMin(flow-used,i->f));
            used+=nw;
            i->f-=nw;
            i->rev->f+=nw;
            head[x]=i;
            if (flow==used) break;
        }
    }
    if (!used) dep[x]=-1;
    return used;
}
void generate::work()
{
    for (int i=0;i<7;i++)
            for (int j=0;j<4;j++)
            {
                if (ui->tableWidget->item(j,i)!=NULL)
                    ui->tableWidget->setItem(j,i,new QTableWidgetItem(""));
            }
    QSqlQuery query;
    QString sql=QString("select * from timedata");
    query.exec(sql);
    queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    n = queryModel->rowCount();
    m = queryModel->columnCount();
    S=0;T=n+m-2;
    for (int i=S;i<=T;i++) head[i]=v[i]=NULL;
    for (int i=0;i<n;i++) rd[i]=i;
    std::random_shuffle(rd,rd+n);
    for (int i=1;i<=n;i++) this->add(S,rd[i-1]+1,1,0);

    for (int ii=0;ii<n;ii++)
    {
        int i=rd[ii];
        //qDebug()<<i;
        QSqlRecord sqlRecord=queryModel->record(i);
        //stu.append(sqlRecord.value(0).toString());
        for (int j=2;j<m-1;j++)
            if (sqlRecord.value(j).toString()=="0")
                this->add(i+1,n+j-1,1,0);
    }
    for (int i=2;i<m-1;i++)
        this->add(n+i-1,T,MAXUSED,0);
    int total=0;
    while (this->bfs())
    {

        for (int i=S;i<=T;i++) head[i]=v[i];
        total+=this->dfs(S,100);
    }
    for (int x=1;x<=n;x++)
        for (edge *i=head[x];i!=NULL;i=i->next)
        {
            //qDebug()<<x<<" "<<i->to;
            if (i->f==0)
            {
                int y=i->to;
                if (x>y) continue;
                int day=(y-n-1)/4;
                int tim=(y-n-1)%4;
                QString ad=queryModel->record(x-1).value(1).toString();
                //qDebug()<<ad<<" "<<day<<" "<<tim<<" "<<y;
                if (ui->tableWidget->item(tim,day)!=NULL)
                    ad+=" "+ui->tableWidget->item(tim,day)->text();
                ui->tableWidget->setItem(tim,day,new QTableWidgetItem(ad));
            }
        }
    ui->tableWidget->resizeColumnsToContents();
    for (int x=S;x<=T;x++)
        for (edge *i=head[x];i!=NULL;)
        {
            edge *nw=i;
            i=i->next;
            delete nw;
        }
    //ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //qDebug()<<total;
}
void generate::add2(int x,int y,int f1,int f2,int v1,int v2)
{
    edge* e1= new edge;edge* e2=new edge;
    e1->to=y;
    e1->f=f1;
    e1->v=v1;
    e1->next=head[x]->next;
    head[x]->next=e1;
    e2->to=x;
    e2->f=f2;
    e2->v=v2;
    e2->next=head[y]->next;
    head[y]->next=e2;
    e1->rev=e2;
    e2->rev=e1;
}
bool generate::SPFA()
{
    for (int i=S;i<=T;i++)
        vis[i]=0,dis[i]=1e9;
    int h=0,t=0,x,y;
    q[++t]=S;vis[S]=1;dis[S]=0;
    //qDebug()<<"SPFA";
    while (h!=t)
    {
        //qDebug()<<h<<" "<<t;
        h++;if (h>=T+4) h-=T+4;
        x=q[h];vis[x]=0;
        for (edge *i=head[x]->next;i!=NULL;i=i->next)
        {
            y=i->to;
            if (i->f&&dis[y]>dis[x]+i->v)
            {
                //qDebug()<<x<<" "<<y;
                dis[y]=dis[x]+i->v;
                pre[y].v=x;
                pre[y].e=i;
                if (!vis[y])
                {
                    t++;
                    if (t>=T+4) t-=T+4;
                    q[t]=y;
                    vis[y]=1;
                }
            }
        }
    }
    return (dis[T]!=1e9);
}
void generate::work2()
{
    for (int i=0;i<7;i++)
            for (int j=0;j<4;j++)
            {
                if (ui->tableWidget->item(j,i)!=NULL)
                    ui->tableWidget->setItem(j,i,new QTableWidgetItem(""));
            }
    QSqlQuery query;
    QString sql=QString("select * from timedata");
    query.exec(sql);
    queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    n = queryModel->rowCount();
    m = queryModel->columnCount();
    S=0;T=n+m-2;
    for (int i=S;i<=T;i++) head[i]=new edge,head[i]->next=NULL;
    for (int i=0;i<n;i++) rd[i]=i,appearTime[i+1]=0;
    for (int i=S;i<=T;i++) usd[i]=0;
    std::random_shuffle(rd,rd+n);
    //for (int i=0;i<n;i++) qDebug()<<rd[i];
    for (int i=1;i<=n;i++) this->add2(S,rd[i-1]+1,1,0,0,0);
    for (int ii=0;ii<n;ii++)
    {
        int i=rd[ii];
        //qDebug()<<i;
        QSqlRecord sqlRecord=queryModel->record(i);
        //stu.append(sqlRecord.value(0).toString());
        for (int j=2;j<m-1;j++)
            if (sqlRecord.value(j).toString()=="0")
                this->add2(i+1,n+j-1,1,0,appearTime[i],-appearTime[i]);
    }
    for (int i=2;i<m-1;i++)
        this->add2(n+i-1,T,MAXUSED-usd[i-2],0,0,0);
    //for (int i=S;i<=T;i++) pre[i]=NULL;
    int ansv=0,ansf=0;
    do
    {
        //for (int x=S;x<=T;x++)
        //    for (edge *i=head[x]->next;i!=NULL;i=i->next)
        //        qDebug()<<i->to<<" "<<i->f<<" "<<i->v;
        ansv=0,ansf=0;
        while (SPFA())
        {
            int F=1e9;
            for (int i=T;i!=S;i=pre[i].v)
                 F=qMin(F,pre[i].e->f);
            //qDebug()<<F;
            ansv+=F*dis[T];
            ansf+=F;
            //qDebug()<<"path";
            for (int i=T;i!=S;i=pre[i].v)
            {
                //qDebug()<<i;
                pre[i].e->f-=F;
                pre[i].e->rev->f+=F;
            }
        }
//        for (int x=S;x<=T;x++)
//            for (edge *i=head[x]->next;i!=NULL;i=i->next)
//            {
//                if (i->to>x)
//                {
//                    qDebug()<<x<<" "<<i->to<<" "<<i->f<<" "<<i->v;
//                    qDebug()<<i->to<<" "<<i->rev->to<<" "<<i->rev->f<<" "<<i->rev->v;

//                }
//            }
        for (int x=1;x<=n;x++)
        {
            for (edge *i=head[x]->next;i!=NULL;i=i->next)
            {
                //qDebug()<<x<<" "<<i->to;
                if (i->rev->f!=0)
                {
                    int y=i->to;
                    if (x>y) continue;
                    occ[x][y]=1;
                    appearTime[x]++;
                    usd[y]++;
                    int day=(y-n-1)/4;
                    int tim=(y-n-1)%4;
                    //qDebug()<<x<<" "<<day<<" "<<tim;
                    QString ad=queryModel->record(x-1).value(1).toString();
                    //qDebug()<<ad<<" "<<day<<" "<<tim<<" "<<y;
                    if (ui->tableWidget->item(tim,day)!=NULL)
                        ad+=" "+ui->tableWidget->item(tim,day)->text();
                    ui->tableWidget->setItem(tim,day,new QTableWidgetItem(ad));
                    //pre->next=i->next;
                }
            }
        }
        for (edge *i=head[S]->next;i!=NULL;i=i->next)
            i->f=1,i->rev->f=0;
        for (int x=1;x<=n;x++)
        {
            for (edge *i=head[x]->next;i!=NULL;i=i->next)
                if (i->to>x)
                {
                    i->f=!occ[x][i->to];
                    i->v=appearTime[x];
                    i->rev->f=0;
                    i->rev->v=-appearTime[x];
                }
        }
        for (int xx=0;xx<m-3;xx++)
        {
            int x=xx+n+1;
            if (usd[x]>2) qDebug()<<x<<"!!!!!!!!!!!!";
            //qDebug()<<usd[x];
            for (edge *i=head[x]->next;i!=NULL;i=i->next)
                if (i->to>x)
                {
                    i->f=MAXUSED-usd[x];
                    i->rev->f=0;
                }
        }
//        for (int x=S;x<=T;x++)
//            for (edge *i=head[x]->next;i!=NULL;i=i->next)
//            {
//                if (i->to>x)
//                {
//                    qDebug()<<x<<" "<<i->to<<" "<<i->f<<" "<<i->v;
//                    qDebug()<<i->to<<" "<<i->rev->to<<" "<<i->rev->f<<" "<<i->rev->v;

//                }
//            }
//       qDebug()<<"";
     //   for (int i=1;i<=n;i++)
     //       qDebug()<<appearTime[i];
    }while(ansf);
    //qDebug()<<"end";
    ui->tableWidget->resizeColumnsToContents();
    for (int x=S;x<=T;x++)
        for (edge *i=head[x]->next;i!=NULL;)
        {
            occ[x][i->to]=0;
            edge *nw=i;
            i=i->next;
            delete nw;
        }
    for (int i=S;i<=T;i++) delete head[i];
        //qDebug()<<ansf;
}
