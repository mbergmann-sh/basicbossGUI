#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLibraryInfo>
#include <QTranslator>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStringList>
#include <QStringListIterator>
#include <QFile>
#include <QTextStream>
#include <QProcess>


#include "prefsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString s_handbuch;
    QString s_compiler;
    QString s_emulator;
    QString s_projectdir;
    QString s_sourcefile;
    QString s_targetfile;
    QStringList myPrefs;

public slots:
    void error(QProcess::ProcessError error);
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void emu_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardError();
    void readyReadStandardOutput();
    void emu_readyReadStandardOutput();
    void started();

private slots:
    void on_btnFileOpen_clicked();
    void on_actionBasicBoss_Handbuch_triggered();
    void on_actionEinstellungen_triggered();
    void on_actionLaden_triggered();
    void getPrefs(); // Werte (Kommagetrennt) aus Datei einlesen
    void on_actionInfo_zu_Qt_triggered();
    void on_actionInfo_zum_Programm_triggered();
    void on_leTarget_textChanged(const QString &arg1);
    void on_leSource_textChanged(const QString &arg1);
    void on_leTarget_editingFinished();
    void on_btnCompile_clicked();
    void on_actionCompilieren_triggered();
    void on_actionStart_im_Emulator_triggered();
    int startProc(QString infile, QString outfile);

    void on_btnStart_clicked();
    void debugVars();

private:
    Ui::MainWindow *ui;
    QString projectdir;
    QProcess proc;


protected:
        void closeEvent( QCloseEvent *event );
};

#endif // MAINWINDOW_H
