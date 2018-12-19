#include "prefsdialog.h"
#include "ui_prefsdialog.h"

#include <QDebug>

prefsDialog::prefsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prefsDialog)
{
    ui->setupUi(this);
    QString Meldung = getPrefs();
}

prefsDialog::~prefsDialog()
{
    delete ui;
}

//
// Save Prefs to disk file (basicboss.ini)
//
QString prefsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button == ui->buttonBox->button(QDialogButtonBox::Save) )
    {
        if(ui->leCompiler->text().isEmpty() || ui->leHandbuch->text().isEmpty() || ui->leEmulator->text().isEmpty() || ui->leProjectDir->text().isEmpty() )
        {
            QMessageBox::warning(this, tr("BasicBoss Einstellungen"),
                               tr("Es wurden nicht alle erforderlichen Pfade angegeben.\nDas Programm kann so nicht korrekt arbeiten!\n"
                                  "Bitte füllen Sie alle Angaben in den Einstellungen aus."),
                               QMessageBox::Ok);

        }
        else
        {
            myPrefs << ui->leCompiler->text() << ui->leHandbuch->text() << ui->leEmulator->text() << ui->leProjectDir->text();

            // Debug:
            // for (int i = 0; i < myPrefs.size(); ++i)
            //         qDebug() << myPrefs.at(i).toLocal8Bit().constData();

            QString str = myPrefs.join(", ");

            QString filename = "basicboss.ini";
                QFile file(filename);
                if (file.open(QIODevice::ReadWrite))
                {
                    QTextStream stream(&file);
                    stream << str << endl;
                }
                else
                {
                    qDebug() << "Dateifehler!";
                }

           this->close();  // Dialog verlassen

           QMessageBox::information(this, tr("BasicBoss"),
                               tr("Einstellungen gespeichert\n"
                                  "Bitte starten Sie das Programm neu."),
                                    QMessageBox::Ok);
        }        
    }
    else if(button == ui->buttonBox->button(QDialogButtonBox::Reset) )
    {
        ui->leCompiler->clear();
        ui->leHandbuch->clear();
        ui->leEmulator->clear();
        ui->leProjectDir->clear();
    }
    else if(button == ui->buttonBox->button(QDialogButtonBox::Cancel) )
    {
        this->close();
        return "GUI reset";
    }
    return "GUI reset";
}

void prefsDialog::on_btnFileOpenCompiler_clicked()
{
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Pfad zum BasicBoss Compiler"), "bbcompiler.exe",
            tr("Ausführbare Datei (*.exe);;All Files (*)"));
    ui->leCompiler->setText(fileName);
}

void prefsDialog::on_btnFileOpenHandbuch_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Pfad zum BasicBoss Handbuch"), "Handbuch.txt",
            tr("Textdateiatei (*.txt);;All Files (*)"));
    ui->leHandbuch->setText(fileName);

}

void prefsDialog::on_btnFileOpenEmulatot_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Pfad zum C64-Emulator"), "x64.exe",
            tr("Ausführbare Datei (*.exe);;All Files (*)"));
    ui->leEmulator->setText(fileName);
}

void prefsDialog::on_btnDirOpenProjekt_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->leProjectDir->setText(dir);
}

QString prefsDialog::getPrefs()
{
    QString line;
    QStringList fields;
    QFile file("basicboss.ini");

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, "BasicBoss Einstellungen", "Fehler beim Öffnen der ini-Datei:\n" + file.errorString() + "\n\nBitte erstellen Sie zuerst die ini-Datei\nund starten Sie das Programm neu!");
        return "Fuck you!";
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

        ui->leCompiler->setText(fields[0]);
        ui->leHandbuch->setText(fields[1]);
        ui->leEmulator->setText(fields[2]);
        ui->leProjectDir->setText(fields[3]);
    }

    return line;
}
