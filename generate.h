#ifndef GENERATE_H
#define GENERATE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QMap>
#define MAXN 310
namespace Ui {
class generate;
}

class generate : public QWidget
{
    Q_OBJECT

public:
    explicit generate(QWidget *parent = nullptr);
    ~generate();

private slots:
    void showGeneratePage();
    void work();
    void work2();
    void navigateToMain();
    void exportSlot();
signals:
    void showMainPage();
private:

    void add(int x,int y,int v1,int v2);
    void add2(int x,int y,int f1,int f2,int v1,int v2);
    bool SPFA();
    bool bfs();
    int dfs(int x,int flow);
    Ui::generate *ui;
    QSqlQueryModel *queryModel;
    int n,m,S,T;
    struct edge{
        int to,f,v;
        edge *next,*rev;
    };
    struct path{
        int v;
        edge *e;
    };
    path pre[MAXN];
    bool vis[MAXN];
    int appearTime[MAXN];
    int dis[MAXN],usd[MAXN];
    int MAXUSED=2;
    QList<QString> stu;
    edge *head[MAXN],*v[MAXN];
    bool occ[MAXN][MAXN];
    //QList<edge> e;
    QMap<QString,QString> mp;
    int dep[MAXN],q[MAXN],rd[MAXN];
    QList<QPair<int,int> > id;
};

#endif // GENERATE_H
