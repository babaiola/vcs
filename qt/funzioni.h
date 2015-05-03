#ifndef FUNZIONI_H
#define FUNZIONI_H
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <QtGui/QApplication>
#include<QtGui>

void cancella(QString stringa); //prototipo funzione cancella elementi
QImage IplImage2QImage(const IplImage *iplImage);
void save_up(QImage img_save, char buf_prod[100][100], int xx);
void save_down(QImage img_save, char buf_prod[100][100], int xx);
void editx(QString stringa);
void file(void);
void acquis_file(IplImage* ima_up[100], IplImage* ima_down[100], int* u, int* d, int* s, int* p, char buff_ser[10][30], char buff_prod[100][100], int* xx);
void settings(int sett_list[8][1]);
void put_settings(int sett_list[8][1]);
void sens(char buff_prod[100][100], int* xx, int sens_list[10][1]);
void put_sens(char buff_prod[100][100], int* xx, int sens_list[10][1]);
void sum_rgb( IplImage* src, IplImage* dst , float* rosso, float* verde, float* blu, float* saturazione);
void pass(char password[100]);
#endif // FUNZIONI_H

