// screenshot_manager.h
#pragma once

#include <QObject>
#include <QKeySequence>
#include <QPixmap>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class ScreenshotManager : public QObject {
    Q_OBJECT
public:
    explicit ScreenshotManager(QObject* parent = nullptr);
    ~ScreenshotManager();

    bool initialize();
    void setHotkey(const QKeySequence& sequence);

    signals:
        void screenshotCaptured(const QPixmap& screenshot);
    void error(const QString& message);

    private slots:
        void handleHotkeyTriggered();

private:
    QPixmap captureFullScreen();
    bool registerHotkey();
    void unregisterHotkey();

#ifdef Q_OS_WIN
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static ScreenshotManager* instance;
    HWND m_hwnd;
#endif

    QKeySequence m_currentSequence;
    bool m_hotkeyRegistered;
};