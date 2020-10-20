#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QVector>
#include "cube.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class Time{
public:
    int hour = 0;
    int minute = 0;
    int second = 0;
    int millisecond = 0;
};


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QTimer *m_timer;
    QTime m_time_counter;
    Time m_now_time;
    QVector<int> m_vt_num;
    Cube *pCube;
    int m_num_a;
    int m_num_b;
    int m_num_c;
    int m_num_d;
    int m_num_e;
    int m_num_f;
    int m_num_g;
    int m_num_h;

    QString clacTimeCounter();
    void initVtNum();
    bool isValid();
    bool regCheck(QString strValue);
    QString warningMessage(const QString str_message);
    QString inputNumLegalMsg();
    bool isSuccess();
    void helpMessage();
    QString checkInputNumLegalMsg();
    void findTip();
private slots:
    void on_timer_timeout () ; //定时溢出处理槽函数

    void on_pushButton_start_clicked();

    void on_pushButton_submit_clicked();

    void on_pushButton_help_clicked();

    void on_pushButton_tip_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
