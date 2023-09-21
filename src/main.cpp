#include "mainwin.hh"
#include <QApplication>
#include <QSharedMemory>
#include <io.h>
#include <direct.h>
#include "qdatetime.h"
#include "qdir.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qmutex.h"


#define APP_NAME "qtNotify"
#define APP_VERSION "v1.0"

QMutex g_LogMutex;
QString g_LogFile;
unsigned int g_LogFileSectionCount=1;
#define MAX_LOG_FILE_SIZE (2*1000000)

QString getLogFile(int secId)
{
	QDateTime qLocalNow;
	QDir qLogDir;
	QString qFullDir,qCutStr("\\"),qPostfix(".log");
	QString qLocalDate;

	qLocalNow = QDateTime::currentDateTime();
	qLocalDate = qLocalNow.toString("yyyyMMdd");
    qFullDir = "logs";
	if (!qLogDir.mkpath(qFullDir)){
		return QString();
	}
	if (secId>=0)
		qFullDir += qCutStr + qLocalDate + QString("_") + QString::number(secId) + qPostfix;
	else
		qFullDir += qCutStr + qLocalDate + qPostfix;

	return qFullDir;

}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QMutexLocker locker(&g_LogMutex);

	QString msgStr("[");
	msgStr.append(QTime::currentTime().toString("hh:mm:ss.zzz")).append("]");

	switch (type) {
	case QtDebugMsg:
		msgStr.append("Debug:").append(msg);
		break;
	case QtInfoMsg:
		msgStr.append("Info:").append(msg);
		break;
	case QtWarningMsg:
		msgStr.append("Warning:").append(msg);
		break;
	case QtCriticalMsg:
		msgStr.append("Critical:").append(msg);
		break;
	case QtFatalMsg:
		msgStr.append("Fatal:").append(msg);
		break;
	}

	QFile file(g_LogFile);
	if (file.open(QIODevice::WriteOnly | QIODevice::Append)){
		if (file.size()>=MAX_LOG_FILE_SIZE){
            file.rename(getLogFile(g_LogFileSectionCount++));
			file.close();

			QFile newFile(g_LogFile);
			QTextStream ts(&newFile);
			ts << msgStr << endl;
			newFile.flush();
			newFile.close();
		}
		else{
			QTextStream ts(&file);
			ts << msgStr << endl;
			file.flush();
			file.close();
		}
	}

}

#if 0
static void Log_Init(const char* appname){
    char currentPath[1024] = {0};
    char logPath[1024] = {0};
    _getcwd(currentPath, sizeof(currentPath));
    sprintf(logPath, "%s/logs/", currentPath);
    if(_access(logPath, 0) == -1){
        _mkdir(logPath);
    }
    FLAGS_colorlogtostderr = true; //设置输出到屏幕的日志显示相应颜色
    std::string logPathString = logPath;
    std::string logfileprefix = logPathString +  "log_fatal_";
    google::SetLogDestination(google::GLOG_FATAL, logfileprefix.c_str()); // 设置 google::FATAL 级别的日志存储路径和文件名前缀
    logfileprefix = logPathString +  "log_error_";
    google::SetLogDestination(google::GLOG_ERROR, logfileprefix.c_str()); //设置 google::ERROR 级别的日志存储路径和文件名前缀
    logfileprefix = logPathString +  "log_warning_";
    google::SetLogDestination(google::GLOG_WARNING, logfileprefix.c_str()); //设置 google::WARNING 级别的日志存储路径和文件名前缀
    logfileprefix = logPathString +  "log_info_";
    google::SetLogDestination(google::GLOG_INFO, logfileprefix.c_str()); //设置 google::INFO 级别的日志存储路径和文件名前缀
    google::InitGoogleLogging(appname);
    //google::SetStderrLogging(google::GLOG_ERROR);
    FLAGS_logbufsecs = 1; //缓冲日志输出，默认为30秒，此处改为立即输出
    //LOG(INFO) << "app running......";
}
#endif

int main(int argc, char *argv[])
{
    // 确保只有一个程序运行
    //QSharedMemory shared(APP_NAME);
    //if(shared.attach()){
    //    return 0;
    //}
    //shared.create(1);

    QApplication a(argc, argv);
    g_LogFile=getLogFile(-1);
    if (!g_LogFile.isEmpty()){
        qInstallMessageHandler(myMessageOutput);
    }
    qInfo() << APP_NAME << " " << APP_VERSION << " start running......";

    a.setWindowIcon(QIcon("://icon/app.png"));
    CMainWin w;
    //w.move(0,0);
    w.show();

    return a.exec();
}
