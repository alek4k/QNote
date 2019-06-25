#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include "container.h"
#include "nota.h"
#include "notelistwidget.h"
#include "notedetailwidget.h"
#include "note.h"

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
private:
    QIcon * const icon;

    QMenuBar * const menuBar;
    QMenu * const fileMenu;
    QAction * const importa;
    QAction * const esporta;
    QAction * const quit;

    QGroupBox * const gridGroupBox;

    NoteListWidget * noteListWidget;
    NoteDetailWidget * noteDetailWidget;

    Note* list;
private slots:
    /*void importNote();
    void exportNote();*/
    void exit();

    //void createMenu();
};

#endif // MAINWINDOW_H
