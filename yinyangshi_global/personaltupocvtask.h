#ifndef PERSONALTUPOCVTASK_H
#define PERSONALTUPOCVTASK_H

#include <QObject>
#include <QWidget>
#include "liaotupocvtask.h"

class personaltupocvtask: public QObject
{
    Q_OBJECT
public:
    personaltupocvtask();
    ~personaltupocvtask();

    void initImage();

    static void setStopFlag(bool flag);

    int countDefeatNumber();
    int thisClassLocateClickPos(cv::Mat &bigImage, cv::Mat &locateImage, double threshold, int ts);
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
    cv::Mat m_poImage;
    cv::Mat m_returnImage;
    cv::Mat m_returnConformImage;
    cv::Mat m_entryImage;
    cv::Mat m_outImage;
    cv::Mat m_finalOutImage;
    static bool m_ispersonalliaotuStop;

    int m_nCount;

    std::vector<cv::Point>m_defeatPos;
};

#endif // PERSONALTUPOCVTASK_H
