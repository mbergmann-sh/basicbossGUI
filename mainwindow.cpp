/* ******************************************************************************************************************
   *** BasicBoss Cross Compiler V2.0                                                                              ***
   *** ------------------------                                                                                   ***
   *** author : Michael Bergmann, C-Version by Marco Timm                                                                                        ***
   *** date   : 10.01.2008                                                                                        ***
   *** version: v0.3                                                                                              ***
   *** contact: mb(at)mbergmann-sh                                                                         ***
   *** licence: FREEWARE                                                                                          ***
   ***                                                                                                            ***
   *** Anwendung: bbcompiler.exe infile [outfile]                                                                 ***
   *** -infile muss prg-format sein                                                                               ***
   *** -outfile wird im prg-format geschrieben                                                                    ***
   ***                                                                                                            ***
   *** WICHTIG!!!:                                                                                                ***
   ***  PROTOCOL muss in der 1.Zeile im Quelltext stehen, damit die Fehler angezeigt werden koennen!!!            ***
   ***  (Quelltext: 0 PfundzeichenPROTOCOL )                                                                      ***
   ***                                                                                                            ***
   *** Der BasicBoss Cross Compiler ist ein 6502 emulator, der den BasicBoss V2.4 ausfuehrt.                      ***
   *** BasicBoss ist ein BasicCompiler für den C64, geschrieben von Thilo Herrmann und Veroeffendlicht 1988.      ***
   ***	                                                                                                          ***
   ***	 Quelle: 64'er Extra Nr. 11                                                                               ***
   ***	 (c) 1988 by Markt&Technik Verlag Aktiengesellschaft,                                                     ***
   ***	 Hans-Pinsel-Straße 2, D-8013 Haar bei München/West-Germany                                               ***
   ******************************************************************************************************************/
#include <iostream>
#include <cstdio>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "boss_abbild.h"


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QProcess>

/******************
** Qt5 GUI Code ***
*******************/

// Prozess einrichten und starten:
static QProcess myProcess;
static QProcess myEmulator;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ProcBeendet(int, QProcess::ExitStatus)));

    ui->actionCompilieren->setDisabled(true);
    ui->actionStart_im_Emulator->setDisabled(true);
    ui->statusBar->showMessage("BasicBoss gestartet.");
    ui->textBrowser->clear();

    // Voreinstellungen laden
    getPrefs();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnFileOpen_clicked()
{
    QDir testdir = s_projectdir;
    if (!(testdir.exists()))
    {
        ui->statusBar->showMessage("FEHLER: Voreinstellungen nicht korrekt.");
        on_actionEinstellungen_triggered();
    }
    else
    {
        ui->statusBar->showMessage("");
        ui->btnCompile->setDisabled(true);
        ui->actionCompilieren->setDisabled(true);
        QString fileName = QFileDialog::getOpenFileName(this,
                tr("Pfad zum Quellprogramm"), s_projectdir,
                tr("C64 Programmdatei (*.prg);;All Files (*)"));
        ui->leSource->setText(fileName);
        if( !(ui->leSource->text().isEmpty()))
        {
            QFile f(fileName);
            QString path = f.fileName();
            QString file = path.section("/",-1,-1);
            QString dir = path.section("/",0,-2);
            QString newfile = dir + "/+" + file;
            ui->leTarget->setText(newfile);
            ui->btnCompile->setEnabled(true);
            ui->actionCompilieren->setEnabled(true);
            ui->textBrowser->clear();
            ui->textBrowser->append("Quelldatei gesetzt: " + path);
            ui->textBrowser->append("Zieldatei gesetzt: " + newfile);
            ui->textBrowser->append("\nBereit zum Compilieren!\n");
            ui->statusBar->showMessage("Bereit für Compilerlauf.");

        }
        else
        {
            ui->statusBar->showMessage("Keine Quelldatei angegeben!");
            ui->textBrowser->clear();
            ui->leTarget->clear();
        }
    }
}

void MainWindow::on_actionBasicBoss_Handbuch_triggered()
{
    ui->textBrowser->clear();
    QFile file(s_handbuch);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
           ui->statusBar->showMessage("Hilfedatei konnte nicht geladen werden");
           QMessageBox::warning(this, tr("BasicBoss"),
                              tr("Die Hilfedatei wurde im angegebenen Pfad nicht gefunden!\n"
                                 "Bitte setzen Sie den korrekten Pfad in den Einstellungen."),
                              QMessageBox::Ok);
           return;
       }
       ui->statusBar->showMessage("BasicBoss Hilfedatei wurde geladen.");

       while (!file.atEnd()) {
           QByteArray line = file.readLine();
           ui->textBrowser->append(line);
       }
}

void MainWindow::on_actionEinstellungen_triggered()
{
    prefsDialog *myprefs = new prefsDialog;
    myprefs->show();
    //getPrefs();
}

void MainWindow::on_actionLaden_triggered()
{
  on_btnFileOpen_clicked();
}

void MainWindow::on_actionInfo_zu_Qt_triggered()
{
    QApplication::aboutQt();
}

// close() Event abfangen:
void MainWindow::closeEvent( QCloseEvent *event )
{
    // einen Dialog instanzieren:
    QMessageBox exitBox (this);
    // Eigene Schaltflächen definieren:
    QAbstractButton *exitButton = exitBox.addButton(tr("Ende"), QMessageBox::ActionRole);
    QAbstractButton *stayButton = exitBox.addButton(tr("Abbruch"), QMessageBox::ActionRole);
    // Dialog-Icon setzen:
    exitBox.setIconPixmap(QPixmap(":/img/img/exit.png"));
    // Meldungstext:
    exitBox.setText("Möchten Sie BasicBoss wirklich verlassen?");
    // Dialogfenster-Titel:
    exitBox.setWindowTitle(tr("Programm beenden"));

    // Dialog öffnen
    exitBox.exec();

    // Dialog Antwort des Benutzers auswerten:
    // Auf das close-Event reagieren
    if (exitBox.clickedButton() == exitButton)
    {
        // Damit die Anwendung schließt muß das Event akzeptiert werden
        // Anwendung schließen:
        event->accept();
    }

    else if (exitBox.clickedButton() == stayButton)
    {
        // Damit die Anwendung nicht schließt muß das Event verworfen werden
        // Beenden abbrechen
        event->ignore();
    }
}

void MainWindow::on_actionInfo_zum_Programm_triggered()
{
    QMessageBox messageBox (this);
    messageBox.setIconPixmap(QPixmap(":/img/img/c64_logo.png"));
    messageBox.setText("<b>BasicBoss v1.0</b> <br /><i>Ein Programm zur Übersetzung von CBM-BASIC 2.0 <br />.PRG-Dateien (BASIC-Programme) in Maschinensprache</i><br /><br>Teile dieses Programms basieren auf bbcompiler v0.2 von Marco Timm<br /><br />Autor: Michael Bergmann 2017<br />Web: www.mbergmann-sh.de<br />Mail: mb@mbergmann-sh.de");

    messageBox.setWindowTitle(tr("Über das Programm"));
    QAbstractButton *okButton = messageBox.addButton(tr("Hab's gelesen"), QMessageBox::ActionRole);
    messageBox.exec();
}

void MainWindow::on_leTarget_textChanged(const QString &arg1)
{
    if(!(ui->leTarget->text().isEmpty() || ui->leSource->text().isEmpty()))
    {
        ui->btnCompile->setEnabled(true);
    }
    else
    {
        ui->btnCompile->setDisabled(true);
    }
    on_leTarget_editingFinished();

}

void MainWindow::on_leSource_textChanged(const QString &arg1)
{
    on_leTarget_textChanged("Fuck");
}

void MainWindow::on_leTarget_editingFinished()
{
    QFileInfo srcinfo = ui->leSource->text();
    QFileInfo info = ui->leTarget->text();
    QDir d = QFileInfo(info).absoluteDir();
    QDir pathDir(d.absolutePath());
    QString suffix = info.completeSuffix();
    QString pfad = info.absoluteFilePath();
    QString file = info.fileName();
    QString plainfile = info.baseName();
    QString sourcefile = srcinfo.fileName();

    debugVars();

    // Fehler bei händischer Eingabe abfangen:
    if(!(ui->leTarget->text().isEmpty() || ui->leSource->text().isEmpty()))
    {
        if (pathDir.exists())
        {
            if(!file.isEmpty() && (suffix == "PRG" || suffix == "prg" ) && (!(plainfile.isEmpty())))
            {
                if(!(file == sourcefile))
                {
                    s_sourcefile = srcinfo.absoluteFilePath();
                    s_targetfile = info.absoluteFilePath();                    
                    ui->btnCompile->setEnabled(true);
                    ui->actionCompilieren->setEnabled(true);
                    ui->actionStart_im_Emulator->setDisabled(true);
                    ui->btnStart->setDisabled(true);
                    ui->statusBar->showMessage("Compiler bereit.");                   
                }
                else
                {
                    s_sourcefile = "";
                    s_targetfile = "";

                    ui->btnCompile->setDisabled(true);
                    ui->actionCompilieren->setDisabled(true);
                    ui->statusBar->showMessage("FEHLER: Dateinamen identisch.");
                }
            }
            else
            {
                s_sourcefile = "";
                s_targetfile = "";

                ui->btnCompile->setDisabled(true);
                ui->actionCompilieren->setDisabled(true);
                ui->statusBar->showMessage("FEHLER: Suffix falsch oder kein Dateiname angegeben.");
            }
        }
        else
        {
            s_sourcefile = "";
            s_targetfile = "";

            ui->btnCompile->setDisabled(true);
            ui->actionCompilieren->setDisabled(true);
            ui->statusBar->showMessage("FEHLER: Dateipfad existiert nicht.");
        }
    }
    else
    {
        s_sourcefile = "";
        s_targetfile = "";
        ui->btnCompile->setDisabled(true);
        ui->actionCompilieren->setDisabled(true);
        ui->statusBar->showMessage("FEHLER: kein Ziel angegeben angegeben.");
    }
}

void MainWindow::on_btnCompile_clicked()
{
    startProc(ui->leSource->text(), ui->leTarget->text());
}


void MainWindow::on_btnStart_clicked()
{
    ui->actionStart_im_Emulator->triggered();
}

/*
 * Voreinstellungen lesen
 */
void MainWindow::getPrefs()
{
    QString line;
    QStringList fields;
    QFile file("basicboss.ini");

    if(!file.open(QIODevice::ReadOnly))
    {
        //QMessageBox::information(this, "BasicBoss Einstellungen", "Fehler beim Öffnen der ini-Datei:\n" + file.errorString());
        on_actionEinstellungen_triggered();
    }
    else
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            line = in.readLine();
            fields = line.split(", ");            
        }

        file.close();

        // Variablen für Shell-Befehle initialisieren
        s_compiler = fields[0];
        s_handbuch = fields[1];
        s_emulator = fields[2];
        s_projectdir = fields[3];

        ui->statusBar->showMessage(tr("Voreinstellungen geladen."));
    }
}


/*
* Haupteinsprungspunkt für den Compiler
*/


void MainWindow::on_actionCompilieren_triggered()
{
   startProc(ui->leSource->text(), ui->leTarget->text());
}


void MainWindow::on_actionStart_im_Emulator_triggered()
{
    /*
     * Aufruf vice mit Parametern unter Windows:
     * @x64.exe -default -parallel8 1 -autostartprgmode 2 -8 "%APPDATA%\vice\3.0\autostart-C64.d64" -autostartprgdiskimage "%APPDATA%\vice\3.0\autostart-C64.d64" -autostart %1
     */

    // Remark: starts x64 and runs bbcompiled prg under Linux - but only starts x64 on Windows..
    // Needs to be fixed!

    QString command = s_emulator;
    QStringList arguments;
    arguments << "-default" << s_targetfile;

    ui->statusBar->showMessage(tr("Emulator gestartet."));

    myEmulator.start(command, arguments);
    //myEmulator.startDetached(command, arguments, s_projectdir);

    QObject::connect(&myEmulator, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(emu_finished(int,QProcess::ExitStatus)));
    QObject::connect(&myEmulator, SIGNAL(readyReadStandardOutput()), this, SLOT(emu_readyReadStandardOutput()));
    QObject::connect(&myEmulator, SIGNAL(started()), this, SLOT(started()));

    debugVars();
}

int MainWindow::startProc(QString infile, QString outfile)
{
    debugVars();
    QString command = s_compiler;
    QStringList arguments;
    arguments << s_sourcefile << s_targetfile;

    ui->statusBar->showMessage(tr("Compilerlauf gestartet."));

    myProcess.start(command, arguments);

    QObject::connect(&myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    QObject::connect(&myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    QObject::connect(&myProcess, SIGNAL(started()), this, SLOT(started()));

    //qDebug() << myProcess.arguments();

    return 0;
}

void MainWindow::error(QProcess::ProcessError error)
{
  qDebug() << "Error: " << error;
}

void MainWindow::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitCode == 0)
    {
        ui->actionStart_im_Emulator->setEnabled(true);
        ui->btnStart->setEnabled(true);
        ui->actionCompilieren->setDisabled(true);
        ui->btnCompile->setDisabled(true);
//        ui->leSource->clear();
//        ui->leTarget->clear();
    }
    else
    {
        ui->actionStart_im_Emulator->setDisabled(true);
        ui->btnStart->setDisabled(true);
        ui->actionCompilieren->setDisabled(true);
        ui->btnCompile->setDisabled(true);

    }
  qDebug() << "Finished: " << exitCode;
  //qApp->exit();

  if (exitStatus==QProcess::CrashExit || exitCode!=0)
  {
      ui->statusBar->showMessage("Fehler beim Compilerlauf!");
      QMessageBox::critical(this, tr("BasicBoss Compiler"),
      tr("Fehler bei der Compilierung - der Prozess ist abgestürzt!"
      "Fehlende Schreibrechte?\nPfade falsch gesetzt?"),
      QMessageBox::Ok);
  }
  else
  {
      ui->statusBar->showMessage("Compilerlauf abgeschlossen.");
      ui->actionStart_im_Emulator->setEnabled(true);

      (void)QMessageBox::information(this, tr("BasicBoss Compiler"),
      tr("Compilierung erfolgreich.\n"
      "Herzlichen Glühstrumpf!"),
      QMessageBox::Ok);
  }
}

void MainWindow::readyReadStandardError()
{
  qDebug() << "ReadyError";
}

void MainWindow::readyReadStandardOutput()
{
  qDebug() << "readyOut";
  QProcess *myProcess = (QProcess *)sender();
  QByteArray buf = myProcess->readAllStandardOutput();

  QFile data(s_projectdir +"\\bbcompiler_out.txt");
  if (data.open(QFile::WriteOnly | QFile::Truncate))
  {
      ui->textBrowser->clear();
      QTextStream out(&data);
      out << buf;
      ui->textBrowser->append(buf);
  }
}

void MainWindow::started()
{
  qDebug() << "Proc Started";
}

void MainWindow::emu_readyReadStandardOutput()
{
  qDebug() << "readyOut";
  QProcess *myEmulator = (QProcess *)sender();
  QByteArray buf = myEmulator->readAllStandardOutput();

  qDebug() << buf;
}

void MainWindow::emu_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitCode == 0)
    {
        qDebug() << "Emulator normal beendet.";
    }
    else
    {
        qDebug() << "Emulator NICHT normal beendet.";
    }
  qDebug() << "Finished: " << exitCode;

  if (exitStatus==QProcess::CrashExit || exitCode!=0)
  {
      ui->statusBar->showMessage("CrashExit - Problem mit dem Emulator!!");
  }
  else
  {
      ui->statusBar->showMessage("Emulator normal beendet.");
      ui->actionStart_im_Emulator->setEnabled(true);
  }
}

void MainWindow::debugVars()
{
//    QString s_handbuch;
//    QString s_compiler;
//    QString s_emulator;
//    QString s_projectdir;
//    QString s_sourcefile;
//    QString s_targetfile;
//    QStringList myPrefs;
    qDebug() << "===========================================================================================";
    qDebug() << "s_projectdir: " << s_projectdir;
    qDebug() << "s_sourcefile: " << s_sourcefile;
    qDebug() << "s_targetfile: " << s_targetfile;
    qDebug() << "s_compiler: " << s_compiler;
    qDebug() << "s_emulator: " << s_emulator;
    qDebug() << "s_handbuch: " << s_handbuch;
    qDebug() << "myPrefs: " << myPrefs;

}


