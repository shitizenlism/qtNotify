#ifndef __CONFIGS_HH__
#define __CONFIGS_HH__

#ifdef  CONFIGS_GLOBAL
#define CONFIGS_EXT
#else
#define CONFIGS_EXT extern
#endif /* CONFIGS_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <QString>
#include <QObject>
#include <QDebug>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include "qjsonobject.h"
#include "qdatetime.h"
#include <atomic>
#include "notify.hh"

using namespace std;

#define CONFIGS_FILEPATH "notify.conf"

class Configs : public QObject
{
    Q_OBJECT
public:
    static Configs* GetInstance();
    bool load_conf(const char *conf_filename=CONFIGS_FILEPATH);
    bool save_conf();

    QString m_datasvr_host;
    int m_datasvr_port;

    QString m_videoID_A;
    QString m_obsID_A;
    int m_tableID_A;
    QString m_videoID_B;
    QString m_obsID_B;
    int m_tableID_B;

private:
    explicit Configs(QObject *parent = nullptr);
    ~Configs();

private:
    static Configs* m_iConfigsInstance;
    char m_cfile[128];

};

/**
  * @}
  */

#endif /* __CONFIGS_HH__ */


