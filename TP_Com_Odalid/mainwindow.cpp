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
ReaderName MonLecteur;


void MainWindow::on_button_Connect_clicked()
{
    uint16_t status = 0;
    //MonLecteur.Type = ReaderTCP;
    //strcpy(MonLecteur.IPReader, pszHost);
    MonLecteur.Type = ReaderCDC;
    MonLecteur.device = 0;
    status = OpenCOM(&MonLecteur);
    qDebug() << "OpenCOM1" << status;

    char version[30];
    uint8_t serial[4];
    char stackReader[20];
    status = Version(&MonLecteur);
    ui->affichage->setText(MonLecteur.version);
    ui->affichage->update();


}

void MainWindow::on_button_Saisie_clicked()
{
    QString Text = ui->saisie->toPlainText();
    qDebug() << "Text : " << Text;
}

void MainWindow::on_button_Quitter_clicked()
{
    int16_t status = 0;
    RF_Power_Control(&MonLecteur, FALSE, 0);
    status = LEDBuzzer(&MonLecteur, LED_OFF);
    status = CloseCOM(&MonLecteur);
    qApp->quit();
}


