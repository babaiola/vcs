#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define SERIAL_7 2

#define MAX_IMAGES 100


#include <QMainWindow>
#include<QtCore>
#include<QtGui>
#include<qextserialport/src/qextserialport.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init_port(); // port initialisation function

private slots:
    void on_spegni_clicked();

    void on_cattura_clicked();

    void on_listView_pressed(const QModelIndex &index);

    void on_ok_pressed(const QModelIndex &index);

    void on_cam_pressed(const QModelIndex &index);

    void on_cancella_clicked();

    void on_tabWidget_currentChanged();

    void on_edita_clicked();


    void on_segnale_clicked();

    void segnals();

    void on_entrata_box_valueChanged(int arg1);

    void on_entrata_down_valueChanged(int arg1);

    void pin13_on(void);

    void pin13_off(void);

    void pin12_on(void);

    void pin12_off(void);

    void pin11_on(void);

    void pin11_off(void);

    void on_scarto_box_valueChanged(int arg1);

    void on_espulsore_box_valueChanged(int arg1);

    void pin_7_in(void);

    void on_ok_tot_butt_clicked();

    void on_ko_tot_butt_clicked();

    void on_combotty_currentIndexChanged(const QString &arg1);

    void confronto_cv();

    void confronto_down();

    void on_tol_minima_valueChanged(double arg1);

    void on_tol_down_valueChanged(double arg1);

    void on_frame_pass_textEdited(const QString &arg1);

    void on_logout_clicked();

    void on_sel_prodotti_currentIndexChanged(int index);

    void on_nuovo_clicked();

    void on_nuovo_lab_returnPressed();

    void on_invio_nome_clicked();

    void on_canc_nome_clicked();

    void on_cam_1_toggled(bool checked);

    void on_cam_2_toggled(bool checked);

    void on_crediti_pressed();

    void on_crediti_released();

    void rilascio_crediti();

    void led_cam_1_on();

    void led_cam_1_off();

    void led_cam_2_on();

    void led_cam_2_off();

    void led_scarto_on();

    void led_scarto_off();

    void led_espulsione_on();

    void led_espulsione_off();

    void led_ftc_on();

    void led_ftc_off();

    void on_rr_1_valueChanged(double arg1);

    void on_vv_1_valueChanged(double arg1);

    void on_bb_1_valueChanged(double arg1);

    void on_ss_1_valueChanged(double arg1);

    void on_rr_2_valueChanged(double arg1);

    void on_vv_2_valueChanged(double arg1);

    void on_bb_2_valueChanged(double arg1);

    void on_ss_2_valueChanged(double arg1);

    void on_catture_valueChanged(int arg1);

    void on_riardo_valueChanged(int arg1);

    void on_riav_progr_clicked();

    void on_cambia_pass_clicked();

    void on_cam_pass_line_textEdited(const QString &arg1);

    void on_new_pass_returnPressed();

    void on_ok_pass_clicked();

    void on_canc_pass_clicked();

    void on_riso_currentIndexChanged(int index);

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;

    QextSerialPort *port;

private:
    QFileSystemModel *dirmodel;
    QFileSystemModel *dirmodel_ok;
    QFileSystemModel *dirmodel_cam;
};


#endif // MAINWINDOW_H
