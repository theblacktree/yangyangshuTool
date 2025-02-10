#include "commonlib.h"

CommonLib::CommonLib() {}

CommonLib::~CommonLib() {}
cv::Mat CommonLib::captureGameWindow(HWND& hwnd)
{
    // SetForegroundWindow(hwnd);
    cv::Mat matColor;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen)
    {
        qDebug() << "无法获取主屏幕";
        return matColor;
    }
    QEventLoop loop;
    QTimer::singleShot(100,&loop, SLOT(quit()));
    QImage image = (screen->grabWindow(0)).toImage();

    switch (image.format()) {
    case QImage::Format_RGB32:
        matColor = cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
        cv::cvtColor(matColor, matColor, cv::COLOR_RGB2BGR); // 转换为BGR格式
        std::cout<<"Format_RGB32"<<std::endl;
        break;
    case QImage::Format_RGB888:
        matColor = cv::Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
        cv::cvtColor(matColor, matColor, cv::COLOR_RGB2BGR); // 转换为BGR格式
        std::cout<<"Format_RGB888"<<std::endl;
        break;
    case QImage::Format_Indexed8:
        matColor = cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
        std::cout<<"Format_Indexed8"<<std::endl;
        break;
    default:
        qWarning() << "Unsupported QImage format";
    }
    return matColor;
}

bool CommonLib::isFindTemplateImage(cv::Mat &imageTemplate,cv::Mat &cutImage, double threshold)
{
    cv::Mat result;
    cv::matchTemplate(cutImage, imageTemplate, result, cv::TM_CCOEFF_NORMED);

    // 找到最大值
    double maxVal;
    cv::minMaxLoc(result, nullptr, &maxVal);

    // 判断是否匹配成功
    if (maxVal >= threshold)
    {
        std::cout << "Template matched successfully!" << std::endl;
        return true;

    } else
    {
        return false;
    }
}

bool CommonLib::locateClickPos(cv::Mat &bigImage, cv::Mat &locateImage, double threshold, int ts)
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
        int y = maxLoc.y + locateImage.rows / 2;

        // 创建 QPoint 对象表示匹配位置
        QPoint matchPosition(x, y);

        // 输出匹配位置
        std::cout << "Match position: (" << matchPosition.x() << ", " << matchPosition.y() << ")" << std::endl;
        // 如果需要，可以在这里添加模拟鼠标点击等操作
        clickPos(matchPosition.x(), matchPosition.y());
        delay(ts);
        return true;
    }

    return false;
}

void CommonLib::clickPos(int x, int y)
{
    INPUT inputdown = {0};
    inputdown.type = INPUT_MOUSE;
    inputdown.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
    inputdown.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));
    inputdown.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN ;
    SendInput(1, &inputdown, sizeof(INPUT));

    int randomInt = QRandomGenerator::global()->bounded(100,250);
    delay(randomInt);

    INPUT inputup = {0};
    inputup.type = INPUT_MOUSE;
    inputup.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
    inputup.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));
    inputup.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
    SendInput(1, &inputup, sizeof(INPUT));
}

void CommonLib::delay(int ts)
{
    QEventLoop loop;
    QTimer::singleShot(ts, &loop, SLOT(quit()));
    loop.exec();
}
