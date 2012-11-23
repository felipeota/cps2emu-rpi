#ifndef CPS2EMU_H
#define CPS2EMU_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
    class CPS2emu;
}

class CPS2emu : public QMainWindow
{
    Q_OBJECT

public:
    explicit CPS2emu(QWidget *parent = 0);
    ~CPS2emu();

private:
    Ui::CPS2emu *ui;
    QString path;
    //QString recentPath;
    QProcess *cps2cache;
    QProcess *cps2run;
    bool romcnv;
    bool running;
    bool checkCache();
    bool checkNVRAM();    
    bool checkHiScore();
    short int i;

private slots:
    void on_runButton_clicked();
    void on_romButton_clicked();
    //Timer
    void timerEvent(QTimerEvent *event);
};

#endif // CPS2EMU_H
