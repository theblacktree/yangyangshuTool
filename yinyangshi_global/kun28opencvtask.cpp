#include "kun28opencvtask.h"
#include "personaltupocvtask.h"
kun28opencvTask::kun28opencvTask()
{
    //加载初始化模板图片
    initImage();
    m_isStop = false;
    m_nCount = 0;
    m_yCount = 200;
    m_cvLib = nullptr;
    m_cvLib = new CommonLib();
}

kun28opencvTask::~kun28opencvTask()
{
    if (m_cvLib)
    {
        delete m_cvLib;
        m_cvLib = nullptr;
    }
}
void kun28opencvTask::initImage()
{
    cv::Mat tempm_coperImage2 = cv::imread("imageFirstTemplate/coperImage2.png");
    cv::Mat tempm_sourceoverImage = cv::imread("imageFirstTemplate/sourceoverImage.png");
    cv::Mat tempm_beginImage = cv::imread("imageFirstTemplate/beginImage.png");
    cv::Mat tempsearchImage = cv::imread("imageFirstTemplate/searchImage.png");
    cv::Mat tempsearchImage2 = cv::imread("imageFirstTemplate/searchImage2.png");
    cv::Mat tempvictoryImage = cv::imread("imageFirstTemplate/victoryImage.png");
    cv::Mat tempm_bossImage = cv::imread("imageFirstTemplate/bossImage.png");
    cv::Mat tempm_endImage = cv::imread("imageFirstTemplate/endImage.png");
    cv::Mat tempm_boxImage = cv::imread("imageFirstTemplate/boxImage.png");
    cv::Mat tempm_boxImage2 = cv::imread("imageFirstTemplate/boxImage2.png");
    cv::Mat tempm_Image28 = cv::imread("imageFirstTemplate/Image28.png");
    cv::Mat tempm_closeImage = cv::imread("imageFirstTemplate/closeImage.png");
    //灰度化
    cv::cvtColor(tempm_coperImage2, m_coperImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_sourceoverImage, m_sourceoverImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_beginImage, m_beginImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempsearchImage, m_searchImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempsearchImage2, m_searchImage2, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempvictoryImage, m_victoryImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_endImage, m_endImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_boxImage, m_boxImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_boxImage2, m_boxImage2, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_Image28, m_Image28, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_bossImage, m_bossImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_closeImage, m_closeImage, cv::COLOR_BGR2GRAY);

}

void kun28opencvTask::setStopFlag(bool flag)
{
    m_isStop = flag;
}
void kun28opencvTask::runTask()
{
    QString windowTitle = "MuMu模拟器12";
    HWND h = FindWindow(nullptr, windowTitle.toStdWString().c_str());
    if (h == nullptr)
    {

        windowTitle = "MuMu模拟器13";
        h = FindWindow(nullptr, windowTitle.toStdWString().c_str());
        if (h == nullptr)
            qWarning("无法找到游戏窗口：%s", qPrintable(windowTitle));
    }
    this->m_hwnd = h;
    SetForegroundWindow(m_hwnd);

    // 获取屏幕中心位置
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    m_cvLib->delay(1000);
    while(true)
    {
        if (m_isStop == true)
        {
            qDebug()<<"结束绘卷子线程循环";
            break;
        }

        cv::Mat colorCutImage = m_cvLib->captureGameWindow(m_hwnd);
        cv::cvtColor(colorCutImage,m_cutImage,cv::COLOR_BGR2GRAY);

        //开始每轮的匹配
        if (m_cvLib->locateClickPos(m_cutImage, m_coperImage, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }
       if (m_yCount >= 100)
        {
            if (m_cvLib->locateClickPos(m_cutImage, m_closeImage, 0.8, 3000))
            {
                personaltupocvtask ptupo;
                ptupo.runTask();
                m_yCount = 0;
                continue;
            }
        }

        if (m_cvLib->locateClickPos(m_cutImage, m_bossImage, 0.8, 7800))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_searchImage, 0.75, 7800) || \
            m_cvLib->locateClickPos(m_cutImage, m_searchImage2, 0.75, 7800))
        {
            m_nCount = 0;
            ++m_yCount;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_victoryImage, 0.8, 2500))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_endImage, 0.8, 700))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_boxImage, 0.8, 200))
        {
            m_cvLib->clickPos(centerX, centerY+500);
            m_cvLib->delay(1000);
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_boxImage2, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_sourceoverImage, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_beginImage, 0.87, 1000))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_Image28, 0.8, 800))
        {
            m_nCount = 0;
            continue;
        }

        m_cvLib->delay(700);
        //拖拽往左滑界面

        // 移动鼠标到屏幕中心
        SetCursorPos(centerX, centerY);

        // 模拟鼠标左键按下
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);

        // 向左移动200像素
        int newX = centerX - 800;
        // 模拟鼠标移动到目标位置
        for (int x = centerX; x > newX; x -= 20) {  // 每次移动20像素
            SetCursorPos(x, centerY);
            Sleep(20);  // 短暂延迟，确保移动平滑
        }

        // 模拟鼠标左键释放
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        //否则，我需要暂停几秒等待程序循环慢点
        m_cvLib->delay(100);
        ++m_nCount;
        std::cout<<"未匹配到任何目标次数"<<m_nCount;
        if (m_nCount >= 100)
            m_isStop = true;

    }
}

