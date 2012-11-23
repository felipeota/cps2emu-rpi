#include <QtGui/QFileDialog>
#include <QTimer>
#include <QProcess>
#include <QMessageBox>
#include <QSettings>
#include "cps2emu.h"
#include "ui_cps2emu.h"

CPS2emu::CPS2emu(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::CPS2emu)
{
    ui->setupUi(this);

    i = 0;
    romcnv = false;    
    running = false;
    //1 second timer
    startTimer(1000);

    //Load Stored Settings...
    QSettings cps2Config("cps2emu","cps2gui");
    path = cps2Config.value("recentFile").toString();

    //If recentFile exists, RUN goes enabled
    QFileInfo recent(path);
    if (recent.exists()){
        ui->romButton->setText(recent.fileName());
        ui->runButton->setEnabled(true);
    }
}

CPS2emu::~CPS2emu()
{
    //Write last path to settings
    QSettings cps2Config("cps2emu","cps2gui");
    cps2Config.setValue("recentFile", path);

    delete ui;
}

void CPS2emu::on_romButton_clicked()
{
    //Default location, load at constructor
    if(path == NULL){
        path = "";
    }
    QFileInfo recent(path);

    QString openPath = QFileDialog::getOpenFileName(this, "Select a ROM...", recent.dir().path(),
                                        "ROM files (*.zip)");

    //After change path is recentPath

    if(openPath != NULL){
        path = openPath;
        QFileInfo rom(path);

        //Check hi and nvram folders
        checkHiScore();
        checkNVRAM();

        //Check cache file
        if(checkCache()){
            ui->runButton->setEnabled(true);
            ui->romButton->setText( rom.fileName() );
        }
        else{
            QMessageBox msgBox;
            QPushButton *createBt = msgBox.addButton(tr("Create"), QMessageBox::AcceptRole);
            msgBox.addButton(QMessageBox::Abort);

            msgBox.setWindowTitle("CPS2emu - cache");
            msgBox.setText("Cache file not found!");
            msgBox.setInformativeText("Create cache file now?");

            msgBox.exec();

            if(msgBox.clickedButton() == createBt){

                cps2cache = new QProcess;
                //cps2cache->setTextModeEnabled(true);
                //cps2cache->setReadChannelMode(QProcess::MergedChannels);
                cps2cache->setWorkingDirectory(rom.dir().path());
                cps2cache->start("/usr/bin/cps2romcnv ./" + rom.fileName());

                //romcnv running...
                romcnv = true;
                //startTimer(300);
                ui->txt->setText("Caching ROM file... Please Wait!");
                ui->romButton->setText( rom.fileName() + ", caching!");

                //Disable buttons
                ui->runButton->setEnabled(false);
                ui->romButton->setEnabled(false);
            }
            else{
                ui->romButton->setText( rom.fileName() + ", not cached!");
                ui->runButton->setEnabled(false);
            }
        }
    }

}

void CPS2emu::on_runButton_clicked()
{
    QFileInfo rom(path);

    cps2run = new QProcess;
    if(checkCache()){
        cps2run->setWorkingDirectory(rom.dir().path());
        cps2run->start("/usr/bin/cps2emu " + rom.fileName());
        ui->runButton->setEnabled(false);
        ui->romButton->setEnabled(false);
        ui->txt->setText("CPS2emu running");
        running = true;
    }
    else{
        QMessageBox::critical(this,"CPS2emu Critical Error!",
                              "ROM file " + rom.baseName() + ", not cached!",
                              QMessageBox::Ok);
    }

}


/**
 * Timed events
 */
void CPS2emu::timerEvent(QTimerEvent *event){
    bool romOK = false;
    char buf[80];
    qint64 lineTam;

    //Try detect PS3 SIXAXIS/DS3 controller...
    QFileInfo js("/dev/input/js0");
    if(js.exists()){        

        ui->controllerLabel->setText("PS3 controller found! All keys remapped!");
    }
    else{
        ui->controllerLabel->setText("Using N900 keyboard!");
    }

    //Check running process
    if(running){
        if(cps2run->atEnd()){
            ui->runButton->setEnabled(true);
            ui->romButton->setEnabled(true);
            ui->txt->setText("CPS2emu ready!");
        }
    }

    /*
     * Check if romcnv still working!
     * It not working with atEnd like cps2run sorry ...
     */
    if(romcnv == true){
        if(++i%2){
            ui->txt->setText("Caching ROM file, Please... ("  + QString::number(i) + "s)");
        }
        else{
            ui->txt->setText("Caching ROM file, Wait!.... ("  + QString::number(i) + "s)");
        }

        //Read process line        
        lineTam = cps2cache->readLine(buf, sizeof(buf));
        if (lineTam != -1) {
            //looks for complete text on dump!
            if(tr(buf).contains("complete")){
                romOK = true;
                cps2cache->close();
                i = 0;
            }
            else if(tr(buf).contains("ERROR")){
                romOK = false;
                cps2cache->close();
                i = 0;
            }
        }

        QFileInfo rom(path);
        if(!cps2cache->isOpen()){
            if(romOK){
                ui->romButton->setText( rom.fileName() + ", cached!" );

                //Enable buttons
                ui->runButton->setEnabled(true);
                ui->romButton->setEnabled(true);

                QMessageBox::warning(this,"CPS2emu romcnv!",
                                     "ROM file " + rom.baseName() + ", cached!",
                                     QMessageBox::Ok);

                romcnv = false;
                i = 0;

                ui->romButton->setText("ROM file " + rom.baseName() + ", cached!");
            }
            else{ //On cache error, because invalid ROMs
                ui->romButton->setText( rom.fileName() + ", cache error!" );

                //Enable buttons
                ui->runButton->setEnabled(false);
                ui->romButton->setEnabled(true);

                QMessageBox::critical(this,"CPS2emu romcnv!",
                                      "ROM file " + rom.baseName() +
                                      ", cache error!",
                                      QMessageBox::Ok);                

                romcnv = false;

            }
        }

    }
}

/**
  * Check if romname.cache file alread exists
  */
bool CPS2emu::checkCache(){
    QFileInfo rom(path);
    //Check if cache is ok! for selected ROM.
    QFileInfo cache(rom.dir().path() + "/cache/" + rom.baseName() + ".cache");
    if(cache.exists()){
        return true;
    }
    return false;
}


/**
  * Check if "hi" folder alread exists
  */
bool CPS2emu::checkHiScore(){
    QFileInfo rom(path);
    //Check if nvram folder exists
    QDir hi(rom.dir().path() + "/hi/");
    if(hi.exists()){
        return true;
    }
    else{
        if(hi.mkpath(rom.dir().path() + "/hi/")){
            return true;
        }
        else{
            return false;
        }
    }
}

/**
  * Check if NVRAM folder alread exists
  */
bool CPS2emu::checkNVRAM(){
    QFileInfo rom(path);
    //Check if nvram folder exists
    QDir nvram(rom.dir().path() + "/nvram/");
    if(nvram.exists()){
        return true;
    }
    else{
        if(nvram.mkpath(rom.dir().path() + "/nvram/")){
            return true;
        }
        else{
            return false;
        }
    }
}
