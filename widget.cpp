#include <QTime>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include "widget.h"
#include "ui_widget.h"
#include "cube.h"

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_timer=new QTimer(this);
    m_timer->stop();

    ui->pushButton_help->setEnabled(true);
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_submit->setEnabled(false);
    ui->pushButton_tip->setEnabled(false);
    ui->lineEdit_a->setEnabled(false);
    ui->lineEdit_b->setEnabled(false);
    ui->lineEdit_c->setEnabled(false);
    ui->lineEdit_d->setEnabled(false);
    ui->lineEdit_e->setEnabled(false);
    ui->lineEdit_f->setEnabled(false);
    ui->lineEdit_g->setEnabled(false);
    ui->lineEdit_h->setEnabled(false);
    ui->lineEdit_a->setValidator(new QIntValidator(1, 99, this));
    ui->lineEdit_b->setValidator(new QIntValidator(1, 99, this));
    ui->lineEdit_c->setValidator(new QIntValidator(1, 99, this));
    ui->lineEdit_d->setValidator(new QIntValidator(1, 99, this));
    ui->lineEdit_e->setValidator(new QIntValidator(1, 99, this));
    ui->lineEdit_f->setValidator(new QIntValidator(1, 99, this));
    ui->lineEdit_g->setValidator(new QIntValidator(1, 99, this));
    ui->lineEdit_h->setValidator(new QIntValidator(1, 99, this));
    pCube = new Cube();
    QHBoxLayout container;
    container.addWidget(pCube);
    ui->openGLWidget->setLayout(&container);

    QObject::connect(m_timer,SIGNAL(timeout()),this,SLOT(on_timer_timeout()));
}


Widget::~Widget()
{
    delete ui;
    delete pCube;
}


void Widget::on_timer_timeout()
{
    QString str = "已用时间：" + clacTimeCounter();
    ui->label_timer->setText(str);
}


void Widget::on_pushButton_start_clicked()
{
    m_timer->start (1);//设置定时周期，单位：毫秒
    m_time_counter.start ();
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_submit->setEnabled(true);
    ui->pushButton_tip->setEnabled(true);
    ui->lineEdit_a->setEnabled(true);
    ui->lineEdit_b->setEnabled(true);
    ui->lineEdit_c->setEnabled(true);
    ui->lineEdit_d->setEnabled(true);
    ui->lineEdit_e->setEnabled(true);
    ui->lineEdit_f->setEnabled(true);
    ui->lineEdit_g->setEnabled(true);
    ui->lineEdit_h->setEnabled(true);

    QString str = "已用时间：" + clacTimeCounter();
    ui->label_timer->setText(str);

    bool is_init_success = false;
    while (!is_init_success) {
        initVtNum();
        if (isValid()) {
            is_init_success = true;
        }
    }

    str = "";
    for (int i=0; i<m_vt_num.size(); i++) {
        str = str + "  " + QString::number(m_vt_num[i]);
    }
    ui->label_arr_num->setText(str); // 记录数字，并输出
}


QString Widget::clacTimeCounter() {
    int elapsed = m_time_counter.elapsed();
    m_now_time.millisecond = elapsed%1000;
    elapsed = elapsed/1000;

    m_now_time.hour = elapsed/3600;
    elapsed = elapsed%3600;

    m_now_time.minute = elapsed/60;
    m_now_time.second = elapsed%60;

    return QString::asprintf("%02d:",m_now_time.hour) + QString::asprintf("%02d:",m_now_time.minute)
            + QString::asprintf("%02d:",m_now_time.second) + QString::asprintf("%03d",m_now_time.millisecond);
}


void Widget::initVtNum() {
    QTime time;
    time = QTime::currentTime();
    qsrand(time.msecsSinceStartOfDay());
    QVector<int> vt_rand_seq;
    const int MAX_RAND_SEQ_LEN = 3000000;
    for (int i=0; i<MAX_RAND_SEQ_LEN; i++) {
        vt_rand_seq.push_back(qrand() % 99 + 1);
    }
    int now_vt_rand_seq_index = 0;

    int times = 400000;
    int valid_times = 0;
    for(int now_times=1; now_times <= times; now_times++) {
        bool is_exist = false;
        now_vt_rand_seq_index = now_vt_rand_seq_index % MAX_RAND_SEQ_LEN;
        while(m_vt_num.size() < 8) {
            for (int vt_num_index=0; vt_num_index<m_vt_num.size(); vt_num_index++) {
                if (m_vt_num[vt_num_index] == vt_rand_seq[now_vt_rand_seq_index]) {
                    now_vt_rand_seq_index++;
                    is_exist = true;
                    break;
                }
            }
            if (!is_exist) {
                m_vt_num.push_back(vt_rand_seq[now_vt_rand_seq_index]);
                now_vt_rand_seq_index++;
            }
            is_exist = false;
        }
        qSort(m_vt_num.begin(), m_vt_num.end());

        if (isValid()) {
            break;
        }
        m_vt_num.clear();
    }
}


bool Widget::isValid() {
    int sum = m_vt_num[0] + m_vt_num[3] + m_vt_num[5] + m_vt_num[6];
    if ((m_vt_num[0] + m_vt_num[3] + m_vt_num[5] + m_vt_num[6] == sum) &&
        (m_vt_num[0] + m_vt_num[2] + m_vt_num[5] + m_vt_num[7] == sum) &&
        (m_vt_num[0] + m_vt_num[1] + m_vt_num[6] + m_vt_num[7] == sum) &&
        (m_vt_num[1] + m_vt_num[3] + m_vt_num[4] + m_vt_num[6] == sum) &&
        (m_vt_num[1] + m_vt_num[2] + m_vt_num[4] + m_vt_num[7] == sum) &&
        (m_vt_num[2] + m_vt_num[3] + m_vt_num[4] + m_vt_num[5] == sum)) {

        return true;
    }
    return false;
}

void Widget::on_pushButton_submit_clicked()
{
    m_num_a = ui->lineEdit_a->text().toInt();
    m_num_b = ui->lineEdit_b->text().toInt();
    m_num_c = ui->lineEdit_c->text().toInt();
    m_num_d = ui->lineEdit_d->text().toInt();
    m_num_e = ui->lineEdit_e->text().toInt();
    m_num_f = ui->lineEdit_f->text().toInt();
    m_num_g = ui->lineEdit_g->text().toInt();
    m_num_h = ui->lineEdit_h->text().toInt();
    QString msg = inputNumLegalMsg();
    if (!(msg == "OK")) {
        warningMessage(msg);
        return ;
    }

    m_timer->stop();
    m_time_counter.setHMS(0,0,0,0);
    QMessageBox::StandardButton reply;
    if (isSuccess()) {
        reply = QMessageBox::information(this, "胜利", "恭喜你，结果正确！\n是否需要记录时间？", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes){
            QString username = QInputDialog::getText(this, "胜利","恭喜你，结果正确！\n请输入您的用户名：", QLineEdit::Normal,"");
            if ("" != username) {
                QFile file("./record.ini");
                //不存在创建，存在则打开
                file.open(QIODevice::ReadWrite | QIODevice::Text);
                QString str = "time = " + m_time_counter.currentTime().toString() + "\n"
                              + "username = " + username + "\n"
                              + "timecost = " + QString::number(m_now_time.hour) + ":"
                                              + QString::number(m_now_time.minute) + ":"
                                              + QString::number(m_now_time.second) + ":"
                                              + QString::number(m_now_time.millisecond);
                file.write(str.toUtf8());
                file.close();
            }
        }
    } else {
        QMessageBox::information(this, "失败", "你输了，结果错误！", "continue");
    }

    reply = QMessageBox::information(this, "重新开始", "想重新开始一局吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        ui->pushButton_help->setEnabled(true);
        ui->pushButton_start->setEnabled(true);
        ui->pushButton_submit->setEnabled(false);
        ui->pushButton_tip->setEnabled(false);
        ui->label_arr_num->setText("");
        ui->label_timer->setText("");
        ui->lineEdit_a->setText("");
        ui->lineEdit_b->setText("");
        ui->lineEdit_c->setText("");
        ui->lineEdit_d->setText("");
        ui->lineEdit_e->setText("");
        ui->lineEdit_f->setText("");
        ui->lineEdit_g->setText("");
        ui->lineEdit_h->setText("");
        ui->lineEdit_a->setEnabled(false);
        ui->lineEdit_b->setEnabled(false);
        ui->lineEdit_c->setEnabled(false);
        ui->lineEdit_d->setEnabled(false);
        ui->lineEdit_e->setEnabled(false);
        ui->lineEdit_f->setEnabled(false);
        ui->lineEdit_g->setEnabled(false);
        ui->lineEdit_h->setEnabled(false);
        m_vt_num.clear();
    } else {
        exit(0);
    }
}

bool Widget::regCheck(QString strValue)
{
    bool is_int_num = true;

    QRegExp reg("^[-?\\d+]?\\d*$");
    QRegExpValidator validator(reg,0);
    int pos = 0;
    if(QValidator::Acceptable!=validator.validate(strValue,pos)){
        is_int_num = false;
    }

    return is_int_num;
}


QString Widget::warningMessage(const QString str_message)
{
    QMessageBox::warning(this, tr("警告"), str_message);
    return "继续";
}

void Widget::helpMessage()
{
    QString str_comment = "本游戏的目标：\n\n        通过在正方体的八个顶点上填数，实现正方体的六个面上，四个点对应数的和都相同。\n\n\n注意：只能使用给出的数字，且每个数字只能使用一次";
    QMessageBox::about(this, "游戏说明", str_comment);
}

QString Widget::inputNumLegalMsg() {
    QString msg = "";
    bool arr_is_visit[8] = {false};

    QVector<int>::iterator it;
    int num_index;

    it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_a);
    if (it == m_vt_num.end()) {
        msg = "a输入的数字不存在，请使用给出的数字！";
        return msg;
    }
    num_index = it - m_vt_num.begin();
    if (arr_is_visit[num_index]) {
        msg = "a输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
        return msg;
    }
    arr_is_visit[num_index] = true;

    it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_b);
    if (it == m_vt_num.end()) {
        msg = "b输入的数字不存在，请使用给出的数字！";
        return msg;
    }
    num_index = it - m_vt_num.begin();
    if (arr_is_visit[num_index]) {
        msg = "b输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
        return msg;
    }
    arr_is_visit[num_index] = true;

    it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_c);
    if (it == m_vt_num.end()) {
        msg = "c输入的数字不存在，请使用给出的数字！";
        return msg;
    }
    num_index = it - m_vt_num.begin();
    if (arr_is_visit[num_index]) {
        msg = "c输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
        return msg;
    }
    arr_is_visit[num_index] = true;

    it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_d);
    if (it == m_vt_num.end()) {
        msg = "d输入的数字不存在，请使用给出的数字！";
        return msg;
    }
    num_index = it - m_vt_num.begin();
    if (arr_is_visit[num_index]) {
        msg = "d输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
        return msg;
    }
    arr_is_visit[num_index] = true;

    it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_e);
    if (it == m_vt_num.end()) {
        msg = "e输入的数字不存在，请使用给出的数字！";
        return msg;
    }
    num_index = it - m_vt_num.begin();
    if (arr_is_visit[num_index]) {
        msg = "e输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
        return msg;
    }
    arr_is_visit[num_index] = true;

    it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_f);
    if (it == m_vt_num.end()) {
        msg = "f输入的数字不存在，请使用给出的数字！";
        return msg;
    }
    num_index = it - m_vt_num.begin();
    if (arr_is_visit[num_index]) {
        msg = "f输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
        return msg;
    }
    arr_is_visit[num_index] = true;

    it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_g);
    if (it == m_vt_num.end()) {
        msg = "g输入的数字不存在，请使用给出的数字！";
        return msg;
    }
    num_index = it - m_vt_num.begin();
    if (arr_is_visit[num_index]) {
        msg = "g输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
        return msg;
    }
    arr_is_visit[num_index] = true;

    it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_h);
    if (it == m_vt_num.end()) {
        msg = "h输入的数字不存在，请使用给出的数字！";
        return msg;
    }
    num_index = it - m_vt_num.begin();
    if (arr_is_visit[num_index]) {
        msg = "h输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
        return msg;
    }
    arr_is_visit[num_index] = true;

    msg = "OK";
    return msg;
}

bool Widget::isSuccess() {
    int sum = m_num_a + m_num_b + m_num_c + m_num_d;
    if ((m_num_a + m_num_b + m_num_c + m_num_d == sum) &&
        (m_num_a + m_num_c + m_num_e + m_num_g == sum) &&
        (m_num_a + m_num_b + m_num_e + m_num_f == sum) &&
        (m_num_b + m_num_d + m_num_h + m_num_f == sum) &&
        (m_num_c + m_num_d + m_num_g + m_num_h == sum) &&
        (m_num_e + m_num_f + m_num_g + m_num_h == sum)) {
        return true;
    }
    return false;
}

void Widget::on_pushButton_help_clicked()
{
    helpMessage();
}

void Widget::on_pushButton_tip_clicked()
{
    m_num_a = ui->lineEdit_a->text().toInt();
    m_num_b = ui->lineEdit_b->text().toInt();
    m_num_c = ui->lineEdit_c->text().toInt();
    m_num_d = ui->lineEdit_d->text().toInt();
    m_num_e = ui->lineEdit_e->text().toInt();
    m_num_f = ui->lineEdit_f->text().toInt();
    m_num_g = ui->lineEdit_g->text().toInt();
    m_num_h = ui->lineEdit_h->text().toInt();

    QString msg = checkInputNumLegalMsg();
    if (!(msg == "OK")) {
        warningMessage(msg);
        return ;
    }

    findTip();
}


QString Widget::checkInputNumLegalMsg() {
    QString msg = "";
    bool arr_is_visit[8] = {false};

    QVector<int>::iterator it;
    int num_index;

    if (m_num_a != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_a);
        if (it == m_vt_num.end()) {
            msg = "a输入的数字不存在，请使用给出的数字！";
            return msg;
        }
        num_index = it - m_vt_num.begin();
        if (arr_is_visit[num_index]) {
            msg = "a输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
            return msg;
        }
        arr_is_visit[num_index] = true;
    }

    if (m_num_b != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_b);
        if (it == m_vt_num.end()) {
            msg = "b输入的数字不存在，请使用给出的数字！";
            return msg;
        }
        num_index = it - m_vt_num.begin();
        if (arr_is_visit[num_index]) {
            msg = "b输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
            return msg;
        }
        arr_is_visit[num_index] = true;
    }

    if (m_num_c != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_c);
        if (it == m_vt_num.end()) {
            msg = "c输入的数字不存在，请使用给出的数字！";
            return msg;
        }
        num_index = it - m_vt_num.begin();
        if (arr_is_visit[num_index]) {
            msg = "c输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
            return msg;
        }
        arr_is_visit[num_index] = true;
    }

    if (m_num_d != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_d);
        if (it == m_vt_num.end()) {
            msg = "d输入的数字不存在，请使用给出的数字！";
            return msg;
        }
        num_index = it - m_vt_num.begin();
        if (arr_is_visit[num_index]) {
            msg = "d输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
            return msg;
        }
        arr_is_visit[num_index] = true;
    }

    if (m_num_e != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_e);
        if (it == m_vt_num.end()) {
            msg = "e输入的数字不存在，请使用给出的数字！";
            return msg;
        }
        num_index = it - m_vt_num.begin();
        if (arr_is_visit[num_index]) {
            msg = "e输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
            return msg;
        }
        arr_is_visit[num_index] = true;
    }

    if (m_num_f != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_f);
        if (it == m_vt_num.end()) {
            msg = "f输入的数字不存在，请使用给出的数字！";
            return msg;
        }
        num_index = it - m_vt_num.begin();
        if (arr_is_visit[num_index]) {
            msg = "f输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
            return msg;
        }
        arr_is_visit[num_index] = true;
    }

    if (m_num_g != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_g);
        if (it == m_vt_num.end()) {
            msg = "g输入的数字不存在，请使用给出的数字！";
            return msg;
        }
        num_index = it - m_vt_num.begin();
        if (arr_is_visit[num_index]) {
            msg = "g输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
            return msg;
        }
        arr_is_visit[num_index] = true;
    }

    if (m_num_h != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_h);
        if (it == m_vt_num.end()) {
            msg = "h输入的数字不存在，请使用给出的数字！";
            return msg;
        }
        num_index = it - m_vt_num.begin();
        if (arr_is_visit[num_index]) {
            msg = "h输入的数字已经使用过，每个给出的数字只能使用一次。请使用其他给出的数字！";
            return msg;
        }
        arr_is_visit[num_index] = true;
    }

    msg = "OK";
    return msg;
}


void Widget::findTip(){
    bool arr_is_visit[8] = {false};
    QVector<int>::iterator it;
    int num_index;

    if (m_num_a != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_a);
        if (it != m_vt_num.end()) {
            arr_is_visit[*it] = true;
        }
    }
    if (m_num_b != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_b);
        if (it != m_vt_num.end()) {
            arr_is_visit[*it] = true;
        }
    }
    if (m_num_c != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_c);
        if (it != m_vt_num.end()) {
            arr_is_visit[*it] = true;
        }
    }
    if (m_num_d != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_d);
        if (it != m_vt_num.end()) {
            arr_is_visit[*it] = true;
        }
    }
    if (m_num_e != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_e);
        if (it != m_vt_num.end()) {
            arr_is_visit[*it] = true;
        }
    }
    if (m_num_f != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_f);
        if (it != m_vt_num.end()) {
            arr_is_visit[*it] = true;
        }
    }
    if (m_num_g != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_g);
        if (it != m_vt_num.end()) {
            arr_is_visit[*it] = true;
        }
    }
    if (m_num_h != 0) {
        it = std::find(m_vt_num.begin(), m_vt_num.end(), m_num_h);
        if (it != m_vt_num.end()) {
            arr_is_visit[*it] = true;
        }
    }


}
