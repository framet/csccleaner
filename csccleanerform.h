#ifndef CSCCLEANERFORM_H
#define CSCCLEANERFORM_H

#include <QWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QListWidget>
#include <QMessageBox>

#include <QDir>

namespace Ui {
class CSCCleanerForm;
}

class CSCCleanerForm : public QWidget
{
    Q_OBJECT

public:
    explicit CSCCleanerForm(QWidget *parent = 0);
    ~CSCCleanerForm();

    QString getDefaultPath();
    void checkDefaultLogPath();

private slots:
    void on_browseDaemonLogPathButton_clicked();

    void on_searchButton_clicked();

    void on_deleteButton_clicked();

    void on_renameButton_clicked();

private:
    Ui::CSCCleanerForm *ui;
    QString daemonLogFileName = "daemon.log";
    QString cloudstationPath = "/AppData/Local/CloudStation/log";

    QFile daemonLogFile;

    QString daemonLogPathString;
};

#endif // CSCCLEANERFORM_H
