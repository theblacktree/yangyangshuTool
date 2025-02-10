#ifndef KUN28OPENCVTASK_H
#define KUN28OPENCVTASK_H
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <windows.h>
#include <Qscreen>
#include <QWindow>
#include <QPixmap>
#include <QGuiApplication>
#include <QFileDialog>
#include <QThread>
#include <QKeyEvent>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "commonlib.h"
class kun28opencvTask:public QObject
{
     Q_OBJECT  // 启用元对象系统
public:
    kun28opencvTask();

    ~kun28opencvTask();

    void initImage();

    void setStopFlag(bool flag);

public slots:
    void runTask();

private:
    CommonLib* m_cvLib;
private:
    //句柄
    HWND m_hwnd;
    //截图cv格式
    cv::Mat m_cutImage;
    //一级模板图像
    cv::Mat m_coperImage;
    cv::Mat m_sourceoverImage;
    cv::Mat m_Image28;
    cv::Mat m_beginImage;
    cv::Mat m_searchImage;
    cv::Mat m_searchImage2;
    cv::Mat m_victoryImage;
    cv::Mat m_bossImage;
    cv::Mat m_endImage;
    cv::Mat m_boxImage;
    cv::Mat m_boxImage2;
    cv::Mat m_closeImage;

    bool m_isStop;

    int m_nCount;

    int m_yCount;
};

#endif // KUN28OPENCVTASK_H
