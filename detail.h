#ifndef DETAIL_H
#define DETAIL_H


#include <QWidget>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class detail; }
QT_END_NAMESPACE

class detail : public QWidget
{
    Q_OBJECT

public:
    detail(QWidget *parent = nullptr);
    ~detail();
private slots:
    void updateData();
    void deleteData();
    void showDetailPage();
    void navigateToMain();
    void navigateToAdd();
    void navigateToModify();
    void multiAddData();
    void changeData();
    void getData(QStringList);
    //void modifyData();
    //void on_buttonDelete_clicked();
    //void on_buttonSure_clicked();
    //void on_buttonCancel_clicked();
signals:
    void showMainPage();
    void showAddPage(int p);
private:
    Ui::detail *ui;
    int row,column;
    int lineNum;
    QSqlQueryModel *queryModel;
};

#endif // DETAIL_H
