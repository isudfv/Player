#include "cef_app.h"
#include "cefwindow.h"
//#include "form.h"
#include <QApplication>
//#include <QDateTime>
//#include <QGuiApplication>
#include <QLabel>
//#include <QLineEdit>
//#include <QPushButton>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <QQuickItem>
#include <QQuickWidget>
#include <QTimer>

int main(int argc, char *argv[])
{
#ifdef __linux__
    CefMainArgs main_args(argc, argv);
#endif
#ifdef _WINDOWS
    HINSTANCE   h = GetModuleHandle(nullptr);
    CefMainArgs main_args(h);
#endif

    // CEF applications have multiple sub-processes (render, GPU, etc) that share
    // the same executable. This function checks the command-line and, if this is
    // a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(main_args, nullptr, nullptr);
    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        return exit_code;
    }

    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();

#ifdef __linux__
    command_line->InitFromArgv(argc, argv);
#endif
#ifdef _WINDOWS
#endif

    // Specify CEF global settings here.
    CefSettings settings;

    CefString(&settings.locale).FromString("zh-CN");
    CefString(&settings.accept_language_list).FromString("zh-CN");

    CefRefPtr<CefApp> app;

    CefInitialize(main_args, settings, app.get(), nullptr);


    QApplication a(argc, argv);

    qmlRegisterType<CEFWindow>("com.fuck.test", 1, 0, "CEFWindow");
    QQmlApplicationEngine engine("qrc:/main.qml");

    auto cefWindow = new CEFWindow("https://www.bilibili.com/video/BV13U4y1N7Uo/?spm_id_from=333.788");

    QObject *QmlObj    = engine.rootObjects().first();
    QWindow *QmlWindow = qobject_cast<QWindow *>(QmlObj);


    engine.rootContext()->setContextProperty("cefWindow", cefWindow);

    //    cefWindow->winId();
    cefWindow->setParent(QmlWindow);
    cefWindow->show();
    //
    cefWindow->setGeometry(0, 111, QmlWindow->width(), QmlWindow->height() - 111);

    CefRunMessageLoop();

    CefShutdown();

    //    auto   testWidget = new QWidget;
    //    QTimer timer;
    //    QObject::connect(&timer, &QTimer::timeout, [&cefWindow, root = QmlWindow] {
    //        qDebug() << cefWindow->focusObject();
    //        qDebug() << root->focusObject();
    //    });
    //    timer.start(2000);

    //    testWidget->show();
    //    cefWindow->resize(QmlWindow->width(), QmlWindow->height() - 111);
    //    cefWindow->move(0, 111);
    //    cefWindow->setGeometry(0,111,QmlWindow->width(), QmlWindow->height() - 111);
    //    qDebug() << "widget before" << cefWindow->pos();

    //    mywi.setProperty("_q_embedded_native_parent_handle", QVariant(parent_HWND));


    return a.exec();
}
