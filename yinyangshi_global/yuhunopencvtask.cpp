#include "yuhunopencvtask.h"

yuhunopencvTask::yuhunopencvTask()
{
    //加载初始化模板图片
    initImage();
    m_isStop = false;
    m_nCount = 0;
    m_yCount = 200;
    m_cvLib = nullptr;
    m_cvLib = new CommonLib();
}
yuhunopencvTask::~yuhunopencvTask()
{
    if (m_cvLib)
    {
        delete m_cvLib;
        m_cvLib = nullptr;
    }
}

void yuhunopencvTask::initImage()
{
    cv::Mat tempm_coperImage2 = cv::imread("imageFirstTemplate/coperImage2.png");
    cv::Mat tempm_sourceoverImage = cv::imread("imageFirstTemplate/sourceoverImage.png");
    cv::Mat tempm_beginyuhunImage = cv::imread("imageFouthTemplate/beginyuhunImage.png");
    cv::Mat tempm_beginguibingyanwuImage = cv::imread("imageFouthTemplate/beginguibingyanwuImage.png");
    cv::Mat tempm_beginbingcangmijingImage = cv::imread("imageFouthTemplate/beginbingcangmijingImage.png");
    cv::Mat tempm_beginpatahuodongImage = cv::imread("imageFouthTemplate/beginpatahuodongImage.png");
    cv::Mat tempm_bingcangzhufuImage = cv::imread("imageFouthTemplate/zhufuImage.png");
    cv::Mat tempm_bingcangshuxingImage = cv::imread("imageFouthTemplate/shuxingImage.png");
    cv::Mat tempm_bingcangquedingImage = cv::imread("imageFouthTemplate/guibquedingImage.png");
    cv::Mat tempm_yaoqingtickImage = cv::imread("imageFouthTemplate/yaoqingtickImage.png");
    cv::Mat tempm_yaoqingconformImage = cv::imread("imageFouthTemplate/yaoqingconformImage.png");

    cv::Mat tempvictoryImage = cv::imread("imageFirstTemplate/victoryImage.png");
    cv::Mat failureImage = cv::imread("imageSecondTemplate/failureImage.png");
    cv::Mat tempm_endImage = cv::imread("imageFirstTemplate/endImage.png");
    cv::Mat tempm_moneyendImage = cv::imread("imageFouthTemplate/moneyEndImage.png");
    //灰度化
    cv::cvtColor(tempm_coperImage2, m_coperImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_sourceoverImage, m_sourceoverImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_beginyuhunImage, m_beginyuhunImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_beginguibingyanwuImage, m_beginguibingyanwuImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_beginbingcangmijingImage, m_beginbingcangmijingImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_beginpatahuodongImage, m_beginpatahuodongImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_bingcangzhufuImage, m_bingcangzhufuImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_bingcangshuxingImage, m_bingcangshuxingImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_bingcangquedingImage, m_bingcangquedingImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_yaoqingtickImage, m_yaoqingtickImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_yaoqingconformImage, m_yaoqingconformImage, cv::COLOR_BGR2GRAY);


    cv::cvtColor(failureImage, m_failureImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempvictoryImage, m_victoryImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_endImage, m_endImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(tempm_moneyendImage, m_moneyEndImage, cv::COLOR_BGR2GRAY);
}

void yuhunopencvTask::setStopFlag(bool flag)
{
    m_isStop = flag;
}

int yuhunopencvTask::LocateClickMoneyEndPos(cv::Mat &bigImage, cv::Mat &locateImage, double threshold, int ts)
{

    // 确保目标图像和模板图像有相同的通道数
    if (bigImage.channels() != locateImage.channels()) {
        std::cout << "Error: The number of channels in the big image and locate image do not match." << std::endl;
        return false;
    }
    // 创建结果矩阵
    cv::Mat result;
    cv::matchTemplate(bigImage, locateImage, result, cv::TM_CCOEFF_NORMED);

    // 找到最大值和最小值的位置
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    // 判断是否匹配成功
    if (maxVal >= threshold)
    {
        std::cout << "Template matched successfully!" << std::endl;

        // 计算匹配位置的中心点
        int x = maxLoc.x + locateImage.cols / 2;
        int y = maxLoc.y + locateImage.rows * 2;

        // 创建 QPoint 对象表示匹配位置
        QPoint matchPosition(x, y);

        // 输出匹配位置
        std::cout << "Match position: (" << matchPosition.x() << ", " << matchPosition.y() << ")" << std::endl;
        // 如果需要，可以在这里添加模拟鼠标点击等操作
        m_cvLib->clickPos(matchPosition.x(), matchPosition.y());
        m_cvLib->delay(ts);
        return true;
    }

    return false;
}
void yuhunopencvTask::runTask()
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

    m_cvLib->delay(1000);
    while(true)
    {
        if (m_isStop == true)
        {
            qDebug()<<"结束御魂子线程循环";
            break;
        }

        cv::Mat colorCutImage = m_cvLib->captureGameWindow(m_hwnd);
        cv::cvtColor(colorCutImage,m_cutImage,cv::COLOR_BGR2GRAY);

        //开始每轮的匹配
        if (m_cvLib->locateClickPos(m_cutImage, m_coperImage, 0.8, 700))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_sourceoverImage, 0.8, 800))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_beginyuhunImage, 0.8, 4000))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_victoryImage, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }

        if (m_cvLib->locateClickPos(m_cutImage, m_failureImage, 0.8, 700))
        {
            cv::Mat colorCutImage = m_cvLib->captureGameWindow(m_hwnd);
            cv::cvtColor(colorCutImage,m_cutImage,cv::COLOR_BGR2GRAY);
            if (m_cvLib->locateClickPos(m_cutImage, m_yaoqingconformImage, 0.8, 400))
            {
                m_nCount = 0;
                continue;
            }
        }
        if (LocateClickMoneyEndPos(m_cutImage, m_moneyEndImage, 0.8, 700))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_endImage, 0.8, 700))
        {
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_beginguibingyanwuImage, 0.8, 4000))
        {
            m_nCount = 0;
            continue;
        }

        if (m_cvLib->locateClickPos(m_cutImage, m_beginbingcangmijingImage, 0.8, 4000))
        {
            m_nCount = 0;
            continue;
        }

        if (m_cvLib->locateClickPos(m_cutImage, m_beginpatahuodongImage, 0.8, 5000))
        {
            m_nCount = 0;
            continue;
        }

        if (m_cvLib->locateClickPos(m_cutImage, m_bingcangzhufuImage, 0.8, 500) || m_cvLib->locateClickPos(m_cutImage, m_bingcangshuxingImage, 0.8, 500))
        {
            cv::Mat colorCutImage = m_cvLib->captureGameWindow(m_hwnd);
            cv::cvtColor(colorCutImage,m_cutImage,cv::COLOR_BGR2GRAY);
            m_cvLib->locateClickPos(m_cutImage, m_bingcangquedingImage, 0.8, 500);
            m_nCount = 0;
            continue;
        }

        if (m_cvLib->locateClickPos(m_cutImage, m_yaoqingtickImage, 0.8, 400))
        {
            cv::Mat colorCutImage = m_cvLib->captureGameWindow(m_hwnd);
            cv::cvtColor(colorCutImage,m_cutImage,cv::COLOR_BGR2GRAY);
            m_cvLib->locateClickPos(m_cutImage, m_yaoqingconformImage, 0.8, 400);
            m_nCount = 0;
            continue;
        }

        if (m_cvLib->locateClickPos(m_cutImage, m_yaoqingconformImage, 0.8, 400))
        {
            m_nCount = 0;
            continue;
        }



        if (m_nCount >= 500)
            break;

        m_cvLib->delay(600);

    }
}
