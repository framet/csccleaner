#include "csccleanerform.h"
#include "ui_csccleanerform.h"

CSCCleanerForm::CSCCleanerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSCCleanerForm)
{
    ui->setupUi(this);

    checkDefaultLogPath();

}

CSCCleanerForm::~CSCCleanerForm()
{
    delete ui;
}

QString CSCCleanerForm::getDefaultPath()
{

    QDir desiredFolder = QDir(QDir::homePath() + cloudstationPath);
    if(desiredFolder.exists()){
        return desiredFolder.path();
    }else{
        return QDir::homePath();
    }

}

void CSCCleanerForm::checkDefaultLogPath()
{
    QDir desiredFolder = QDir(QDir::homePath() + cloudstationPath);
    if(desiredFolder.exists(daemonLogFileName)){
        daemonLogPathString = desiredFolder.absoluteFilePath(daemonLogPathString)+"/"+daemonLogFileName;
        ui->daemonLogPathLineEdit->setText(daemonLogPathString);
    }
}

void CSCCleanerForm::on_browseDaemonLogPathButton_clicked(){

    qDebug() << getDefaultPath();
    daemonLogPathString = QFileDialog::getOpenFileName(this,
                                                       tr("Open daemon.log"),
                                                       getDefaultPath(),
                                                       tr("daemon.log"));

    ui->daemonLogPathLineEdit->setText(daemonLogPathString);
}

void CSCCleanerForm::on_searchButton_clicked()
{
    //open file of not open in read only mode
    if(!daemonLogFile.isOpen()){
        daemonLogFile.setFileName(daemonLogPathString);
        daemonLogFile.open(QIODevice::ReadOnly);
    }

    //remove all items in listWidget to start fresh
    for(int i = 0; i < ui->listWidget->count();i++){
        ui->listWidget->takeItem(i);
    }

    //setup textStream to go through each line
    QTextStream in(&daemonLogFile);
    quint32 lineCounter = 0;
    quint32 foundProblematicFileCounter = 0;
    while (!in.atEnd())
    {
        QString line = in.readLine();

        //check if the line contains the corresponding error to the problematic file
        if(line.contains("[ERROR]") && line.contains("Unable to handle locked/no permission file")){

            //cut out the unecessary text so we only have the path+filename
            QStringList list = line.split("'");
            if(list.size() > 1){
                QString problematicFile = list.at(list.size()-2);


                //                QListWidget* list = ui->listWidget;
                //                if(list->findItems(problematicFile,Qt::MatchFlag::MatchExactly).size()==0){
                ui->listWidget->addItem(problematicFile);
                foundProblematicFileCounter++;
                //                }
            }
        }

        lineCounter++;
    }
    daemonLogFile.close();

    if(foundProblematicFileCounter == 0){
        QMessageBox msgBox;
        msgBox.setText("No problematic file has been found in the daemon.log.\nCheck if CSC is paused.");
        msgBox.exec();
    }
}

void CSCCleanerForm::on_deleteButton_clicked()
{

    //retreive filename from list
    QString fileToDelete = ui->listWidget->currentItem()->text();

    //replace double backslashes with one forward slash since CSC daemon.log can contain double backslash
    fileToDelete = fileToDelete.replace("\\","/");


    //open the file which we want to rename
    QFile file(fileToDelete);
    //since the problem is the read-only-flag we have to overwrite the permission so we can delete or rename it
    file.setPermissions(QFile::ReadOther | QFile::WriteOther);


    if(file.remove()){
        //remove was successfull and we take out the item out of the list;
        ui->listWidget->takeItem( ui->listWidget->currentRow());
    }else{
        //remove was not successfull and we mark it red
        ui->listWidget->currentItem()->setTextColor(Qt::red);
        //jump down one row to go through list more convenient
        ui->listWidget->setCurrentRow( ui->listWidget->currentRow()+1);
    }
}

void CSCCleanerForm::on_renameButton_clicked()
{
    //retreive filename from list
    QString fileToRename = ui->listWidget->currentItem()->text();

    //replace double backslashes with one forward slash since CSC daemon.log can contain double backslash
    fileToRename = fileToRename.replace("\\","/");

    //extract the path and name of the file
    QString fileName = "";
    QString filePath = "";
    QStringList list = fileToRename.split("/");
    filePath = list.first();
    for(int i = 1; i < list.size()-1;i++){
        filePath += "/"+list.at(i);
    }
    fileName = list.last();

    //add an underscore infront of the old file name
    QString newFileName = filePath + "/_" + fileName;


    //open the file which we want to rename
    QFile file(fileToRename);

    //since the problem is the read-only-flag we have to overwrite the permission so we can delete or rename it
    file.setPermissions(QFile::ReadOther | QFile::WriteOther);


    if(file.rename(newFileName)){
        //rename was successfull and we take out the item out of the list;
        ui->listWidget->takeItem( ui->listWidget->currentRow());
    }else{
        //rename was not successfull and we mark it blue
        ui->listWidget->currentItem()->setTextColor(Qt::blue);
        //jump down one row to go through list more convenient
        if(ui->listWidget->currentRow()+1 < ui->listWidget->count()){
            ui->listWidget->setCurrentRow( ui->listWidget->currentRow()+1);
        }
    }
}
