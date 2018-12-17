#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStringList>
#include <QStringListIterator>
#include <QFile>
#include <QTextStream>

namespace Ui {
class prefsDialog;
}

class prefsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit prefsDialog(QWidget *parent = 0);
    ~prefsDialog();

    QStringList myPrefs;


private slots:
    QString on_buttonBox_clicked(QAbstractButton *button);
    void on_btnFileOpenCompiler_clicked();
    void on_btnFileOpenHandbuch_clicked();
    void on_btnFileOpenEmulatot_clicked();
    void on_btnDirOpenProjekt_clicked();
    QString getPrefs(); // Werte (Kommagetrennt) aus Datei einlesen

private:
    Ui::prefsDialog *ui;
};

#endif // PREFSDIALOG_H
