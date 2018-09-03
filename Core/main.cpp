#include "widgets/mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QTranslator>
#include <QMessageBox>

#include "Util/rutil.h"
#include "Util/rlog.h"
#include "constants.h"
#include "global.h"
#include "file/globalconfigfile.h"
#include "Util/rsingleton.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName(Constant::ApplicationName);
    QApplication::setOrganizationDomain(QObject::tr("rengu.com"));
    QApplication::setOrganizationName(QObject::tr("NanJing RenGu"));
    QApplication::setApplicationVersion(Constant::Version);

#if QT_VERSION > 0x050000
    QTextCodec * codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    QString configFullPath = qApp->applicationDirPath() + QString(Constant::PATH_ConfigPath);

    //解析INI文件
    QSettings * settings = new QSettings(configFullPath+"/config.ini",QSettings::IniFormat);
    RUtil::setGlobalSettings(settings);

    //解析XML文件
    RGlobal::G_GlobalConfigFile = new GlobalConfigFile;
    RGlobal::G_GlobalConfigFile->setSettings(settings);
    if(!RGlobal::G_GlobalConfigFile->parseFile()){
        QMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("System INI file parsed false!"),QMessageBox::Yes,QMessageBox::Yes);
        return -1;
    }

    if(!RSingleton<RLog>::instance()->init(RGlobal::G_GlobalConfigFile->logConfig))
    {
        QMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Log module initialization failure!"),QMessageBox::Yes,QMessageBox::Yes);
    }

    //语言
    QTranslator translator;

    QString translationPath = configFullPath + QString(Constant::CONFIG_LocalePath);
    if(RUtil::createDir(translationPath))
    {
        QStringList uiLanguages;

#if (QT_VERSION >= 0x040801) || (QT_VERSION >= 0x040800 && !defined(Q_OS_WIN))
        uiLanguages = QLocale::system().uiLanguages();
#endif

        foreach(QString locale, uiLanguages)
        {
#if (QT_VERSION >= 0x050000)
            locale = QLocale(locale).name();
#else
            locale.replace(QLatin1Char('-'), QLatin1Char('_'));
#endif
            if(translator.load(QString(Constant::ApplicationName)+"_"+ locale,translationPath))
            {
                a.installTranslator(&translator);
                a.setProperty("rimLocale", locale);
                break;
            }
        }
    }

    //样式
    QFile blackStyleFile(":/resource/style/Black.qss");
    if(!blackStyleFile.open(QFile::ReadOnly)){
        RLOG_ERROR("style file read error!");
        return -1;
    }
    a.setStyleSheet(blackStyleFile.readAll());

    MainWindow w;
    w.show();

    return a.exec();
}
