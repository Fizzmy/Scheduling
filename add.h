#ifndef ADD_H
#define ADD_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class add;
}

class add : public QWidget
{
    Q_OBJECT

public:
    explicit add(QWidget *parent = 0);
    ~add();
private slots:
    void showAddPage(int x);
    void navigateToDetail();
    void submitData();
    void resetData();
signals:
    void showDetailPage();
    void sendData(QStringList data);
private:
    Ui::add *ui;
    int tag;
    QSqlQueryModel *queryModel;
};

#endif // ADD_H
