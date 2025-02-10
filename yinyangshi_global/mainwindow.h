#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include "kun28opencvtask.h"
#include "liaotupocvtask.h"
#include "personaltupocvtask.h"
#include "yuhunopencvtask.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setThisFocus();
public:
    void keyReleaseEvent(QKeyEvent* event)override;
    void closeEvent(QCloseEvent* event)override;

private:
    QPushButton* btnKun28Start;
    QPushButton* btnliaotuStart;
    QPushButton* btnyuhunStart;

    QWidget* centralWidget;
    QVBoxLayout* vLayout;
    QPlainTextEdit* m_textEdit;

    kun28opencvTask* m_kun28CVTask;
    liaotupoopencvTask* m_pliaotuCVTask;
    yuhunopencvTask* m_yuhunCVTask;

    QThread* m_pkun28workerThread;
    QThread* m_pliaotuworkerThread;
    QThread* m_yuhunworkerThread;

private slots:
    void startKun28();
    void startLiaotu();
    void startYuhun();

public slots:
    void handleGlobalKeyRelease(int keyCode) {
        std::cout << "Handling global key release: " << keyCode << std::endl;
        QKeyEvent fakeKeyEvent(QEvent::KeyRelease, keyCode, Qt::NoModifier, QKeySequence(keyCode).toString());
        keyReleaseEvent(&fakeKeyEvent);  // 使用 postEvent 将事件发送到事件队列
    }
};
#endif // MAINWINDOW_H
