#ifndef COMMONLIB_H
#define COMMONLIB_H

#include "CommonLib_global.h"
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <QApplication>
#include <QScreen>
#include <QCursor>
#include <QPoint>
#include <QImage>
#include <QPixmap>
#include <windows.h>
#include <QTimer>
#include <windows.h>
#include <QKeyEvent>
#include <QRandomGenerator>

class COMMONLIB_EXPORT CommonLib
{
public:
    CommonLib();
    ~CommonLib();
    //这个函数根据句柄截图游戏屏幕并转换成Mat形式
    cv::Mat captureGameWindow(HWND& hwnd);
    //两个Mat是否能匹配到
    bool isFindTemplateImage(cv::Mat &imageTemplate,cv::Mat &cutImage, double threshold);
    //匹配两个Mat，匹配到就会点击并延时
    bool locateClickPos(cv::Mat &bigImage, cv::Mat &locateImage, double threshold, int ts);
    //点击指定像素位置
    void clickPos(int x, int y);

    void delay(int ts);
};

#endif // COMMONLIB_H
