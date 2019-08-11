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
protected slots:
    void importNote();
    void exportNote();
    void save();
    void exit();
    void load(bool showError = true);
private:
    QIcon icon;

    QMenu * const fileMenu;
    QAction * const importa;
    QAction * const esporta;
    QAction * const salva;
    QAction * const quit;

    void closeEvent(QCloseEvent *event);

    NoteWidget * mainWidget;

    ListaNote list;
    QString percorsoFile;
};

#endif // MAINWINDOW_H
