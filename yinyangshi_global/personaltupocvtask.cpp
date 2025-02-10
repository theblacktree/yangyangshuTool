#include "personaltupocvtask.h"
bool personaltupocvtask::m_ispersonalliaotuStop = false;
personaltupocvtask::personaltupocvtask()
{
    //加载初始化模板图片
    initImage();
    m_nCount = 0;
    m_cvLib = nullptr;
    m_cvLib = new CommonLib();
}
personaltupocvtask::~personaltupocvtask()
{
    if (m_cvLib)
    {
        delete m_cvLib;
        m_cvLib = nullptr;
    }
}

void personaltupocvtask::initImage()
{
    cv::Mat xunzhang1Image = cv::imread("imageThirdTemplate/tupoxunzhang1.png");
    cv::Mat xunzhang2Image = cv::imread("imageSecondTemplate/xunzhang2Image.png");
    cv::Mat attackImage = cv::imread("imageSecondTemplate/attackImage.png");
    cv::Mat charactorImage = cv::imread("imageSecondTemplate/charactorImage.png");
    cv::Mat charactorImage2 = cv::imread("imageSecondTemplate/charactorImage2.png");
    cv::Mat endImage = cv::imread("imageSecondTemplate/endImage.png");
    cv::Mat failureImage = cv::imread("imageSecondTemplate/failureImage.png");
    cv::Mat coperImage2 = cv::imread("imageSecondTemplate/coperImage2.png");
    cv::Mat poImage = cv::imread("imageThirdTemplate/po.png");
    cv::Mat returnImage = cv::imread("imageThirdTemplate/return.png");
    cv::Mat returnConformImage = cv::imread("imageThirdTemplate/returnConform.png");
    cv::Mat entryImage = cv::imread("imageThirdTemplate/entry.png");
    cv::Mat outImage = cv::imread("imageThirdTemplate/out.png");
    cv::Mat finalOutImage = cv::imread("imageFirstTemplate/closeImage.png");

    //灰度化
    cv::cvtColor(xunzhang1Image, m_xunzhang1Image, cv::COLOR_BGR2GRAY);
    cv::cvtColor(xunzhang2Image, m_xunzhang2Image, cv::COLOR_BGR2GRAY);
    cv::cvtColor(attackImage, m_attackImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(charactorImage, m_charactorImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(charactorImage2, m_charactorImage2, cv::COLOR_BGR2GRAY);
    cv::cvtColor(endImage, m_endImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(failureImage, m_failureImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(coperImage2, m_coperImage2, cv::COLOR_BGR2GRAY);
    cv::cvtColor(poImage, m_poImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(returnImage, m_returnImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(returnConformImage, m_returnConformImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(entryImage, m_entryImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(outImage, m_outImage, cv::COLOR_BGR2GRAY);
    cv::cvtColor(finalOutImage, m_finalOutImage, cv::COLOR_BGR2GRAY);
}

void personaltupocvtask::setStopFlag(bool flag)
{
    m_ispersonalliaotuStop = flag;
}

int personaltupocvtask::countDefeatNumber()
{
    // 创建结果矩阵
    cv::Mat result;
    int result_cols = m_cutImage.cols - m_poImage.cols + 1;
    int result_rows = m_cutImage.rows - m_poImage.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    // 执行模板匹配
    cv::matchTemplate(m_cutImage, m_poImage, result, cv::TM_CCOEFF_NORMED);

    // 设定阈值
    double threshold = 0.8;  // 根据实际情况调整此值

    // 查找大于阈值的位置
    cv::threshold(result, result, threshold, 1., cv::THRESH_BINARY);

    // 使用 minMaxLoc 方法查找所有局部最大值
    int size = m_defeatPos.size();
    for (int i = 0; i <size; ++i)
    {
        m_defeatPos.pop_back();
    }
        // 清空并释放内存
    while(true) {
        double minVal;
        double maxVal;
        cv::Point minLoc;
        cv::Point maxLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

        if(maxVal >= threshold) {
            m_defeatPos.emplace_back(maxLoc);

            // 将找到的最大值区域设为非常低的值，以寻找下一个匹配
            cv::rectangle(result, cv::Point(maxLoc.x-4, maxLoc.y -4), cv::Point(maxLoc.x + m_poImage.cols+4, maxLoc.y + m_poImage.rows+4), cv::Scalar(0), cv::FILLED);
        } else {
            break;
        }
    }

    // 输出匹配次数
    int matchCount = m_defeatPos.size();
    std::cout<<"攻破次数是 "<<matchCount<<std::endl;
    return matchCount;
}

int personaltupocvtask::thisClassLocateClickPos(cv::Mat &bigImage, cv::Mat &locateImage, double threshold, int ts)
{

    // 确保目标图像和模板图像有相同的通道数
    if (bigImage.channels() != locateImage.channels()) {
        std::cout << "Error: The number of channels in the big image and locate image do not match." << std::endl;
        return false;
    }
    // 创建结果矩阵
    cv::Mat result;
    cv::matchTemplate(bigImage, locateImage, result, cv::TM_CCOEFF_NORMED);

    //将已经击败的对手区域置信度置为0
    for (auto& point : m_defeatPos)
    {
        // 将找到的击破点区域设为非常低的值，
        cv::rectangle(result, cv::Point(point.x - 4 * m_poImage.cols - 4, point.y -4 ), cv::Point(point.x + m_poImage.cols + 4 , point.y + 2 * m_poImage.rows + 4), cv::Scalar(0), cv::FILLED);
    }
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
        int y = maxLoc.y + locateImage.rows / 2;

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
void personaltupocvtask::runTask()
{
    QString windowTitle = "MuMu模拟器12";
    HWND h = FindWindow(nullptr, windowTitle.toStdWString().c_str());
    if (h == nullptr)
    {
        qWarning("无法找到游戏窗口：%s", qPrintable(windowTitle));
    }
    this->m_hwnd = h;
    SetForegroundWindow(m_hwnd);
    bool isNeedClickCharacter = false;
    while(true)
    {
        if (m_ispersonalliaotuStop == true)
        {
            qDebug()<<"结束结界突破子线程循环";
            break;
        }

        cv::Mat cutImage = m_cvLib->captureGameWindow(m_hwnd);
        cv::cvtColor(cutImage,this->m_cutImage,cv::COLOR_BGR2GRAY);

        //开始每轮的匹配
        if (m_cvLib->locateClickPos(m_cutImage, m_entryImage, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }

        if (countDefeatNumber() >= 8)
        {
            int count = 0;
            while (count <= 4)
            {
                cv::Mat cutImage = m_cvLib->captureGameWindow(m_hwnd);
                cv::cvtColor(cutImage,this->m_cutImage,cv::COLOR_BGR2GRAY);
                if (m_cvLib->locateClickPos(m_cutImage, m_attackImage, 0.8, 300))
                {
                    //退出代码
                    cv::Mat cutImage = m_cvLib->captureGameWindow(m_hwnd);
                    cv::Mat cutGrayImage;
                    cv::cvtColor(cutImage,cutGrayImage,cv::COLOR_BGR2GRAY);
                    if (m_cvLib->locateClickPos(cutGrayImage, m_outImage, 0.8, 600))
                    {
                        //退出代码
                        if (m_cvLib->locateClickPos(cutGrayImage, m_finalOutImage, 0.8, 1000))
                            return;
                    }
                    m_cvLib->delay(1000);
                    continue;
                }
                if (thisClassLocateClickPos(m_cutImage, m_xunzhang2Image, 0.8, 1000)|| \
                    thisClassLocateClickPos(m_cutImage, m_xunzhang1Image, 0.8, 1000))
                {
                    ++count;
                    continue;
                }
                m_cvLib->locateClickPos(m_cutImage, m_returnConformImage, 0.8, 4000);
                m_cvLib->locateClickPos(m_cutImage, m_returnImage, 0.8, 800);                
                m_cvLib->locateClickPos(m_cutImage, m_failureImage, 0.8, 1000);
                m_cvLib->locateClickPos(m_cutImage, m_endImage, 0.8, 1000);
            }
            m_cvLib->delay(500);
            cv::Mat cutImage = m_cvLib->captureGameWindow(m_hwnd);
            cv::cvtColor(cutImage,this->m_cutImage,cv::COLOR_BGR2GRAY);
            if (m_cvLib->locateClickPos(m_cutImage, m_attackImage, 0.8, 1300))
            {
                isNeedClickCharacter = true;
                continue;
            }
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_attackImage, 0.8, 500))
        {
            //退出代码
            cv::Mat cutImage = m_cvLib->captureGameWindow(m_hwnd);
            cv::cvtColor(cutImage,this->m_cutImage,cv::COLOR_BGR2GRAY);
            if (m_cvLib->locateClickPos(m_cutImage, m_outImage, 0.8, 600))
            {
                //退出代码
                if (m_cvLib->locateClickPos(m_cutImage, m_finalOutImage, 0.8, 1000))
                    return;
            }
           // m_cvLib->delay(3500);
            m_nCount = 0;
            isNeedClickCharacter = true;
            continue;
        }
        if (isNeedClickCharacter && m_cvLib->locateClickPos(m_cutImage, m_charactorImage, 0.8, 4000))
        {
            isNeedClickCharacter = false;
            m_nCount = 0;
            continue;
        }
        if (m_cvLib->locateClickPos(m_cutImage, m_endImage, 0.8, 1000))
        {
            m_nCount = 0;
            continue;
        }
        if (thisClassLocateClickPos(m_cutImage, m_xunzhang2Image, 0.7, 1000) || \
            thisClassLocateClickPos(m_cutImage, m_xunzhang1Image, 0.7, 1000))
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
        m_cvLib->delay(100);
        ++m_nCount;
        qDebug()<<"未匹配到模板的次数是："<<m_nCount;
        if (m_nCount >= 1000)
            m_ispersonalliaotuStop = true;
    }
}
