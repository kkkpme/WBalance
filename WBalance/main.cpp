#pragma execution_character_set("utf-8")

#include "WBalance.h"
#include <QtWidgets/QApplication>
#include <QSplashScreen>
#include <qtimer.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //加载启动图片
    QString imagePath = QCoreApplication::applicationDirPath() + "/MCM_welcome.jpg";
    QPixmap pixmap(imagePath); 
    QSize targetSize(1317, 785);
    QPixmap scaledPixmap = pixmap.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    
    //显示启动画面
    QSplashScreen splash(scaledPixmap);
    
    QFont font;
    font.setPointSize(12);  //设置字体大小
    splash.setFont(font);
    
    splash.show();
    splash.showMessage(("正在初始化..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    //处理事件以确保画面显示
    a.processEvents();

    WBalance w;

    //跨平台延时（非阻塞，推荐）
    QTimer::singleShot(2000, [&]() {
        splash.close();      //关闭欢迎界面
        w.show();            //显示主窗口
        });
    return a.exec();
}
