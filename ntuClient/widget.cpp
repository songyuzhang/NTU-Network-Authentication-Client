#include "widget.h"
#include "ui_widget.h"
#include <QPalette>
#include <QPixmap>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QSystemTrayIcon>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    Qt::WindowFlags window_flag = Qt::Window
        | Qt::WindowCloseButtonHint
        | Qt::WindowMinimizeButtonHint;

    this->setWindowFlags(window_flag);
    //设置背景图片
    QPalette palette;
    QPixmap pixmap(":/images/background.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);
    flag = true;//登陆 true, 登出false
    manager = new QNetworkAccessManager(this);
    read();//从本地读取用户信息
    connect(manager,SIGNAL(finished(QNetworkReply*)), this, SLOT(repleyFinished(QNetworkReply*)));
    //设置最小化到托盘
    /*QSystemTrayIcon * systemTrayIcon = new QSystemTrayIcon();
    systemTrayIcon->setToolTip(QString("南通大学上网认证客户端v1.2"));
    systemTrayIcon->setIcon(QIcon(":/images/tray.jpg"));
    connect(systemTrayIcon , SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));*/

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_login_pushButton_clicked()//登陆信号
{
   QUrl url;
   if(ui->comboBox->currentIndex() == 1)//判断用户选择
   {
       url = "http://172.15.0.2/cgi-bin/do_login";
       //qDebug() << "使用宿舍区登陆";
   }
   else if(ui->comboBox->currentIndex() == 2)
   {
       url = "http://210.29.65.198:3333/cgi-bin/do_login";
       //qDebug() << "使用教学区登陆";
   }
   else
   {
       QMessageBox::warning(NULL,"warning",tr("请选择上网区域"),QMessageBox::Ok);//弹出警告窗口
       return;
   }
   //QUrl url("http://172.15.0.2/cgi-bin/do_login_juniper");
   //qDebug() << url;
   username = ui->username_lineEdit->text();
   password = ui->password_lineEdit->text();
   if(username == "" || password == "")
   {
      // QMessageBox::warning(NULL,"warning",tr("请填写用户名和密码"),QMessageBox::Ok);//弹出警告窗口
       return;
   }
   if(ui->save_checkBox->isChecked())//保存用户信息
   {
       save(username, password);
   }
   //qDebug() << username << password;
   request.setUrl(url);
   request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");//设置http头类型
   QByteArray postData;
   postData.append("username=" + username + "&");
   postData.append("password=" + password);
   reply = manager->post(request, postData);//向服务器发送数据
   ui->showState_label->setText(tr("登陆中...."));
}


void Widget::repleyFinished(QNetworkReply *r)//处理结束信号
{
    if(flag)//处理登陆信号
    {
         QByteArray bytes = r->readAll();  // 记录服务器返回值
         if(bytes.at(0) >= '0' && bytes.at(0) <= '9')//返回数字则登陆成功,此处只截取返回字符串的第一位数字
         {
             result_uid = bytes;
             ui->showState_label->setText(tr("<font color = red>登陆成功</font>"));
             ui->save_checkBox->setEnabled(false);//保存密码失效
             ui->login_pushButton->setEnabled(false);//登陆按钮失效
             ui->logout_pushButton->setEnabled(true);//激活登出按钮
             ui->groupBox->setEnabled(true);//激活信息栏
             flag = false;
             QDateTime start_current_date_time = QDateTime::currentDateTime();//记录系统当前时间
             QString satrt_current_date = start_current_date_time.toString("yyyy-MM-dd hh:mm:ss");//设置时间格式
             start_current_time = QTime::currentTime();
             start_hour = start_current_time.hour();
             start_minute = start_current_time.minute();
             start_second = start_current_time.second();
             ui->showOnlineTime_label->setText(satrt_current_date);
             ui->showOfflineTime_label->setText("");
             ui->showTotalTime_label->setText("");
             ui->comboBox->setEnabled(false);
         }
         else//登陆失败
         {
             if(bytes == "user_tab_error")
             {
                 ui->showState_label->setText(tr("<font color = red>认证程序未启动!</font>"));
             }

             else if(bytes == "username_error")
             {
                 ui->showState_label->setText(tr("<font color = red>用户名错误!</font>"));
             }

             else if(bytes == "non_auth_error")
             {
                 ui->showState_label->setText(tr("<font color = red>您无须认证，可直接上网!</font>"));
             }

             else if(bytes == "password_error")
             {
                 ui->showState_label->setText(tr("<font color = red>密码错误!</font>"));
             }

             else if(bytes == "status_error")
             {
                 ui->showState_label->setText(tr("<font color = red>用户已欠费，请尽快充值!</font>"));
             }

             else if(bytes == "available_error")
             {
                 ui->showState_label->setText(tr("<font color = red>用户已禁用!</font>"));
             }

             else if(bytes == "ip_exist_error")
             {
                 ui->showState_label->setText(tr("<font color = red>您的IP尚未下线，请等待2分钟再试!</font>"));
             }

             else if(bytes == "usernum_error")
             {
                 ui->showState_label->setText(tr("<font color = red>用户数已达上限!</font>"));
             }

             else if(bytes == "online_num_error")
             {
                 ui->showState_label->setText(tr("<font color = red>该帐号的登录人数已超过限额\n如果怀疑帐号被盗用，请联系管理员!</font>"));
                 /*ui->logoutPushButton->setEnabled(true);
                 flag = false;
                 return;*/
             }

             else if(bytes == "mode_error")
             {
                 ui->showState_label->setText(tr("<font color = red>系统已禁止登陆!</font>"));
             }

             else if(bytes == "time_policy_error")
             {
                 ui->showState_label->setText(tr("<font color = red>当前时段不允许连接!</font>"));
             }

             else if(bytes == "flux_error")
             {
                 ui->showState_label->setText(tr("<font color = red>您的流量已超支!</font>"));
             }

             else if(bytes == "minutes_error")
             {
                 ui->showState_label->setText(tr("<font color = red>您的时长已超支!</font>"));
             }

             else if(bytes == "ip_error")
             {
                 ui->showState_label->setText(tr("<font color = red>您的IP地址不合法!</font>"));
             }

             else if(bytes == "mac_error")
             {
                 ui->showState_label->setText(tr("<font color = red>您的MAC地址不合法!</font>"));
             }

             else if(bytes == "sync_error")
             {
                 ui->showState_label->setText(tr("<font color = red>您的资料已修改，正在等待同步，请2分钟后再试!</font>"));
             }

             else if(bytes == "coa_error")
             {
                 ui->showState_label->setText(tr("<font color = red>COA错误!</font>"));
             }

             else
             {
                 ui->showState_label->setText(tr("<font color = red>找不到认证服务器!</font>"));
             }
         }
     }
    else //处理退出信号
     {
         QByteArray bytes = r->readAll();  // bytes
         if(bytes == "logout_ok")
         {
             ui->showState_label->setText(tr("<font color = red >下线成功</font>"));
             flag = true;
             ui->logout_pushButton->setEnabled(false);
             ui->save_checkBox->setEnabled(true);
             ui->login_pushButton->setEnabled(true);
             end_current_time = QTime::currentTime();
             end_hour = end_current_time.hour();
             end_minute = end_current_time.minute();
             end_second = end_current_time.second();
             QDateTime end_current_date_time = QDateTime::currentDateTime();
             QString end_current_date = end_current_date_time.toString("yyyy-MM-dd hh:mm:ss");
             ui->showOfflineTime_label->setText(end_current_date);
             setTotalTime();
             //ui->groupBox->setEnabled(false);
             ui->comboBox->setEnabled(true);
         }
         else if(bytes == "")
         {
             ui->showState_label->setText(tr("<font color = red >服务器未响应</font>"));
         }
         else if(bytes == "logout_error")
         {
             ui->showState_label->setText(tr("<font color = red >注销时发生错误</font>"));
         }
         else if(bytes == "uid_error")
         {
             ui->showState_label->setText(tr("<font color = red >你不在线上</font>"));
         }
         else
         {
             ui->showState_label->setText(tr("<font color = red >未知错误</font>"));
         }
     }
}

void Widget::save(QString username, QString password)//保存用户信息
{
    QFile file("inf.dat");
    file.open(QIODevice::WriteOnly | QFile::Truncate);
    QDataStream out(&file);
    out << username;
    out << password;
    file.close();
}

void Widget::read()//读取用户信息
{
    QFile file("inf.dat");
    //qDebug() << file.size();
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> username >> password;
    file.close();
    if(username == " ")
    {
        //qDebug() << "文件内容为空";
        ui->username_lineEdit->setText("");
        ui->password_lineEdit->setText("");
    }
    else
    {
        //qDebug() << "文件内容不空";
        ui->username_lineEdit->setText(username);
        ui->password_lineEdit->setText(password);
    }
}

void Widget::on_logout_pushButton_clicked()//登出按钮
{
    QUrl url;
    QByteArray post_logout;
    //qDebug() << "测试combox" << ui->comboBox->currentIndex();
    if(ui->comboBox->currentIndex() == 1)//因宿舍去和教学区退出机制不一样，分开处理
    {
        url = "http://172.15.0.2/cgi-bin/srun_portal";
        post_logout.append("action=logout");
        //qDebug() << "使用宿舍区登出";
    }
    else if(ui->comboBox->currentIndex() == 2)
    {
        url = "http://210.29.65.198:3333/cgi-bin/force_logout";
        post_logout.append("username=" + username + "&");
        post_logout.append("password=" + password);
        //qDebug() << "使用教学区登陆";
    }
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    reply= manager->post(request, post_logout);
}

void Widget::setTotalTime()//设置上网总时间
{
    qint64 start_total_time = start_hour * 3600 + start_minute * 60 + start_second;
    qint64 end_total_time = end_hour * 3600 + end_minute * 60 + end_second;
    qint64 total_time = end_total_time - start_total_time;
    qint64 total_hour = total_time / 3600;
    qint64 total_minute = (total_time % 3600) / 60;
    qint64 total_second = (total_time % 3600) % 60;
    QString h = QString::number(total_hour);
    QString m = QString::number(total_minute);
    QString s = QString::number(total_second);
    //ui->showTotalTime_label->setText(h + tr("小时 ") + m + tr("分钟 ") +s + tr("秒"));
}

/*void Widget::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    //点击托盘显示窗口
      case QSystemTrayIcon::Trigger:
     {
       showNormal();
       break;
     }
    //双击托盘显示窗口
    case QSystemTrayIcon::DoubleClick:
    {
      showNormal();
      break;
    }
    default:
     break;
    }
}*/

