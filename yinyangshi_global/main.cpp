#include "mainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <QNetworkInterface>
QString getUniqueMachineID()
{
    foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces())
    {
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack) && netInterface.flags() & QNetworkInterface::IsUp)
            return netInterface.hardwareAddress().toLower().remove(':');
    }
    return QString("defaultID");
}

bool checkTrialPeriod()
{
    QString machineID = getUniqueMachineID(); // 获取机器唯一标识符
    QSettings settings("MyCompany", "MyApp"); // 使用系统范围的存储位置

    // 构建基于机器ID的关键字
    QString key = "firstRunDate_" + machineID;

    if(settings.contains(key))
    {
        QDate firstRunDate = settings.value(key).toDate();
        QDate currentDate = QDate::currentDate();
        int daysUsed = firstRunDate.daysTo(currentDate);
        const int TRIAL_PERIOD_DAYS = 20; // 设置试用期为365天
        if(daysUsed > TRIAL_PERIOD_DAYS)
        {
            QMessageBox::information(nullptr, "试用期结束", "您的试用期已结束，请购买完整版本。");
            return false;
        }
    }
    else
    {
        // 如果是第一次运行，记录当前日期
        settings.setValue(key, QDate::currentDate());
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!checkTrialPeriod())
    {
        return -1; // 如果试用期结束，退出程序
    }
    MainWindow w;
    w.show();
    return a.exec();
}
