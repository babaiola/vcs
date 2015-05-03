#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <QImage>
#include"funzioni.h"
#include"mainwindow.h"


QImage IplImage2QImage(const IplImage *iplImage)
{
       int height = iplImage->height;
       int width = iplImage->width;
       const uchar *qImageBuffer =(const uchar*)iplImage->imageData;
       QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
       return img.rgbSwapped();
}

void save_up(QImage img_save, char buf_prod[100][100], int xx){
    //----- struttura tempo
    struct tm* newtime;
    time_t t;
    time(&t);
    newtime=localtime(&t);
    //--------------

    char buffer[1000];
    sprintf(buffer,"%d_%d_%d__%d_%d_%d.jpeg", newtime->tm_year-100, newtime->tm_mon, newtime->tm_mday, \
            newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
    img_save.save(buffer, "JPEG");
    char strina[100]="mv *.jpeg images/\"";
    strcat(strina, buf_prod[xx]);
    strcat(strina, "\"");
    strcat(strina, "/cam_1");
    system(strina);
}

void save_down(QImage img_save, char buf_prod[100][100], int xx){
    //----- struttura tempo
    struct tm* newtime;
    time_t t;
    time(&t);
    newtime=localtime(&t);
    //--------------

    char buffer[1000];
    sprintf(buffer,"%d_%d_%d__%d_%d_%d.jpeg", newtime->tm_year-100, newtime->tm_mon, newtime->tm_mday, \
            newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
    img_save.save(buffer, "JPEG");
    char strina[100]="mv *.jpeg images/\"";
    strcat(strina, buf_prod[xx]);
    strcat(strina, "\"");
    strcat(strina, "/cam_2");
    system(strina);
}

void cancella(QString stringa){     //cancella immagini
    char sttr[200]="rm -r \"";
    strcat(sttr, stringa.toAscii().data());
    strcat(sttr, "/.sens_list\"");
    system(sttr);
    QFile::remove(stringa);
    rmdir(stringa.toAscii().data());
}

void editx(QString stringa){
  const char* str_2 = stringa.toAscii().data();
    char str_1[300] = "krita \"";
    strcat(str_1, str_2);
    strcat(str_1, "\"");
     system(str_1);
    system("rm */*/*/*.???~");
}

void file(void){
    system("ls  images > sett/images_list"); //salva lista immagini
    system("ls   /dev/ttyUSB* > sett/serial_list");
}
////////////////////////////////////////////////////////////////////////////////////////////
void acquis_file(IplImage* ima_up[MAX_IMAGES], IplImage* ima_down[MAX_IMAGES], int* u, int* d, int* s, int* p,\
                 char buff_ser[10][30], char buff_prod[100][100], int* xx){
    char buff_up[100][100]; char buff_down[100][100]; int i;



    FILE *ser;
    ser = fopen("sett/serial_list", "r");
    if (ser)
    {
    while (!feof(ser))
    {
        fscanf(ser, "%s", buff_ser[s[0]++]);
    }
    fclose (ser);
    }
    else
        printf("errore durante l'' apertura del file.") ;


    FILE *prod;
    prod = fopen("sett/images_list", "r");
    if (prod)
    {
    while (!feof(prod))
    {
        fscanf(prod, "\n%[^\n]", buff_prod[p[0]++]);
    }
    fclose (prod);
    }
    else
        printf("errore durante l'' apertura del file.") ;



    char strs_1[100]="ls images/\"";
    strcat(strs_1, buff_prod[xx[0]]);
    strcat(strs_1, "\"/cam_1/*jpeg > sett/cam_1_list");
    system(strs_1);

    char strs_2[100]="ls images/\"";
    strcat(strs_2, buff_prod[xx[0]]);
    strcat(strs_2, "\"/cam_2/*jpeg > sett/cam_2_list");
    system(strs_2);

    FILE *upl;
    upl = fopen("sett/cam_1_list", "r");
if (upl)
{
while (!feof(upl))
{
    fscanf(upl, "\n%[^\n]", buff_up[u[0]++]);
}
fclose (upl);
}
else
    printf("errore durante l' apertura del file.") ;

FILE *downl;
downl = fopen("sett/cam_2_list", "r");
if (downl)
{
while (!feof(downl))
{
fscanf(downl, "\n%[^\n]", buff_down[d[0]++]);
}
fclose (downl);
}
else
printf("errore durante l' apertura del file.") ;

u[0]--; d[0]--; s[0]--; p[0]--;
for (i=0; i<u[0]; i++) ima_up[i] = cvLoadImage(buff_up[i], CV_LOAD_IMAGE_COLOR);
for (i=0; i<d[0]; i++) ima_down[i] = cvLoadImage(buff_down[i], CV_LOAD_IMAGE_COLOR);
}




void settings(int sett_list[10][1]){
    FILE *sett; int i;
    sett = fopen("sett/sett_list", "r");
    if (sett)
    {
        for(i=0; i<10; i++) fscanf(sett, "%d", sett_list[i]);
    fclose (sett);
    }
    else
        printf("errore durante l'' apertura del file.") ;
}


void put_settings(int sett_list[10][1]){
    FILE *sett2; int i;
    system("mv sett/sett_list sett/sett_list~");
    system("touch sett/sett_list");
    sett2 = fopen("sett/sett_list", "w");
    if (sett2)
    {
        for(i=0; i<10; i++) fprintf(sett2, "%d\n", sett_list[i][0]);
    fclose (sett2);
    }
    else
        printf("errore durante l'' apertura del file.") ;

}

void sens(char buff_prod[100][100], int* xx, int sens_list[12][1]){
    FILE *settu; int i; char stringo[100]="images/";
    strcat(stringo, buff_prod[xx[0]]);
    strcat(stringo, "/.sens_list");
    settu = fopen(stringo, "r");
    if (settu)
    {
        for(i=0; i<12; i++) fscanf(settu, "%d", sens_list[i]);
    fclose (settu);
    }
    else
        printf("errore durante l'' apertura del file.") ;
}

void put_sens(char buff_prod[100][100], int* xx, int sens_list[12][1]){
    FILE *settu2; int i; char stringo[100]="images/"; char toucho[100]="touch images/\"";
    strcat(stringo, buff_prod[xx[0]]);
    strcat(stringo, "/.sens_list");
    strcat(toucho, buff_prod[xx[0]]);
    strcat(toucho, "\"/.sens_list");
    system(toucho);
    settu2 = fopen(stringo, "w");
    if (settu2)
    {
        for(i=0; i<12; i++) fprintf(settu2, "%d\n", sens_list[i][0]);
    fclose (settu2);
    }
    else
        printf("errore durante l' apertura del file.") ;

}
void sum_rgb( IplImage* src, IplImage* dst , float* rosso, float* verde, float* blu, float* saturazione){

    // Allocate image planes
    IplImage* r = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
    IplImage* g = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
    IplImage* b = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );

    // Split image onto the color planes
    cvSplit( src, r, g, b, NULL );

    IplImage* s = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );

    // Add equally weighted rgb values
    cvAddWeighted( r, 1./rosso[0], g, 1./verde[0], 0.0, s );
    cvAddWeighted( s, 2./saturazione[0], b, 1./blu[0], 0.0, s );


    // Truncate values over 100
    cvThreshold( s, dst, 100, 1, CV_THRESH_TRUNC );

    cvReleaseImage( &r );
    cvReleaseImage( &g );
    cvReleaseImage( &b );
    cvReleaseImage( &s );
}

void pass(char password[100]){
    FILE *pass;
    pass = fopen("sett/pass", "r");
    if (pass)
    {
       fscanf(pass, "\n%[^\n]", password);
    fclose (pass);
    }
    else
        printf("errore durante l'' apertura del file.") ;
}
