#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <exception>

#include "mainwindow.h"
#include "defines.hpp"

/* 全局异常处理 */
static void globalExceptionHandler() {
    DebugLogger::writeCrashLog("未处理异常", "未知异常导致程序终止");
    std::abort();
}

/* Qt消息处理器 */
static void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    Q_UNUSED(context);
    switch (type) {
    case QtFatalMsg: DebugLogger::writeCrashLog("QtFatal", msg); break;
    case QtCriticalMsg: DebugLogger::writeCrashLog("QtCritical", msg); break;
    default: break;
    }
    fprintf(stderr, "%s\n", msg.toLocal8Bit().constData());
}

int main(int argc, char* argv[])
{
    std::set_terminate(globalExceptionHandler);
    qInstallMessageHandler(customMessageHandler);

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString& locale : uiLanguages) {
        const QString baseName = "Graph-Visual_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    DebugLogger::instance();  /* 初始化日志 */

    MainWindow w;
    w.show();
    w.statusBar()->setMinimumHeight(28);
    /* 主题色从defines.hpp读取 */
    auto c3=[](int r,int g,int b){return QString("#%1%2%3").arg(r,2,16,QChar('0')).arg(g,2,16,QChar('0')).arg(b,2,16,QChar('0'));};
    QString winBg=c3(DEF_THEME_WINDOW),barBg=c3(DEF_THEME_BAR),border=c3(DEF_THEME_BORDER);
    QString text=c3(DEF_THEME_TEXT),hover=c3(DEF_THEME_HOVER),panel=c3(DEF_THEME_PANEL);
    QString inpBg=c3(DEF_THEME_INPUT_BG),inpTxt=c3(DEF_THEME_INPUT_TEXT);
    QString sbarText=c3(DEF_SBAR_NORMAL);
    w.statusBar()->setStyleSheet(QString("QStatusBar{font-size:12px;min-height:26px;background:%1;color:%2;border-top:1px solid %3;}").arg(barBg,sbarText,border));
    w.setStyleSheet(QString("QMainWindow{background:%1;}QMenuBar{background:%2;color:%3;border-bottom:1px solid %4;}QMenuBar::item:selected{background:%5;}"
        "QMenu{background:%2;color:%3;border:1px solid %4;}QMenu::item:selected{background:%5;}"
        "QMessageBox{background:%2;color:%3;}QMessageBox QLabel{color:%3;}QMessageBox QPushButton{background:%6;color:%3;border:1px solid %4;padding:4px 16px;border-radius:3px;}"
        "QMessageBox QPushButton:hover{background:%5;}QInputDialog{background:%2;color:%3;}QInputDialog QLabel{color:%3;}QInputDialog QLineEdit{background:%7;color:%8;border:1px solid %4;padding:4px;}")
        .arg(winBg,barBg,text,border,hover,panel,inpBg,inpTxt));
    return QApplication::exec();
}
