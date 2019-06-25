#include "mainwindow.h"
#include <QtCore>
#include <QtWidgets>
#include "simplenote.h"
#include "nota.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent), icon(new QIcon("QNote.ico")),
      menuBar(new QMenuBar),
      fileMenu(new QMenu("File")),
      importa(new QAction(this)), esporta(new QAction(this)),
      quit(new QAction(this)),
      gridGroupBox(new QGroupBox()),
      list(new Note)
{
    QVector<QString> vector;
    vector << "alpha" << "beta" << "delta";
    //QList<SimpleNote>* list = new QList<SimpleNote>();
    list->push_back(new SimpleNote("titolo", "descrizione", vector));

    // Impostazioni finestra
    setWindowIcon(*icon);
    setWindowTitle("QNote - Gestore di note");
    //QHBoxLayout *body = new QHBoxLayout(this); // layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMenuBar(menuBar);
    resize(800, 600);

    //QVBoxLayout *left = new QVBoxLayout;
    noteListWidget = new NoteListWidget(list);
    //left->addWidget(noteListWidget);
    //body->addLayout(left, 33);

    noteListWidget->addEntry(list->begin());

    noteDetailWidget = new NoteDetailWidget();
    //auto items = noteListWidget->selectedItems();
    noteDetailWidget->showNota(*(list->begin()));

    //setCentralWidget(noteListWidget);
    QTextEdit* textArea = new QTextEdit;
    textArea->setText("Weeeeeee");


    QGridLayout *layout = new QGridLayout;
    layout->addWidget(noteListWidget, 1, 1);
    layout->addWidget(textArea, 1, 2);
    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 20);
    //layout->setSpacing(0);
    layout->setMargin(0);
    gridGroupBox->setLayout(layout);

    //mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(0,0,0,0);

    mainLayout->addWidget(gridGroupBox);
    setLayout(mainLayout);

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

    menuBar->addMenu(fileMenu);
    //
}

void MainWindow::exit() {
    QApplication::quit();
}

