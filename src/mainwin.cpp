#define  MAINWIN_GLOBAL

#define WIN32_LEAN_AND_MEAN
/* includes-------------------------------------------------------------------*/
#include "mainwin.hh"
#include "ui_mainwin.h"
#include "configs.hh"
#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QProcess>
#include <objbase.h>
#include "qmessagebox.h"

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>

#include <winsock2.h>
#include <windows.h>
#include "md5.h"

using namespace std;

typedef unsigned int t_uint32;

struct PACKET_HEADER
{
    t_uint32  cmd;
    t_uint32  size;
    t_uint32  seq;
};

#define DECL_PACKET(pack_name, id) const t_uint32 CMD_##pack_name = id; struct PK_##pack_name
#define INIT_HEAD(type, pack) (pack).head.seq=0; (pack).head.cmd = htonl(CMD_##type); (pack).head.size = htonl(sizeof(PK_##type));

DECL_PACKET(OBS_SWITCH, 0x0BA001)
{
    PACKET_HEADER head;
    char vid[4];		//video id, "BG01", "BG02"
    char id[4];			//device id. fixed: "A001"
    char token[32];		//token =  md5(vid + 'obs' + id  + "15ad55a77b5c215bdcb7d5411d0d5bd8")
    t_uint32 scene;  //1: green-room-1, 2:green-room-2
};

DECL_PACKET(OBS_SWITCH_R, 0x0BA002)
{
    PACKET_HEADER head;
    t_uint32 code;  //0: success, other:error code
};

void ItoHexstring(unsigned char* strin, char* strout, unsigned int length)
{
    char* pchar = strout;
    for (unsigned int i = 0; i < length; i++)
    {
        //char tmp[3];
        _itoa(strin[i], pchar, 16);

        if (strlen(pchar) == 1)
        {
            pchar[1] = pchar[0];
            pchar[0] = '0';
            pchar[2] = '\0';
        }
        pchar = pchar + 2;
    }
    *pchar = 0;
}

/**
 * 32位MD5加密,strout的长度必须大于32
 *
 * @author Robot.O (2013/9/4)
 *
 * @param strin
 * @param ilen
 * @param strout
 */
void EncryMD5(unsigned char* strin, unsigned int ilen, char* strout)
{
    MD5_CTX m_md5;

    MD5Init(&m_md5);
    MD5Update(&m_md5, strin, ilen);
    MD5Final(&m_md5);

    ItoHexstring(m_md5.digest, strout, ilen);
}

/*
refer to : https://docs.microsoft.com/zh-cn/windows/win32/api/winsock2/nf-winsock2-send

*/
int send_obs_switch_msg(const char *TcpHost, unsigned short usPort, int scene_id, unsigned int seq)
{
    int ret=0;
    WSADATA wsaData;
    int sockFd=-1;

    qInfo() << "connect to "<< TcpHost<<":"<<usPort<<",scene_id="<<scene_id;
    do{
        if (sockFd < 0) {
            ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (ret != NO_ERROR) {
                qCritical() << "WSAStartup failed with error:" << ret;
                ret = -1;	break;
            }

            sockFd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockFd < 0) {
                qCritical() <<"Error at socket(): "<< WSAGetLastError();
                ret = -2;	break;
            }

            int cltSockTimeout = 3000;
            struct timeval tv;
            tv.tv_sec = cltSockTimeout / 1000;
            tv.tv_usec = (cltSockTimeout % 1000) * 1000;
            setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, (char*)& tv, sizeof(tv));
            setsockopt(sockFd, SOL_SOCKET, SO_SNDTIMEO, (char*)& tv, sizeof(tv));

            //hostent *he;
            //if ((he = gethostbyname(TcpHost)) == 0) {
            //	cout<<"get host name error:"<<strerror(errno)<<endl;
            //	ret=-21; break;
            //}

            // The sockaddr_in structure specifies the address family,
            // IP address, and port for the socket that is being bound.
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(usPort);
            addr.sin_addr.s_addr = inet_addr(TcpHost);

            ret = connect(sockFd, (SOCKADDR*)& addr, sizeof(addr));
            if (ret == SOCKET_ERROR) {
                qCritical() <<"connect to server timeout! error="<<WSAGetLastError();
                ret = -3;	break;
            }

            char token[200] = { 0 };
            PK_OBS_SWITCH msg;
            INIT_HEAD(OBS_SWITCH, msg)
            msg.head.seq = htonl(seq);
            if (scene_id == 1) {
                memcpy(msg.vid, Configs::GetInstance()->m_videoID_A.toStdString().c_str(), sizeof(msg.vid));
                memcpy(msg.id, Configs::GetInstance()->m_obsID_A.toStdString().c_str(), sizeof(msg.id));
                msg.scene = htonl(Configs::GetInstance()->m_obsID_A.toInt());
            }
            else {
                memcpy(msg.vid, Configs::GetInstance()->m_videoID_B.toStdString().c_str(), sizeof(msg.vid));
                memcpy(msg.id, Configs::GetInstance()->m_obsID_B.toStdString().c_str(), sizeof(msg.id));
                msg.scene = htonl(Configs::GetInstance()->m_obsID_B.toInt());
            }

            //token =  md5(vid + 'obs' + id  + "15ad55a77b5c215bdcb7d5411d0d5bd8")
            char token_str[200] = {0};
            //sprintf(token_str, "%sobs%s15ad55a77b5c215bdcb7d5411d0d5bd8");
            memcpy(token_str,msg.vid,sizeof(msg.vid));
            memcpy(token_str+sizeof(msg.vid),"obs",3);
            memcpy(token_str+sizeof(msg.vid)+3,msg.id,sizeof(msg.id));
            memcpy(token_str+sizeof(msg.vid)+3+sizeof(msg.id),"15ad55a77b5c215bdcb7d5411d0d5bd8",strlen("15ad55a77b5c215bdcb7d5411d0d5bd8"));
            //string token_s = string(msg.vid) + string("obs") + string(msg.id) + string("15ad55a77b5c215bdcb7d5411d0d5bd8");
            qInfo() <<"token-str="<< token_str;
            EncryMD5((unsigned char*)token_str, strlen(token_str), token);
            memcpy(msg.token, token, 32);
            qInfo() <<"token="<<token<<",token_len="<<strlen(token);

            char* sndBuf = (char*)& msg;
            int len = sizeof(msg);
            if ((ret = send(sockFd, sndBuf, len, 0)) == SOCKET_ERROR) {
                qCritical() <<"send failed with error: "<<WSAGetLastError();
                ret = -4; break;
            }
            qInfo() <<"send msg done. bytes="<<ret;

            Sleep(2000);

			int count=0;
            PK_OBS_SWITCH_R* pMsgR = NULL;
            char buf[1024] = { 0 };
            ret = recv(sockFd, buf, sizeof(buf), 0);
            while (ret <= 0) {
                qCritical() << "Error when receiving data. errno=" << errno;
                Sleep(2000);
				count++;
				if (count>=5){
                	ret=-5;	break;
				}
				ret = recv(sockFd, buf, sizeof(buf), 0);
            }

			if (ret>0)
			{
                pMsgR = (PK_OBS_SWITCH_R *)buf;
                unsigned int cmd = ntohl(pMsgR->head.cmd);
                int code = ntohl(pMsgR->code);
                if ((cmd != CMD_OBS_SWITCH_R)||(code != 0)){
                    qWarning() << "datasvr response is wrong! cmd="<<cmd<<",code="<<code;
                    ret = -6;
                }else{
                    qInfo() << "datasvr confirmed. count="<<count;
                }
            }
        }
    }while(0);

    if (sockFd >= 0) {
        shutdown(sockFd, SD_BOTH);	//only winsock.h requires
        closesocket(sockFd);
    }
    WSACleanup();
    qInfo() << "session finished. Disconnect with "<< TcpHost<<":"<<usPort<<",scene_id="<<scene_id;
    return ret;
}


CMainWin::CMainWin(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CMainWin)
{
    ui->setupUi(this);
    //this->setMinimumSize(1280, 720);
    QIcon icon = QIcon(QPixmap("://icon/app.png"));
    setWindowIcon(icon);

	Configs::GetInstance()->load_conf();
    ui->tableA_videoID->setText(Configs::GetInstance()->m_videoID_A);
    ui->tableA_obsID->setText(	Configs::GetInstance()->m_obsID_A);
    ui->tableA_tableID->setValue(Configs::GetInstance()->m_tableID_A);

    ui->datasvr_host->setText(Configs::GetInstance()->m_datasvr_host);
    string port = to_string(Configs::GetInstance()->m_datasvr_port);
    ui->datasvr_port->setText(QString::fromStdString(port));

    ui->tableB_videoID->setText(Configs::GetInstance()->m_videoID_B);
    ui->tableB_obsID->setText(Configs::GetInstance()->m_obsID_B);
    ui->tableB_tableID->setValue(Configs::GetInstance()->m_tableID_B);

    connect(ui->Btn_SwitchToTableA, SIGNAL(clicked()), this, SLOT(onBtnTableAClicked()) );
    connect(ui->Btn_SwitchToTableB, SIGNAL(clicked()), this, SLOT(onBtnTableBClicked()) );
    m_seq = 0;
}

CMainWin::~CMainWin()
{
    delete ui;
}

void CMainWin::onBtnTableAClicked()
{
    Configs::GetInstance()->m_datasvr_host = ui->datasvr_host->text();
    Configs::GetInstance()->m_datasvr_port = ui->datasvr_port->text().toInt();
    Configs::GetInstance()->m_videoID_A = ui->tableA_videoID->text();
    Configs::GetInstance()->m_obsID_A = ui->tableA_obsID->text();
    Configs::GetInstance()->m_tableID_A = ui->tableA_tableID->text().toInt();
    Configs::GetInstance()->save_conf();

    QPalette highlight;
    m_seq++;
    int ret = send_obs_switch_msg(Configs::GetInstance()->m_datasvr_host.toStdString().c_str(),(unsigned short)Configs::GetInstance()->m_datasvr_port, 1, m_seq);
    if (ret <= 0){
        QMessageBox::warning(this, "Error", "Wow! Notify message failed! Please review datasvr config.");
    }else{
        //QPalette pal = QPalette();
        //pal.setColor(QPalette::Window, Qt::green);
        //ui->Btn_SwitchToTableA->setAutoFillBackground(true);
        //ui->Btn_SwitchToTableA->setPalette(pal);
        //ui->Btn_SwitchToTableA->show();
        ui->Btn_SwitchToTableA->setStyleSheet("background-color:grey;");
        ui->Btn_SwitchToTableA->setEnabled(false);

        ui->Btn_SwitchToTableB->setEnabled(true);
        ui->Btn_SwitchToTableB->setStyleSheet("background-color:green;");
    }
}

void CMainWin::onBtnTableBClicked()
{
    Configs::GetInstance()->m_datasvr_host = ui->datasvr_host->text();
    Configs::GetInstance()->m_datasvr_port = ui->datasvr_port->text().toInt();
    Configs::GetInstance()->m_videoID_B = ui->tableB_videoID->text();
    Configs::GetInstance()->m_obsID_B = ui->tableB_obsID->text();
    Configs::GetInstance()->m_tableID_B = ui->tableB_tableID->text().toInt();
    Configs::GetInstance()->save_conf();

    m_seq++;
    int ret = send_obs_switch_msg(Configs::GetInstance()->m_datasvr_host.toStdString().c_str(),(unsigned short)Configs::GetInstance()->m_datasvr_port, 2, m_seq);
    if (ret <= 0){
        QMessageBox::warning(this, "Error", "Wow! Notify message failed! Please review datasvr config.");
    }else{
        ui->Btn_SwitchToTableB->setStyleSheet("background-color:grey;");
        ui->Btn_SwitchToTableB->setEnabled(false);

        ui->Btn_SwitchToTableA->setEnabled(true);
        ui->Btn_SwitchToTableA->setStyleSheet("background-color:green;");
    }
}


