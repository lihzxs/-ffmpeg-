#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QString>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //使用ffmpeg对mkv，mov，ts，flv，avi，wmv格式转换为mp4
    ui->setupUi(this);
    this->setFixedSize(700,520);
    this->setWindowTitle("视频转换MP4");
    // 加载背景图片
    QPixmap backgroundImage("F:/QTnew/Project/VideoConversion/image.png");
    // 设置窗口背景图片
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    this->setPalette(palette);

    QPushButton *xuanzewenjian = new QPushButton("选择文件",this);

    xuanzewenjian->setStyleSheet("QPushButton {"
                                 "    color: #e43c2f;"           // 文本颜色为白色
                                 "    background-color: #0ac521;" // 初始背景颜色为pink
                                 "    border-radius: 10px;"     // 圆角半径为10px
                                 "    font-family:微软雅黑;"
                                 "    font-size:18px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: #ff9800;" // 鼠标悬停时背景颜色为浅蓝色
                                 "}");
    xuanzewenjian->resize(100,30);
    xuanzewenjian->setGeometry(300,300,100,50);

    // 连接按钮的点击事件到Lambda表达式
    connect(xuanzewenjian, &QPushButton::clicked, [=]() {
        // 创建一个文件选择对话框
        QFileDialog fileDialog(this);
        fileDialog.setNameFilter("Video Files (*.mkv *.avi *.ts *.mov *.flv *.wmv)"); // 设置过滤器，只显示指定格式的视频文件
        fileDialog.setFileMode(QFileDialog::ExistingFile);

        // 显示对话框
        if (fileDialog.exec())
        {
            // 获取用户选择的文件路径
            QString selectedFile = fileDialog.selectedFiles().at(0);

            // 获取原始文件名（不包含扩展名）和原始文件的目录
            QFileInfo fileInfo(selectedFile);
            QString fileDir = fileInfo.path();

            qDebug()<<fileInfo;   //带括号
            qDebug()<<fileDir;    //转换的文件夹路径，和原来输入视频的一样
            qDebug() << selectedFile;  //输入视频路径

            //转换命令
            QString conversion = "ffmpeg -i "+selectedFile+" -c copy "+fileDir+"/"+suijizifuchuan()+".mp4";

            // 调用FFmpeg命令
            QProcess process;
            process.start(conversion);
            process.waitForFinished();  // 等待进程完成

            // 检查FFmpeg执行结果
            if (process.exitCode() == 0) {
                qDebug() << "转换成功!";
                QMessageBox::information(this,"转换成功","视频已成功转换!");
            } else {
                qDebug() << "转换失败，错误代码：" << process.exitCode();
                qDebug() << "错误信息：" << process.errorString();
                QMessageBox::warning(this,"转换失败","视频格式转化失败");
            }
        }
    });
}
//随机生成长度为5的字符串用来命名输出视频的名字
QString MainWindow::suijizifuchuan(){
    QString str="";
    // 设置随机数种子
    qsrand(QTime::currentTime().msec());
    QString st = "abcdefghijklmnopqrstuvwxyz";
    qDebug()<<"长度为:"<<st.size();
    // 生成随机数
    for (int i = 0; i < 5; ++i) {
        int randomValue = qrand()%26; // 生成随机数
        qDebug() << "随机数:" << randomValue;
        str+=st[randomValue];
    }
    qDebug()<<"新的视频名称叫："<<str;
    return str;
}
MainWindow::~MainWindow()
{
    delete ui;
}
