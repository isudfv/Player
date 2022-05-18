#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJSValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QWindow>
#include <map>

#include "qbrowserwindow.h"

class WindowManager: public QObject
{
Q_OBJECT
public:
    static WindowManager *getInstanse()
    {
        static WindowManager wm;
        return &wm;
    }

    Q_INVOKABLE void toggleTab(int windowId);
    Q_INVOKABLE void doGoBack(int windowId);
    Q_INVOKABLE void doGoForward(int windowId);
    Q_INVOKABLE void doReLoad(int windowId);
    Q_INVOKABLE void doLoadUrl(int windowId, QString url, int uid);
    Q_INVOKABLE void addWindow(QString url);
    Q_INVOKABLE void addWindowNotInFocus(QString url);
    Q_INVOKABLE void removeWindow(int windowId);

signals:
    void addTab(int newWindowId, QString newWindowTitle, QString newWindowUrl, QString icon, bool toToggle);
    void removeTab(int removeWindowId);
    void setHeaderState(bool activeBack,
                        bool activeForward);
    void setTabState(int windowId,
                     QString title,
                     QString url,
                     QString icon);

public slots:
    void addWindow(QBrowserWindow *new_window);
    void tabStateChanged(int windowId,
                         const QString& title,
                         const QString& url,
                         const QString& icon);
    void loadStateChanged(int windowId, bool activeBack,
                          bool activeForward);

private:
    int currentWindowId;
    std::map<int, QBrowserWindow *> mp;
};

#endif// WINDOWMANAGER_H
