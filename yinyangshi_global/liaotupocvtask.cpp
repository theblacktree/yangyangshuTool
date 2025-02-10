#include "liaotupocvtask.h"

liaotupoopencvTask::liaotupoopencvTask()
{
    //加载初始化模板图片
    initImage();
    m_isStop = false;
    m_nCount = 0;
    m_cvLib = nullptr;
    m_cvLib = new CommonLib();
}

liaotupoopencvTask::~liaotupoopencvTask()
{
    if (m_cvLib)
    {
        delete m_cvLib;
        m_cvLib = nullptr;
    }
}
void liaotupoopencvTask::initImage()
{
    cv::Mat xunzhang1Image = cv::imread("imageSecondTemplate/xunzhang1Image.png");
    cv::Mat xunzhang2Image = cv::imread("imageSecondTemplate/xunzhang2Image.png");
    cv::Mat attackImage = cv::imread("imageSecondTemplate/attackImage.png");
    cv::Mat charactorImage = cv::imread("imageSecondTemplate/charactorImage.png");
    cv::Mat charactorImage2 = cv::imread("imageSecondTemplate/charactorImage2.png");
    cv::Mat endImage = cv::imread("imageSecondTemplate/endImage.png");
    cv::Mat failureImage = cv::imread("imageSecondTemplate/failureImage.png");
    cv::Mat coperImage2 = cv::imread("imageSecondTemplate/coperImage2.png");



    //灰度化
    cv::cvtColor(xunzhang1Image, m_xunzhang1Image, cv::COLOR_BGR2GRAY);
    cv::cvtColor(xunzhang2Image, m_xunzhang2Image, cv::COLOR_BGR2GRAY);
    cv::cvtColor(attackImage, m_attackImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(charactorImage, m_charactorImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(charactorImage2, m_charactorImage2, cv::COLOR_BGR2GRAY);
    cv::cvtColor(endImage, m_endImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(failureImage, m_failureImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(coperImage2, m_coperImage2, cv::COLOR_BGR2GRAY);
}

void liaotupoopencvTask::setStopFlag(bool flag)
{
    m_isStop = flag;
}
void liaotupoopencvTask::runTask()
{
    QString windowTitle = "MuMu模拟器12";
    HWND h = FindWindow(nullptr, windowTitle.toStdWString().c_str());
    if (h == nullptr)
    {
        qWarning("无法找到游戏窗口：%s", qPrintable(windowTitle));
    }
    this->m_hwnd = h;
    SetForegroundWindow(m_hwnd);
    while(true)
    {
        if (m_isStop == true)
        {
            qDebug()<<"结束寮突破子线程循环";
            break;
        }

        cv::Mat colorCutImage = m_cvLib->captureGameWindow(m_hwnd);
        cv::cvtColor(colorCutImage,this->m_cutImage,cv::COLOR_BGR2GRAY);

        //开始每轮的匹配
        if (m_cvLib->locateClickPos(m_cutImage, m_coperImage2, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_attackImage, 0.8, 4000))
        {
            //m_cvLib->locateClickPos(m_cutImage, m_charactorImage, 0.6, 1000);
           // m_cvLib->locateClickPos(m_cutImage, m_charactorImage2, 0.6, 1000);
           // m_cvLib->locateClickPos(m_cutImage, m_charactorImage, 0.6, 1000);
            m_nCount = 0;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_charactorImage, 0.6, 10000))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_xunzhang2Image, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_xunzhang1Image, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_endImage, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }

        if (m_cvLib->locateClickPos(m_cutImage, m_failureImage, 0.8, 1500))
        {
            m_nCount = 0;
            continue;
        }

        //否则，我需要暂停几秒等待程序循环慢点
        m_cvLib->delay(1000);
        ++m_nCount;
        qDebug()<<m_nCount;
        if (m_nCount >= 100)
            m_isStop = true;

    }
}


