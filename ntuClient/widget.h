#ifndef WIDGET_H
#define WIDGET_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QTime>
#include <QWidget>
#include <QByteArray>
//#include <QSystemTrayIcon>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void repleyFinished(QNetworkReply*);//结束请求槽
    void save(QString username, QString password);//把用户信息保存到本地
    void read();//读取用户信息
    void setTotalTime();//设置用户上网总时间
    void on_login_pushButton_clicked();//登陆按钮
    void on_logout_pushButton_clicked();//登出按钮
    //void iconIsActived(QSystemTrayIcon::ActivationReason);//最小到托盘，单击显示窗体

private:
    Ui::Widget *ui;
    QNetworkAccessManager *manager;//管理网络操作
    QNetworkReply *reply;//处理网络应答
    QNetworkRequest request;//网络请求
    QFile *file;//文件
    QString username;//用户名
    QString password;//密码
    bool flag;//控制槽来响应那个信号(登陆,登出)
    QTime start_current_time;//当前开始时间
    QTime end_current_time;//当前结束时间
    qint64 start_hour;//开始小时
    qint64 start_minute;//开始分钟
    qint64 start_second;//开始秒
    qint64 end_hour;//结束小时
    qint64 end_minute;//结束分钟
    qint64 end_second;//结束秒
    QByteArray result_uid;//记录登出成功服务器的返回值，留作登出时使用
};

#endif // WIDGET_H
