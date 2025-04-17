#pragma execution_character_set("utf-8")

#include "WBalance.h"
#include <QtWidgets/QApplication>
#include <QSplashScreen>
#include <qtimer.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //��������ͼƬ
    QString imagePath = QCoreApplication::applicationDirPath() + "/MCM_welcome.jpg";
    QPixmap pixmap(imagePath); 
    QSize targetSize(1317, 785);
    QPixmap scaledPixmap = pixmap.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    
    //��ʾ��������
    QSplashScreen splash(scaledPixmap);
    
    QFont font;
    font.setPointSize(12);  //���������С
    splash.setFont(font);
    
    splash.show();
    splash.showMessage(("���ڳ�ʼ��..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    //�����¼���ȷ��������ʾ
    a.processEvents();

    WBalance w;

    //��ƽ̨��ʱ�����������Ƽ���
    QTimer::singleShot(2000, [&]() {
        splash.close();      //�رջ�ӭ����
        w.show();            //��ʾ������
        });
    return a.exec();
}
