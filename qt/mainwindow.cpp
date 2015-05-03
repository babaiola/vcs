#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"funzioni.h"
#include <QtGui>

int x_ris=640, y_ris=480;

//immagine grande scansione
QImage cam_up;
QRectF target_up(10, 10, 640/2.2, 480/2.2);
QRectF source_up;//(0.0, 0.0, x_ris, y_ris);
QImage cam_down;
QRectF target_down(302.0, 10, 640/2.2, 480/2.2);
QRectF source_down;//(0.0, 0.0, x_ris, y_ris);
//immagine anteprima
QImage img2;
QRectF target2(10.0, 231.0, 640/2.2, 480/2.2);
QRectF source2;//(0.0, 0.0, x_ris, y_ris);
QImage img4;
QRectF target4(302.0, 231.0, 640/2.2, 480/2.2);
QRectF source4;//(0.0, 0.0, x_ris, y_ris);

QImage img3;
QRectF target3(950.0, 210.0, 640/2.2, 480/2.2);
QRectF source3;//(0.0, 0.0, x_ris, y_ris);

//img save
QImage img_save_up;
QImage img_save_down;

CvCapture *captured_up;
CvCapture *captured_down;
int up=0, down=0;

IplImage* frame_up;// = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_8U,1);
IplImage* frame_down;// = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_8U,1);
IplImage* frame_upo;// = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_32F,1);
 IplImage* frame_downo;// = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_32F,1);

int time_up, time_down, time_uscita, time_pin, time_max;
float u_min; float d_min; float tol_min, tol_down; float u_min_temp, d_min_temp;
int sett_list[10][1], sens_list[12][1];
int scatti, tempo_scatti;

CvPoint minloc, maxloc;
double minval, maxval;
int img_width, img_height;
int tpl_width, tpl_height;
int res_width, res_height;

QString stringa; //stringa per funzione cancella elementi

IplImage* ima_up[MAX_IMAGES];
IplImage* ima_down[MAX_IMAGES];

int u=0, d=0; int s=0; int p=0; int xx=0; char buf_ser[10][30]; char buf_prod[100][100];
float r_1=5, v_1=5, b_1=5, s_1=5;
float r_2=5, v_2=5, b_2=5, s_2=5;

char password[30];

int controllo=1;

    char result[1];
MainWindow::MainWindow(QWidget *parent) : //main
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
   int i;

   system("mkdir sett");
   system("mkdir images");

    file(); //salva liste


    acquis_file(ima_up, ima_down, &u, &d, &s, &p, buf_ser, buf_prod, &xx);
    sens(buf_prod, &xx, sens_list);

    settings(sett_list);

    time_up = sett_list[0][0];
    time_down = sett_list[1][0];
    time_uscita=sett_list[2][0]; time_pin = sett_list[3][0];
    tol_min=(float)(sens_list[0][0])/100000;
    tol_down=(float)(sens_list[1][0])/100000;
    r_1=(float)(sens_list[2][0])/10; v_1=(float)(sens_list[3][0])/10;
    b_1=(float)(sens_list[4][0])/10; s_1=(float)(sens_list[5][0])/10;
    r_2=(float)(sens_list[6][0])/10; v_2=(float)(sens_list[7][0])/10;
    b_2=(float)(sens_list[8][0])/10; s_2=(float)(sens_list[9][0])/10;
    scatti =sens_list[10][0]; tempo_scatti =sens_list[11][0];

////////////////////////////////////////////////////////////////////////////////
    init_port(); //initiation of the port
    FILE* fp; int gg;
    fp = fopen("/dev/ttyUSB0", "r+");
    if (fp==NULL){
        printf("Non si può aprire %s\n", "/dev/ttyUSB0");
        exit(EXIT_FAILURE);
    }
    gg = getc(fp);
    if(gg==SERIAL_7) {
        fseek(fp, 0, SEEK_SET);
        fputc(0, fp);
    }
    fclose(fp);

////////////////////////////////////////////////////////////////////////////////////

    pass(password);

    ui->setupUi(this);

    x_ris=sett_list[8][0];
    y_ris=sett_list[9][0];
    if (x_ris==320) ui->riso->setCurrentIndex(0);
    else if (x_ris==640) ui->riso->setCurrentIndex(1);
    else if (x_ris==1024) ui->riso->setCurrentIndex(2);
    source_up = QRectF(0.0, 0.0, x_ris, y_ris);
    source_down = QRectF(0.0, 0.0, x_ris, y_ris);
    source2 = QRectF(0.0, 0.0, x_ris, y_ris);
    source4 = QRectF(0.0, 0.0, x_ris, y_ris);
    source3 = QRectF(0.0, 0.0, x_ris, y_ris);
    frame_up = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_8U,1);
    frame_down = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_8U,1);
    frame_upo = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_32F,1);
    frame_downo = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_32F,1);

    for(i=0; i<s; i++)
    ui->combotty->addItem(buf_ser[i]);

    for(i=0; i<p; i++){
        ui->sel_prodotti->addItem(buf_prod[i]);
    }

    ui->entrata_box->setValue(time_up/10);
    ui->entrata_down->setValue(time_down/10);
    ui->scarto_box->setValue(time_uscita/10);
    ui->espulsore_box->setValue(time_pin/10);
    ui->tol_minima->setValue(tol_min);
    ui->tol_down->setValue(tol_down);
    ui->rr_1->setValue(r_1);
    ui->vv_1->setValue(v_1);
    ui->bb_1->setValue(b_1);
    ui->ss_1->setValue(s_1);
    ui->rr_2->setValue(r_2);
    ui->vv_2->setValue(v_2);
    ui->bb_2->setValue(b_2);
    ui->ss_2->setValue(s_2);
    ui->catture->setValue(scatti);
    ui->riardo->setValue(tempo_scatti/10);

    ui->lcd_ok_par->display(sett_list[4][0]);
    ui->lcd_ok_tot->display(sett_list[5][0]);
    ui->lcd_ko_par->display(sett_list[6][0]);
    ui->lcd_ko_tot->display(sett_list[7][0]);


//-------------------------------
    //istruzioni albero file
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath("images");
    ui->listView->setModel(dirmodel);
    ui->listView->setRootIndex(dirmodel->index("images"));

    dirmodel_ok = new QFileSystemModel(this);
    dirmodel_ok->setRootPath("/mnt/floppy");
    ui->ok->setModel(dirmodel_ok);
    ui->ok->setRootIndex(dirmodel_ok->index("/mnt/floppy"));

    dirmodel_cam = new QFileSystemModel(this);
    dirmodel_cam->setRootPath("/mnt/floppy");
    ui->cam->setModel(dirmodel_cam);
    ui->cam->setRootIndex(dirmodel_cam->index("/mnt/floppy"));


    ui->nuovo_lab->hide();
    ui->canc_nome->hide();
    ui->invio_nome->hide();
    ui->lab_1->hide();
    ui->lab_2->hide();
    ui->cam_pass_line->hide();
    ui->new_pass->hide();
    ui->ok_pass->hide();
    ui->canc_pass->hide();
    ui->lab_pass->hide();
    ui->lab_new_pass->hide();

    ui->semaforo->setPalette(QPalette(QColor(170, 170, 255)));

    ui->crediti_s->hide();

    QMainWindow::showFullScreen(); //fullscreen
}

MainWindow::~MainWindow(){
    put_settings(sett_list);
    file(); //chiusura salva file nuovamente
    pin13_off(); pin12_off(); pin11_off();
    port->close(); //we close the port at the end of the program
      cvReleaseImage(&frame_up);
      cvReleaseImage(&frame_down);
      int i;
      for (i=0; i<MAX_IMAGES; i++){
          cvReleaseImage(&ima_up[i]);
          cvReleaseImage(&ima_down[i]);
      }
      if (up) cvReleaseCapture(&captured_up);
      if (down) cvReleaseCapture(&captured_down);
    delete ui;
    delete dirmodel;
     delete dirmodel_ok;
      delete dirmodel_cam;
}

void MainWindow::confronto_cv(void){
  u_min = 1;

            int i;
IplImage* frame_c = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_32F,1);

            frame_c = cvCloneImage(frame_upo);
    //    sum_rgb(frame_up, frame_c);

            img_width = frame_c->width;          /////riconoscimento
            img_height = frame_c->height;

            IplImage *tes[u];

        for(i=0; i<u; i++){
        tpl_width = ima_up[i]->width;
        tpl_height = ima_up[i]->height;
        res_width = img_width - tpl_width + 1;
        res_height = img_height - tpl_height + 1;

        tes[i] = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );

        cvMatchTemplate( frame_c, ima_up[i], tes[i], CV_TM_SQDIFF_NORMED );


        cvMinMaxLoc( tes[i], &minval, &maxval, &minloc, &maxloc, 0 );

        if(u_min > minval) u_min = minval;

        cvReleaseImage( &tes[i] );

        }

        cvRectangle( frame_c,
        cvPoint( minloc.x, minloc.y ),
        cvPoint( minloc.x + tpl_width, minloc.y + tpl_height ),
        cvScalar( 0, 255, 0, 0 ), 3, 0, 0 );

              img2 = IplImage2QImage(frame_c);
                    cvReleaseImage(&frame_c);
        ui->lcd_min_ok->display(u_min);
        if(u_min_temp<u_min) u_min_temp=u_min;
            u_min=u_min_temp;

}
void MainWindow::confronto_down(void){
  d_min = 1;

    int i;
IplImage* frame_c = cvCreateImage(cvSize( x_ris, y_ris ),IPL_DEPTH_32F,1);
    frame_c = cvCloneImage(frame_downo);
    img_width = frame_c->width;          /////riconoscimento
    img_height = frame_c->height;

    IplImage *tes[d];

for(i=0; i<d; i++){
tpl_width = ima_down[i]->width;
tpl_height = ima_down[i]->height;
res_width = img_width - tpl_width + 1;
res_height = img_height - tpl_height + 1;

tes[i] = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );

cvMatchTemplate( frame_c, ima_down[i], tes[i], CV_TM_SQDIFF_NORMED );



cvMinMaxLoc( tes[i], &minval, &maxval, &minloc, &maxloc, 0 );

if(d_min > minval) d_min = minval;

cvReleaseImage( &tes[i] );



}


cvRectangle( frame_c,
cvPoint( minloc.x, minloc.y ),
cvPoint( minloc.x + tpl_width, minloc.y + tpl_height ),
cvScalar( 0, 255, 0, 0 ), 3, 0, 0 );
    img4 = IplImage2QImage(frame_c);
          cvReleaseImage(&frame_c);

    ui->lcd_min_down->display(d_min);
    if(d_min_temp<d_min) d_min_temp=d_min;
        d_min=d_min_temp;

}


void MainWindow::paintEvent(QPaintEvent *){ //le due immagini sul video


    if (up) {
        frame_upo = cvQueryFrame(captured_up);
        sum_rgb(frame_upo, frame_up, &r_1, &v_1, &b_1, &s_1);
    }
    if (down){
        frame_downo = cvQueryFrame(captured_down);
        sum_rgb(frame_downo, frame_down, &r_2, &v_2, &b_2, &s_2);
    }

    if (up) cvCvtColor(frame_up,frame_upo,CV_GRAY2RGB);
    if (down) cvCvtColor(frame_down,frame_downo,CV_GRAY2RGB);

    if (up)    img_save_up = IplImage2QImage(frame_upo);
    if (down) img_save_down = IplImage2QImage(frame_downo);

     pin_7_in();



     if (up)  cam_up = IplImage2QImage(frame_upo);
        if(down) cam_down = IplImage2QImage(frame_downo);


    update();

    QPainter painter(this);                           //grande
    painter.drawImage(target_up, cam_up, source_up);
    painter.drawImage(target_down, cam_down, source_down);
    painter.drawImage(target2, img2, source2);
    painter.drawImage(target4, img4, source4);
    painter.drawImage(target3, img3, source3);



}

void MainWindow::on_spegni_clicked()      //pulsante chiusura //da aggiungere spegnimento
{
    close();
    //system("poweroff");
}

void MainWindow::on_cattura_clicked()       //pulsante acquisizione immagini //da vedere con seriale
{
    if (up){
        save_up(img_save_up, buf_prod, xx);
        img2 = img_save_up;
    }
    if (down) {
        save_down(img_save_down, buf_prod, xx);
        img4 = img_save_down;
    }
    update();
}


void MainWindow::on_listView_pressed(const QModelIndex &index){  //immagini selezionate
    QString sstring = dirmodel->fileInfo(index).absoluteFilePath();
    ui->ok->setRootIndex(dirmodel_ok->index(sstring));
    stringa=sstring;
    QImage imag("images/load");
    img3 = imag;
    update();
}

void MainWindow::on_ok_pressed(const QModelIndex &index){  //immagini selezionate
    QString sstring = dirmodel->fileInfo(index).absoluteFilePath();
    ui->cam->setRootIndex(dirmodel_cam->index(sstring));
    stringa=sstring;
    QImage imag("images/load");
    img3 = imag;
    update();
}


void MainWindow::on_cam_pressed(const QModelIndex &index){  //immagini selezionate
    QString sstring = dirmodel_cam->fileInfo(index).absoluteFilePath();
    QImage imag(sstring);
    img3 = imag;
    stringa=sstring;
    update();
}

void MainWindow::on_cancella_clicked()  //pulsante cancella cliccato
{
    int i;
    cancella(stringa);
    put_sens(buf_prod, &xx, sens_list);
  update();
  QImage imag("images/load");
  img3 = imag;
  u=0; d=0; p=0;
  file();
  acquis_file(ima_up, ima_down, &u, &d, &s, &p, buf_ser, buf_prod, &xx);

  ui->sel_prodotti->clear();
  for(i=0; i<p; i++)  ui->sel_prodotti->addItem(buf_prod[i]);

}

void MainWindow::on_tabWidget_currentChanged() //cambio tab da immagini
{
    QImage imag("images/load");
    img3 = imag;
}


void MainWindow::on_edita_clicked()
{
editx(stringa);
u=0; d=0; p=0;
acquis_file(ima_up, ima_down, &u, &d, &s, &p, buf_ser, buf_prod, &xx);
}



void MainWindow::on_segnale_clicked()
{
    if (controllo == 1){
    controllo=0;
    u_min_temp=0; d_min_temp=0;
    if (!down) d_min = 0; if (!up) u_min = 0;
    QTimer::singleShot(0, this, SLOT(segnals()));
    char x[1] ={8}, y[1] ={9}, z[1]={27};
    port->write(x); port->write(y); port->write(z);
    ui->semaforo->setPalette(QColor(170, 170,255));
    ui->led_1->setPalette(QColor(170, 170,255));
    ui->led_2->setPalette(QColor(170, 170,255));
    }
}

void MainWindow::segnals(){
    int i=0;
    for(i=0; i<scatti; i++){
    if (up) QTimer::singleShot(time_up+i*tempo_scatti, this, SLOT(confronto_cv()));
     if (down) QTimer::singleShot(time_down+i*tempo_scatti, this, SLOT(confronto_down()));
    }
    if (up){
        QTimer::singleShot(time_up, this, SLOT(led_cam_1_on()));
    QTimer::singleShot(time_up+2000+tempo_scatti*(scatti-1), this, SLOT(led_cam_1_off()));}
    if(down){
        QTimer::singleShot(time_down, this, SLOT(led_cam_2_on()));
        QTimer::singleShot(time_down+2000+tempo_scatti*(scatti-1), this, SLOT(led_cam_2_off()));}

    if (time_up>time_down) time_max = time_up+tempo_scatti*(scatti-1);
    else time_max = time_down+tempo_scatti*(scatti-1);

    QTimer::singleShot(time_max+time_uscita, this, SLOT(pin13_on()));
         QTimer::singleShot(time_max+time_uscita + time_pin, this, SLOT(pin13_off()));
         QTimer::singleShot(time_max, this, SLOT(pin11_on()));
        QTimer::singleShot(time_max, this, SLOT(pin12_on()));
        QTimer::singleShot(time_max, this, SLOT(led_scarto_on()));
        QTimer::singleShot(time_max+time_uscita, this, SLOT(led_scarto_off()));
}

void MainWindow::on_entrata_box_valueChanged(int arg1)
{
    time_up = arg1*10;
    sett_list[0][0] = time_up;
}

void MainWindow::on_entrata_down_valueChanged(int arg1)
{
    time_down = arg1*10;
    sett_list[1][0] = time_down;
}


void MainWindow::on_scarto_box_valueChanged(int arg1)
{
     time_uscita = arg1*10;
     sett_list[2][0] = time_uscita;
}

void MainWindow::on_espulsore_box_valueChanged(int arg1)
{
   time_pin = arg1*10;
   sett_list[3][0] = time_pin;
}


void MainWindow::on_tol_minima_valueChanged(double arg1)
{
   tol_min = arg1;
   sens_list[0][0]=tol_min*100000;
   put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_tol_down_valueChanged(double arg1)
{
   tol_down = arg1;
   sens_list[1][0]=tol_down*100000;
   put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::init_port()
{
    port = new QextSerialPort("/dev/ttyUSB0"); //we create the port

    port->open(QIODevice::ReadWrite | QIODevice::Unbuffered); //we open the port
    if(!port->isOpen())
    {
        QMessageBox::warning(this, "Errore", "Impossibile aprire la porta!");
    }
    //we set the port properties
    port->setBaudRate(BAUD9600);//modify the port settings on your own
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

}
void MainWindow::pin13_on(void){
    if((u_min>=tol_min) || (d_min>=tol_down)){
        led_espulsione_on();
    char x[1] ={26};
    port->write(x);
    }
}
void MainWindow::pin13_off(void){
    if((u_min>=tol_min) || (d_min>=tol_down)){
        led_espulsione_off();
    char x[1] ={10};
    port->write(x);
    }
}
void MainWindow::pin12_on(void){
    char p[1] ={11};
    port->write(p);
    controllo=1;
    if((u_min<tol_min) && (d_min<tol_down)){
    char x[1] ={24};
    port->write(x);
    ui->semaforo->setPalette(QColor(85, 255,0));
    (sett_list[4][0])++; (sett_list[5][0])++;
    ui->lcd_ok_par->display(sett_list[4][0]);
    ui->lcd_ok_tot->display(sett_list[5][0]);
    }
    ui->lcd_min_ok->display(u_min);
    ui->lcd_min_down->display(d_min);
}
void MainWindow::pin12_off(void){
    if((u_min<tol_min) && (d_min<tol_down)){
    char x[1] ={8};
    port->write(x);
    }
}
void MainWindow::pin11_on(void){
    if (u_min<tol_min) ui->led_1->setPalette(QColor(85, 255, 0));
    else ui->led_1->setPalette(QColor(255, 0, 0));
    if (d_min<tol_down) ui->led_2->setPalette(QColor(85, 255, 0));
    else ui->led_2->setPalette(QColor(255, 0, 0));
    if((u_min>=tol_min) || (d_min>=tol_down)){
    char x[1] ={25};
    port->write(x);
    ui->semaforo->setPalette(QColor(255, 0, 0));
    (sett_list[6][0])++; (sett_list[7][0])++;
    ui->lcd_ko_par->display(sett_list[6][0]);
    ui->lcd_ko_tot->display(sett_list[7][0]);
    }
}
void MainWindow::pin11_off(void){
    if((u_min>=tol_min) || (d_min>=tol_down)){
    char x[1] ={9};
    port->write(x);
    }
}

void MainWindow::pin_7_in(void){
    FILE* fp2; int gg;
    fp2 = fopen("/dev/ttyUSB0", "r+");
    if (fp2==NULL){
        printf("Non si può aprire %s\n", "/dev/ttyUSB0");
    }
    gg = getc(fp2);
    if(gg==SERIAL_7) {
        if (controllo == 1){
        led_ftc_on();
        QTimer::singleShot(2000, this, SLOT(led_ftc_off()));
        on_segnale_clicked();
        }
        fseek(fp2, 0, SEEK_SET);
        fputc(0, fp2);
    }
    fclose(fp2);

}

void MainWindow::on_ok_tot_butt_clicked()
{
    sett_list[4][0]=0;
    ui->lcd_ok_par->display(sett_list[4][0]);
}

void MainWindow::on_ko_tot_butt_clicked()
{
    sett_list[6][0]=0;
    ui->lcd_ko_par->display(sett_list[6][0]);
}

void MainWindow::on_combotty_currentIndexChanged(const QString &arg1)
{
    pin13_off(); pin12_off(); pin11_off();
    port->close(); //we close the port at the end of the program

    port = new QextSerialPort(arg1); //we create the port

    port->open(QIODevice::ReadWrite | QIODevice::Unbuffered); //we open the port
    if(!port->isOpen())
    {
        QMessageBox::warning(this, "Errore", "Impossibile aprire la porta USB!");
    }
    //we set the port properties
    port->setBaudRate(BAUD9600);//modify the port settings on your own
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
}

void MainWindow::on_frame_pass_textEdited(const QString &arg1)
{
        if (arg1 == password) ui->frame->hide();
}

void MainWindow::on_logout_clicked()
{
ui->tabWidget->setCurrentIndex(0);
ui->frame_pass->clear();
ui->frame->show();
}

void MainWindow::on_sel_prodotti_currentIndexChanged(int index)
{
    xx=index; u=0; d=0; p=0;
    acquis_file(ima_up, ima_down, &u, &d, &s, &p, buf_ser, buf_prod, &xx);
    sens(buf_prod, &xx, sens_list);
    tol_min=(float)(sens_list[0][0])/100000;
    tol_down=(float)(sens_list[1][0])/100000;
    r_1=(float)(sens_list[2][0])/10;
    v_1=(float)(sens_list[3][0])/10;
    b_1=(float)(sens_list[4][0])/10;
    s_1=(float)(sens_list[5][0])/10;
    r_2=(float)(sens_list[6][0])/10;
    v_2=(float)(sens_list[7][0])/10;
    b_2=(float)(sens_list[8][0])/10;
    s_2=(float)(sens_list[9][0])/10;
    scatti=(sens_list[10][0]);
    tempo_scatti=(sens_list[11][0]);
    ui->tol_minima->setValue(tol_min);
    ui->tol_down->setValue(tol_down);
    ui->rr_1->setValue(r_1);
    ui->vv_1->setValue(v_1);
    ui->bb_1->setValue(b_1);
    ui->ss_1->setValue(s_1);
    ui->rr_2->setValue(r_2);
    ui->vv_2->setValue(v_2);
    ui->bb_2->setValue(b_2);
    ui->ss_2->setValue(s_2);
}

void MainWindow::on_nuovo_clicked()
{
    ui->nuovo_lab->show();
    ui->nuovo->hide();
    ui->canc_nome->show();
    ui->invio_nome->show();
}

void MainWindow::on_nuovo_lab_returnPressed()
{
    int i;
    char frf[100]="mkdir images/\""; char dede[100]="";
    strcat(frf, ui->nuovo_lab->text().toAscii().data());
    strcat(frf, "\"");
    system(frf);
    strcpy(dede, frf);
    strcat(frf, "/cam_1");
    strcat(dede, "/cam_2");
    system(frf);
    system(dede);
    file();
    u=0; d=0;p=0;
    acquis_file(ima_up, ima_down, &u, &d, &s, &p, buf_ser, buf_prod, &xx);
    ui->sel_prodotti->clear();
    for(i=0; i<p; i++)  ui->sel_prodotti->addItem(buf_prod[i]);
    sens(buf_prod, &xx, sens_list);
    ui->nuovo_lab->hide();
    ui->nuovo_lab->clear();
    ui->nuovo->show();
    ui->sel_prodotti->clear();
    for(i=0; i<p; i++)  ui->sel_prodotti->addItem(buf_prod[i]);
    ui->canc_nome->hide();
    ui->invio_nome->hide();
}

void MainWindow::on_invio_nome_clicked()
{
    int i;
    char frf[100]="mkdir images/\""; char dede[100]="";
    strcat(frf, ui->nuovo_lab->text().toAscii().data());
    strcat(frf, "\"");
    system(frf);
    strcpy(dede, frf);
    strcat(frf, "/cam_1");
    strcat(dede, "/cam_2");
    system(frf);
    system(dede);
    file();
    u=0; d=0; p=0;
    acquis_file(ima_up, ima_down, &u, &d, &s, &p, buf_ser, buf_prod, &xx);
    ui->sel_prodotti->clear();
    for(i=0; i<p; i++)  ui->sel_prodotti->addItem(buf_prod[i]);
    sens(buf_prod, &xx, sens_list);
    ui->nuovo_lab->hide();
    ui->nuovo_lab->clear();
    ui->nuovo->show();
    ui->sel_prodotti->clear();
    for(i=0; i<p; i++)  ui->sel_prodotti->addItem(buf_prod[i]);
    ui->canc_nome->hide();
    ui->invio_nome->hide();
}

void MainWindow::on_canc_nome_clicked()
{
    ui->nuovo_lab->clear();
    ui->nuovo_lab->hide();
    ui->nuovo->show();
    ui->canc_nome->hide();
    ui->invio_nome->hide();
}

void MainWindow::on_cam_1_toggled(bool checked)
{
    if (checked){
    if ((captured_up = cvCaptureFromCAM( 0 ))){
    cvSetCaptureProperty( captured_up, CV_CAP_PROP_FRAME_WIDTH, x_ris );
    cvSetCaptureProperty( captured_up, CV_CAP_PROP_FRAME_HEIGHT, y_ris );
    //cvSetCaptureProperty( captured_up, CV_CAP_PROP_FPS, 30 );
    //cvSetCaptureProperty( captured_up, CV_CAP_PROP_EXPOSURE, 1/30 );
    up=1;
    ui->lab_1->show();
    ui->lab_1_o->hide();} else {
        QMessageBox::warning(this, "Errore Camera", "Impossibile Aprire!");
        ui->cam_1->setChecked(0);
    }
    } else {
        cvReleaseCapture(&captured_up);
        up=0;
        ui->lab_1->hide();
        ui->lab_1_o->show();
    }
}

void MainWindow::on_cam_2_toggled(bool checked)
{
    if (checked){
    if ((captured_down = cvCaptureFromCAM( 1 ))){
    cvSetCaptureProperty( captured_down, CV_CAP_PROP_FRAME_WIDTH, x_ris );
    cvSetCaptureProperty( captured_down, CV_CAP_PROP_FRAME_HEIGHT, y_ris );
   // cvSetCaptureProperty( captured_down, CV_CAP_PROP_EXPOSURE, 1/100 );
    //cvSetCaptureProperty( captured_down, CV_CAP_PROP_FPS, 1 );
    down=1;
    ui->lab_2->show();
    ui->lab_2_o->hide();} else {
        QMessageBox::warning(this, "Errore Camera", "Impossibile Aprire!");
        ui->cam_2->setChecked(0);
    }
    } else {
        cvReleaseCapture(&captured_down);
        down=0;
        ui->lab_2->hide();
        ui->lab_2_o->show();
    }
}

void MainWindow::on_crediti_pressed()
{
        ui->crediti_s->show();
}

void MainWindow::on_crediti_released()
{
    QTimer::singleShot(6000, this, SLOT(rilascio_crediti()));


}

void MainWindow::rilascio_crediti(){
    ui->crediti_s->hide();
}

void MainWindow::led_cam_1_on(){
    ui->led_cam_1->setPalette(QColor(85, 255, 0));
}
void MainWindow::led_cam_1_off(){
    ui->led_cam_1->setPalette(QColor(170, 170, 255));
}
void MainWindow::led_cam_2_on(){
    ui->led_cam_2->setPalette(QColor(85, 255, 0));
}
void MainWindow::led_cam_2_off(){
    ui->led_cam_2->setPalette(QColor(170, 170, 255));
}
void MainWindow::led_scarto_on(){
    ui->led_scarto->setPalette(QColor(85, 255, 0));
}
void MainWindow::led_scarto_off(){
    ui->led_scarto->setPalette(QColor(170, 170, 255));
}
void MainWindow::led_espulsione_on(){
    ui->led_espulsione->setPalette(QColor(255, 0, 0));
}
void MainWindow::led_espulsione_off(){
    ui->led_espulsione->setPalette(QColor(170, 170, 255));
}
void MainWindow::led_ftc_on(){
    ui->led_ftc->setPalette(QColor(85, 255, 0));
}
void MainWindow::led_ftc_off(){
    ui->led_ftc->setPalette(QColor(170, 170, 255));
}

void MainWindow::on_rr_1_valueChanged(double arg1)
{
    r_1 = arg1;
    sens_list[2][0]=r_1*10;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_vv_1_valueChanged(double arg1)
{
    v_1 = arg1;
    sens_list[3][0]=v_1*10;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_bb_1_valueChanged(double arg1)
{
    b_1 = arg1;
    sens_list[4][0]=b_1*10;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_ss_1_valueChanged(double arg1)
{
    s_1 = arg1;
    sens_list[5][0]=s_1*10;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_rr_2_valueChanged(double arg1)
{
    r_2 = arg1;
    sens_list[6][0]=r_2*10;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_vv_2_valueChanged(double arg1)
{
    v_2 = arg1;
    sens_list[7][0]=v_2*10;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_bb_2_valueChanged(double arg1)
{
    b_2 = arg1;
    sens_list[8][0]=b_2*10;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_ss_2_valueChanged(double arg1)
{
    s_2 = arg1;
    sens_list[9][0]=s_2*10;
    put_sens(buf_prod, &xx, sens_list);
}



void MainWindow::on_catture_valueChanged(int arg1)
{
    scatti= arg1;
    sens_list[10][0]=scatti;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_riardo_valueChanged(int arg1)
{
    tempo_scatti= arg1*10;
    sens_list[11][0]=tempo_scatti;
    put_sens(buf_prod, &xx, sens_list);
}

void MainWindow::on_riav_progr_clicked()
{
    if (up) ui->cam_1->setChecked(false);
    if (down) ui->cam_2->setChecked(false);
    close();
    system("./load");
   // system("reboot");
}

void MainWindow::on_cambia_pass_clicked()
{
    ui->cambia_pass->hide();
    ui->cam_pass_line->show();
    ui->lab_pass->show();
}

void MainWindow::on_cam_pass_line_textEdited(const QString &arg1)
{
    if(arg1==password){
        ui->cam_pass_line->clear();
        ui->cam_pass_line->hide();
        ui->new_pass->show();
        ui->lab_pass->hide();
        ui->lab_new_pass->show();
        ui->ok_pass->show();
        ui->canc_pass->show();
    }
}

void MainWindow::on_new_pass_returnPressed()
{
    char sder[100]="echo \"";
    strcat(sder, ui->new_pass->text().toAscii().data());
    strcat(sder, "\" > sett/pass");
    system("rm sett/pass");
    system(sder);
    pass(password);
    ui->new_pass->clear();
    ui->new_pass->hide();
    ui->cambia_pass->show();
    ui->ok_pass->hide();
    ui->canc_pass->hide();
    ui->lab_new_pass->hide();
}

void MainWindow::on_ok_pass_clicked()
{
    char sder[100]="echo \"";
    strcat(sder, ui->new_pass->text().toAscii().data());
    strcat(sder, "\" > sett/pass");
    system("rm sett/pass");
    system(sder);
    pass(password);
    ui->new_pass->clear();
    ui->new_pass->hide();
    ui->cambia_pass->show();
    ui->ok_pass->hide();
    ui->canc_pass->hide();
    ui->lab_new_pass->hide();
}

void MainWindow::on_canc_pass_clicked()
{
    ui->cambia_pass->show();
    ui->cam_pass_line->clear();
    ui->cam_pass_line->hide();
    ui->new_pass->clear();
    ui->new_pass->hide();
    ui->ok_pass->hide();
    ui->canc_pass->hide();
    ui->lab_new_pass->hide();
    ui->lab_pass->hide();
}

void MainWindow::on_riso_currentIndexChanged(int index)
{
    if(index==0){
        sett_list[8][0]=320;
        sett_list[9][0]=240;
    } else if(index==1){
        sett_list[8][0]=640;
        sett_list[9][0]=480;
    } else if(index==2){
        sett_list[8][0]=1024;
        sett_list[9][0]=768;
    }
}
