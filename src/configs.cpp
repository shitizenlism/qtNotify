#define  CONFIGS_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "qfile.h"
#include "qjsondocument.h"
#include "qjsonarray.h"
#include "qdebug.h"
#include "configs.hh"


/** @defgroup CONFIGS
  * @brief CONFIGS system modules
  * @{
  */

/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/** @defgroup CONFIGS_Private_Functions
  * @{
  */

/**
  * @}
  */

/** @defgroup CONFIGS_CLASS_Functions
  * @example: Configs::GetInstance()->load_conf()
  */
Configs* Configs::m_iConfigsInstance = nullptr;

Configs *Configs::GetInstance()
{
    if(m_iConfigsInstance == nullptr){
        m_iConfigsInstance = new Configs();
    }
    return m_iConfigsInstance;
}

Configs::Configs(QObject *parent) : QObject(parent)
{

}
Configs::~Configs()
{

}

bool Configs::load_conf(const char *conf_filename)
{
    QFile presetFile(conf_filename);
    if (!presetFile.open(QIODevice::ReadOnly))	{
        qWarning()<<"load_conf() failed at #tp1";
        return false;
    }

    QByteArray data = presetFile.readAll();
    QJsonParseError error;
    QJsonDocument jDoc(QJsonDocument::fromJson(data,&error));
    if (jDoc.isNull())	{
        qWarning()<<"load_conf() failed at #tp2";
        return false;
    }

    QJsonObject cjson = jDoc.object();
    QJsonObject datasvr = cjson.value("datasvr").toObject();
    m_datasvr_host = datasvr.value("host").toString();
    m_datasvr_port = datasvr.value("port").toInt(99);

    QJsonObject tableA = cjson.value("tableA").toObject();
    m_videoID_A = tableA.value("videoID").toString();
    m_obsID_A = tableA.value("obsID").toString();
    m_tableID_A = tableA.value("tableID").toInt(99);

    QJsonObject tableB = cjson.value("tableB").toObject();
    m_videoID_B = tableB.value("videoID").toString();
    m_obsID_B = tableB.value("obsID").toString();
    m_tableID_B = tableB.value("tableID").toInt(99);

    strcpy(m_cfile, conf_filename);
    presetFile.close();
    return true;
}

bool Configs::save_conf()
{
    QFile file(m_cfile);

    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "couldn't open conf file for save.";
        return false;
    }
    QJsonObject datasvr,tableA,tableB;
    datasvr["host"] = m_datasvr_host;
    datasvr["port"] = m_datasvr_port;
    tableA["videoID"] = m_videoID_A;
    tableA["obsID"] = m_obsID_A;
    tableA["tableID"] = m_tableID_A;
    tableB["videoID"] = m_videoID_B;
    tableB["obsID"] = m_obsID_B;
    tableB["tableID"] = m_tableID_B;

    QJsonObject json;
    json["datasvr"] = datasvr;
    json["tableA"] = tableA;
    json["tableB"] = tableB;
    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();
    return true;
}






