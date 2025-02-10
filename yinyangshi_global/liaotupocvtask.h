#ifndef LIAOTUPOCVTASK_H
#define LIAOTUPOCVTASK_H

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
class liaotupoopencvTask:public QObject
{
    Q_OBJECT  // 启用元对象系统
public:
    liaotupoopencvTask();

    ~liaotupoopencvTask();

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
    cv::Mat m_xunzhang1Image;
    cv::Mat m_xunzhang2Image;
    cv::Mat m_attackImage;
    cv::Mat m_charactorImage;
    cv::Mat m_charactorImage2;
    cv::Mat m_endImage;
    cv::Mat m_failureImage;
    cv::Mat m_coperImage2;


    bool m_isStop;

    int m_nCount;
};
#endif // LIAOTUPOCVTASK_H
