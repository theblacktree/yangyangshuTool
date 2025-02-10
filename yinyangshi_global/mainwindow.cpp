#include "mainwindow.h"

// 全局变量，用于存储钩子句柄
HHOOK g_hHook = NULL;
// 静态指针，指向 MainWindow 实例
static MainWindow *mainWindowInstance = nullptr;
// 全局键盘钩子回调函数
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {
        return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }

    KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;
    int keyCode = pKeyboard->vkCode;

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
    {
        std::cout << "Global key released: " << keyCode << std::endl;
        // 发送信号给 MainWindow
        if (mainWindowInstance)
            QMetaObject::invokeMethod(mainWindowInstance, "handleGlobalKeyRelease", Qt::QueuedConnection, Q_ARG(int, keyCode));
        else
            std::cout << "MainWindow not found" << std::endl;
    }

    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    centralWidget = nullptr;
    vLayout = nullptr;
    btnKun28Start = nullptr;
    m_kun28CVTask = nullptr;
    m_pkun28workerThread = nullptr;
    m_pliaotuCVTask = nullptr;
    m_pliaotuworkerThread = nullptr;
    m_yuhunCVTask = nullptr;
    m_yuhunworkerThread = nullptr;

    centralWidget = new QWidget(this);
    vLayout = new QVBoxLayout(centralWidget);
    btnKun28Start = new QPushButton(centralWidget);
    btnKun28Start->setText("绘卷/结界突破启动");
    btnKun28Start->setMinimumSize(100,40);
    btnKun28Start->setCheckable(true);

    btnliaotuStart = new QPushButton(centralWidget);
    btnliaotuStart->setText("寮突破启动");
    btnliaotuStart->setMinimumSize(100,40);
    btnliaotuStart->setCheckable(true);

    btnyuhunStart = new QPushButton(centralWidget);
    btnyuhunStart->setText("御魂等活动启动");
    btnyuhunStart->setMinimumSize(100,40);
    btnyuhunStart->setCheckable(true);



    m_textEdit = new QPlainTextEdit(centralWidget);
    m_textEdit->setPlainText("使用说明：\n将游戏窗口全屏，角色移动到困28那里\n点击按钮开始任务\nQ键退出\n本\
软件由副会长七夕制作，闲鱼号：河边护不湿郎，禁止其他人售卖，遇到请举报！正版享受永久更新");

    vLayout->addWidget(btnKun28Start);
    vLayout->setAlignment(btnKun28Start,Qt::AlignCenter);
    vLayout->addWidget(btnliaotuStart);
    vLayout->setAlignment(btnliaotuStart,Qt::AlignCenter);
    vLayout->addWidget(btnyuhunStart);
    vLayout->setAlignment(btnyuhunStart,Qt::AlignCenter);

    vLayout->addWidget(m_textEdit);
    vLayout->setAlignment(m_textEdit,Qt::AlignCenter);

    centralWidget->setLayout(vLayout);
    this->setCentralWidget(centralWidget);
    this->resize(800, 600);

    // 设置焦点策略
    setFocusPolicy(Qt::StrongFocus);
    setFocus();  // 设置焦点到主窗口

    // 激活窗口
    activateWindow();

    btnKun28Start->setEnabled(true);
    btnliaotuStart->setEnabled(true);
    btnyuhunStart->setEnabled(true);

    connect(btnKun28Start, &QPushButton::clicked, this, &MainWindow::startKun28);
    connect(btnliaotuStart, &QPushButton::clicked, this, &MainWindow::startLiaotu);
    connect(btnyuhunStart, &QPushButton::clicked, this, &MainWindow::startYuhun);

    // 安装全局键盘钩子
    g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, nullptr, 0);
    if (g_hHook == nullptr) {
        std::cerr << "Failed to install global keyboard hook" << std::endl;
    }
    // 保存 MainWindow 实例指针q
    mainWindowInstance = this;
    setWindowIcon(QIcon("title.png"));
}

MainWindow::~MainWindow()
{
    // 卸载全局键盘钩子
    if (g_hHook != nullptr)
    {
        UnhookWindowsHookEx(g_hHook);
    }
}

void MainWindow::setThisFocus()
{
    setFocus();  // 设置焦点到主窗口
    // 激活窗口
    activateWindow();
    qDebug()<<"信号触发，设置窗口焦点";
}

void MainWindow::startKun28()
{
    btnKun28Start->setChecked(true);
    if (m_kun28CVTask !=nullptr)
    {
        m_kun28CVTask->setStopFlag(true);
        delete m_kun28CVTask;
        m_kun28CVTask = nullptr;

    }
    if (m_pkun28workerThread != nullptr)
    {
        delete m_pkun28workerThread;
        m_pkun28workerThread = nullptr;
    }
    m_kun28CVTask = new kun28opencvTask();
    m_pkun28workerThread = new QThread();
    m_kun28CVTask->moveToThread(m_pkun28workerThread);
    connect(m_pkun28workerThread, &QThread::started, m_kun28CVTask, &kun28opencvTask::runTask);
 //   connect(m_pkun28workerThread, &QThread::finished, m_kun28CVTask, &QObject::deleteLater);
    m_pkun28workerThread->start();

}

void MainWindow::startLiaotu()
{
    btnliaotuStart->setChecked(true);
    if (m_pliaotuCVTask !=nullptr)
    {
        m_pliaotuCVTask->setStopFlag(true);
        delete m_pliaotuCVTask;
        m_pliaotuCVTask = nullptr;

    }
    if (m_pliaotuworkerThread != nullptr)
    {
        delete m_pliaotuworkerThread;
        m_pliaotuworkerThread = nullptr;
    }
    m_pliaotuCVTask = new liaotupoopencvTask();
    m_pliaotuworkerThread = new QThread();
    m_pliaotuCVTask->moveToThread(m_pliaotuworkerThread);
    connect(m_pliaotuworkerThread, &QThread::started, m_pliaotuCVTask, &liaotupoopencvTask::runTask);
    m_pliaotuworkerThread->start();
}

void MainWindow::startYuhun()
{
    btnyuhunStart->setChecked(true);
    if (m_yuhunCVTask !=nullptr)
    {
        m_yuhunCVTask->setStopFlag(true);
        delete m_yuhunCVTask;
        m_yuhunCVTask = nullptr;

    }
    if (m_yuhunworkerThread != nullptr)
    {
        delete m_yuhunworkerThread;
        m_yuhunworkerThread = nullptr;
    }
    m_yuhunCVTask = new yuhunopencvTask();
    m_yuhunworkerThread = new QThread();
    m_yuhunCVTask->moveToThread(m_yuhunworkerThread);
    connect(m_yuhunworkerThread, &QThread::started, m_yuhunCVTask, &yuhunopencvTask::runTask);
    m_yuhunworkerThread->start();

}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    qDebug()<<"Q按下，正在执行关闭工作线程，请稍后......";
    if (event->key() == Qt::Key_Q)
    {



        btnKun28Start->setChecked(false);
        if (m_kun28CVTask !=nullptr)
        {
            personaltupocvtask::setStopFlag(true);
            m_kun28CVTask->setStopFlag(true);
        }

        if (m_pkun28workerThread != nullptr)
        {
            m_pkun28workerThread->quit();
            m_pkun28workerThread->wait();
            qDebug()<<"已关闭绘卷工作线程,关闭完成！";
        }
        btnliaotuStart->setChecked(false);
        if (m_pliaotuCVTask !=nullptr)
        {
            m_pliaotuCVTask->setStopFlag(true);
        }

        if (m_pliaotuworkerThread != nullptr)
        {
            m_pliaotuworkerThread->quit();
            m_pliaotuworkerThread->wait();
            qDebug()<<"已关闭寮突工作线程";
        }
        btnyuhunStart->setChecked(false);
        if (m_yuhunCVTask !=nullptr)
        {
            m_yuhunCVTask->setStopFlag(true);
        }
        if (m_yuhunworkerThread != nullptr)
        {
            m_yuhunworkerThread->quit();
            m_yuhunworkerThread->wait();
            qDebug()<<"已关闭御魂工作线程";
        }


    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{

    if (m_kun28CVTask !=nullptr)
    {
        m_kun28CVTask->setStopFlag(true);
    }

    if (m_pkun28workerThread != nullptr)
    {
        connect(m_yuhunworkerThread, &QThread::finished, m_yuhunCVTask, &QObject::deleteLater);
        m_pkun28workerThread->exit();
        m_pkun28workerThread->wait();
        qDebug()<<"已关闭工作线程";
    }

    if (m_pliaotuCVTask !=nullptr)
    {
        m_pliaotuCVTask->setStopFlag(true);
    }

    if (m_pliaotuworkerThread != nullptr)
    {
        connect(m_pliaotuworkerThread, &QThread::finished, m_pliaotuCVTask, &QObject::deleteLater);
        m_pliaotuworkerThread->exit();
        m_pliaotuworkerThread->wait();
        qDebug()<<"已关闭工作线程";
    }

    if (m_yuhunCVTask !=nullptr)
    {
        m_yuhunCVTask->setStopFlag(true);
    }

    if (m_yuhunworkerThread != nullptr)
    {
        connect(m_yuhunworkerThread, &QThread::finished, m_yuhunCVTask, &QObject::deleteLater);
        m_yuhunworkerThread->exit();
        m_yuhunworkerThread->wait();
        qDebug()<<"已关闭御魂工作线程";
    }
}
