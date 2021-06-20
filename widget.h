#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <detail.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void navigateToDetail();
    void navigateToGenerate();
    void showMainPage();
    //void on_buttonDelete_clicked();
    //void on_buttonSure_clicked();
    //void on_buttonCancel_clicked();
signals:
    void showDetailPage();
    void showGeneratePage();
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
