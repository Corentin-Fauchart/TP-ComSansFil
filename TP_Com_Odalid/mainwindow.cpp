#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int card_read(BYTE sect_count);

uint8_t key_sec2Lec[6] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
uint8_t key_sec2Ecr[6] = { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5 };

uint8_t key_sec3Lec[6] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5 };
uint8_t key_sec3Ecr[6] = { 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5 };


char pszHost[] = "192.168.1.4";
BOOL bench = FALSE;
ReaderName MonLecteur;


void MainWindow::on_button_Connect_clicked()
{
    uint16_t status = 0;
    //MonLecteur.Type = ReaderTCP;
    //strcpy(MonLecteur.IPReader, pszHost);
    MonLecteur.Type = ReaderCDC;
    MonLecteur.device = 0;
    status = OpenCOM(&MonLecteur);
    if(status==0){
        ui->affichageCarte->update();
        ui->button_Connect->setEnabled(false);
        ui->button_Deconnexion->setEnabled(true);
        ui->button_Reset->setEnabled(true);
        ui->button_Lecture1->setEnabled(true);
    }
    qDebug() << "OpenCOM1" << status;

    RF_Power_Control(&MonLecteur, TRUE, 0);
    status = Mf_Classic_LoadKey(&MonLecteur, Auth_KeyA, key_sec2Lec, 2);
    status = Mf_Classic_LoadKey(&MonLecteur, Auth_KeyB, key_sec2Ecr, 2);
    status = Mf_Classic_LoadKey(&MonLecteur, Auth_KeyA, key_sec3Lec, 3);
    status = Mf_Classic_LoadKey(&MonLecteur, Auth_KeyB, key_sec3Ecr, 3);
    status = Version(&MonLecteur);
    ui->affichageCarte->setText(MonLecteur.version);


}


void MainWindow::on_button_Quitter_clicked()
{
    int16_t status = 0;
    RF_Power_Control(&MonLecteur, FALSE, 0);
    status = LEDBuzzer(&MonLecteur, LED_OFF);
    status = CloseCOM(&MonLecteur);
    qApp->quit();
}

int  MainWindow::card_read()
{
    uint8_t data[240] = {0};
    uint32_t dataMonnaie = 0;
    char data2[240] = {0};
    char data3[240] = {0};

    int16_t status = 0;
    uint8_t atq[2];
    uint8_t sak[1];
    uint8_t uid[12];
    uint16_t uid_len = 12;
    QString affichage="";

  status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);

  //identité
  status = Mf_Classic_Read_Block(&MonLecteur, TRUE, 9, data, Auth_KeyA, 2);
  strcpy(data2, (char*)data);
  ui->affichageInfo2->setText(data2);

  status = Mf_Classic_Read_Block(&MonLecteur, TRUE, 10, data, Auth_KeyA, 2);
  strcpy(data3, (char*)data);
  ui->affichageInfo3->setText(data3);

  status = Mf_Classic_Read_Value(&MonLecteur, TRUE, 14, &dataMonnaie, Auth_KeyA, 3);


  ui->affichageMonnaie->setText(QString::number((int32_t)dataMonnaie));

  //qDebug() <<(char*)data;
  if(!ui->button_Ecriture->isEnabled()){
      ui->button_Ecriture->setEnabled(true);
  }
  if(!ui->pushButton_Payer->isEnabled()){
      ui->pushButton_Payer->setEnabled(true);
  }
  if(!ui->pushButton_Charger->isEnabled()){
      ui->pushButton_Charger->setEnabled(true);
  }

}

int  MainWindow::card_write(){
    int16_t status = 0;
    uint8_t atq[2];
    uint8_t sak[1];
    uint8_t uid[12];
    uint16_t uid_len = 12;


  status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
  char DataIn[16];
  sprintf(DataIn, ui->affichageInfo2->toPlainText().toUtf8().data(), 16);
  status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 9, (uint8_t*)DataIn, Auth_KeyB, 2);

  sprintf(DataIn, ui->affichageInfo3->toPlainText().toUtf8().data(), 16);
  status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 10, (uint8_t*)DataIn, Auth_KeyB, 2);

  qDebug() << "Vous avez écris sur la carte !";



}

void MainWindow::on_button_Lecture1_clicked()
{
    card_read();
}



void MainWindow::on_button_Ecriture_clicked()
{
    card_write();
}

void MainWindow::on_button_Deconnexion_clicked()
{
    int16_t status = 0;
    RF_Power_Control(&MonLecteur, FALSE, 0);
    status = LEDBuzzer(&MonLecteur, LED_OFF);
    status = CloseCOM(&MonLecteur);
    ui->affichageInfo2->clear();
    ui->affichageInfo3->clear();
    ui->affichageMonnaie->clear();
}

void MainWindow::on_pushButton_Payer_clicked()
{
    changerValeurPM(PAYER);
}

void MainWindow::on_pushButton_Charger_clicked()
{
    changerValeurPM(CHARGER);
}

void MainWindow::changerValeurPM(bool choixAction)
{
    int16_t status = 0;
    uint8_t atq[2];
    uint8_t sak[1];
    uint8_t uid[12];
    uint16_t uid_len = 12;
    int valOpe = ui->spinBox_selectValue->text().toInt();
    int valMonnaie = ui->affichageMonnaie->toPlainText().toInt();

    status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
    if(choixAction)
    {
        status = Mf_Classic_Increment_Value(&MonLecteur,TRUE,14,valOpe,13,Auth_KeyB,3);
        status = Mf_Classic_Restore_Value(&MonLecteur,TRUE,13,14,Auth_KeyB,3);


    }else{
        status = Mf_Classic_Decrement_Value(&MonLecteur,TRUE,14,valOpe,13,Auth_KeyB,3);
        status = Mf_Classic_Restore_Value(&MonLecteur,TRUE,13,14,Auth_KeyB,3);

    }
    this->card_read();
}



void MainWindow::on_button_Reset_clicked()
{
  int16_t status = 0;
  uint8_t atq[2];
  uint8_t sak[1];
  uint8_t uid[12];
  uint16_t uid_len = 12;

  status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
  char DataIn[16];
  sprintf(DataIn, "Vincent", 16);
  status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 9, (uint8_t*)DataIn, Auth_KeyB, 2);

  sprintf(DataIn, "Thivent", 16);
  status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 10, (uint8_t*)DataIn, Auth_KeyB, 2);

  qDebug() << "Vous avez réinitialiser la carte !";

}
