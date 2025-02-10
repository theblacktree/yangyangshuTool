#ifndef YUHUNOPENCVTASK_H
#define YUHUNOPENCVTASK_H

#include <QObject>
#include <QWidget>
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
class yuhunopencvTask:public QObject
{
     Q_OBJECT  // 启用元对象系统
public:
    yuhunopencvTask();
    ~yuhunopencvTask();

    void initImage();

    void setStopFlag(bool flag);

    int LocateClickMoneyEndPos(cv::Mat &bigImage, cv::Mat &locateImage, double threshold, int ts);

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
    cv::Mat m_beginyuhunImage;
    cv::Mat m_beginpatahuodongImage;
    cv::Mat m_beginguibingyanwuImage;
    cv::Mat m_beginbingcangmijingImage;
    cv::Mat m_bingcangzhufuImage;
    cv::Mat m_bingcangshuxingImage;
    cv::Mat m_bingcangquedingImage;
    cv::Mat m_yaoqingtickImage;
    cv::Mat m_yaoqingconformImage;

    cv::Mat m_victoryImage;
    cv::Mat m_failureImage;
    cv::Mat m_endImage;
    cv::Mat m_moneyEndImage;

    bool m_isStop;

    int m_nCount;

    int m_yCount;

};

#endif // YUHUNOPENCVTASK_H
