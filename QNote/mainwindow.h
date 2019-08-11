#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include "container.h"
#include "nota.h"
#include "notelistwidget.h"
#include "notewidget.h"
#include "note.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
private:
    QIcon icon;

    QMenu * const fileMenu;
    QAction * const importa;
    QAction * const esporta;
    QAction * const salva;
    QAction * const quit;

    NoteWidget * mainWidget;

    ListaNote list;
    QString percorsoFile;
private slots:
    void importNote();
    void exportNote();
    void saveNote();
    void exit();
    void load(bool showError = true);
};

#endif // MAINWINDOW_H
