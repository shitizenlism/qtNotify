#ifndef __MAINWIN_HH__
#define __MAINWIN_HH__


/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSystemTrayIcon>



/** @addtogroup MAINWIN
  * @{
  */
 
int send_obs_switch_msg(const char *TcpHost, unsigned short usPort, int scene_id, unsigned int seq);
/*============================================================================*/
/*                             @MACROS & @TYPEDEFS                            */
/*============================================================================*/
                                                                                
/*============================================================================*/
/*                             @GLOBAL VIRIABLES                              */
/*============================================================================*/
                                                                                
/*============================================================================*/
/*                                   @FUNCS                                   */
/*============================================================================*/
                                                                                
/*============================================================================*/
/*                                   @CLASS                                   */
/*============================================================================*/
namespace Ui {
class CMainWin;
}

class CMainWin : public QMainWindow
{
    Q_OBJECT

public:
    CMainWin(QWidget *parent = nullptr);
    ~CMainWin();

public slots:
    void onBtnTableAClicked();
    void onBtnTableBClicked();

private:
    Ui::CMainWin *ui;
    unsigned int m_seq;
};                                                                                

/**                                                                             
  * @}                                                                         
  */                                                                            
                                                                                
#endif /* __MAINWIN_HH__ */                                       


