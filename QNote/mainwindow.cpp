#include "mainwindow.h"
#include <QtCore>
#include <QtWidgets>
#include "simplenote.h"
#include "nota.h"
#include "todonote.h"
#include "imgnote.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      icon(new QIcon("QNote.ico")),
      fileMenu(menuBar()->addMenu("&File")),
      importa(new QAction(this)),
      esporta(new QAction(this)),
      quit(new QAction(this))
{
    QVector<QString> vector;
    vector << "alpha" << "beta" << "delta";
    QVector<QString> vector2;
    vector2 << "ok" << "beta";
    list.push_front(new SimpleNote("titolo", "descrizione", vector));
    list.push_front(new SimpleNote("aaaaah", "ok ok ok!!!", vector2));
    list.push_front(new ImgNote("immagine", "questa è una nota con immagine", vector, ""));
    list.push_front(new ToDoNote("todo", "to do list nota", vector2, QList<ToDoItem>()));

    mainWidget = new NoteWidget(list, this);

    // Impostazioni finestra
    setWindowIcon(*icon);
    setWindowTitle("QNote - Gestore di note");
    resize(900, 650);

    // File Menu
    importa->setText("Importa");
    esporta->setText("Esporta");
    quit->setText("Uscita");

    importa->setShortcuts(QKeySequence::Open);
    importa->setStatusTip("Importa da file");
    connect(importa, SIGNAL(triggered()), this, SLOT(importNote()));
    fileMenu->addAction(importa);

    esporta->setShortcuts(QKeySequence::Save);
    esporta->setStatusTip("Esporta su file");
    connect(esporta, SIGNAL(triggered()), this, SLOT(exportNote()));
    fileMenu->addAction(esporta);

    fileMenu->addSeparator();

    quit->setShortcuts(QKeySequence::Quit);
    quit->setStatusTip("Esci dal programma");
    connect(quit, SIGNAL(triggered()), this, SLOT(exit()));
    fileMenu->addAction(quit);

    // Imposto il widget principale sulla finestra
    setCentralWidget(mainWidget);
}

void MainWindow::exit() {
    QVector<QString> vector;
    vector << "alpha" << "beta" << "delta";
    list.push_front(new SimpleNote("kkfkdsk", "fdsf ok ok!!!", vector));
    //mainWidget->lista->addEntry(list.end());
    //QApplication::quit();
}

