#include "mainwindow.h"
#include <QtCore>
#include <QtWidgets>
#include "simplenote.h"
#include "nota.h"
#include "todonote.h"
#include "imgnote.h"

#include "iostream"
#include "vector"
#include "list"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      icon(QIcon("QNote.ico")),
      fileMenu(menuBar()->addMenu("&File")),
      importa(new QAction(this)),
      esporta(new QAction(this)),
      quit(new QAction(this)),
      percorsoFile("qNote.json")
{


    // Impostazioni finestra
    setWindowIcon(icon);
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

    list.push_back(new SimpleNote("QNote", "Con QNote puoi organizzare le tue note, aggiungendo tag, immagini e liste di obiettivi!"));
    mainWidget = new NoteWidget(list, percorsoFile, this);

    // Imposto il widget principale sulla finestra
    setCentralWidget(mainWidget);    

    load();
}

void MainWindow::exit() {
    QApplication::quit();
}

void MainWindow::load() {
    // Carico il file selezionato dall'utente
    try {
        DeserializzaNote lettoreNote(percorsoFile);
        ListaNote risultatoDeserializzazione = ListaNote::deserializza(lettoreNote);
        list.erase();
        //list.restore(risultatoDeserializzazione);
        //TODO: cambiare sta schifezza!!!
        for (auto it = risultatoDeserializzazione.cbegin(); it != risultatoDeserializzazione.cend(); ++it){
            auto simpleNote = dynamic_cast<const SimpleNote*>(&(*it));
            auto toDoNote = dynamic_cast<const ToDoNote*>(&(*it));
            auto imgNote = dynamic_cast<const ImgNote*>(&(*it));
            if (simpleNote) {
                list.push_back(new SimpleNote(*simpleNote));
            }
            else if (toDoNote) {
                list.push_back(new ToDoNote(*toDoNote));
            }
            else if (imgNote) {
                list.push_back(new ImgNote(*imgNote));
            }
        }

        QWidget *theWidget = centralWidget();
        mainWidget = new NoteWidget(list, percorsoFile, this);
        setCentralWidget(mainWidget);
        theWidget->deleteLater();
    } catch (const DeserializeException& ex) {
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Impossibile ripristinare le note", ex.what());
    }
}

void MainWindow::importNote() {
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Importa..."), "",
            tr("Note JSON (*.json);;All Files (*)"));

    // L'utente ha premuto su annulla
    if (fileName.isEmpty()) return;

    // Verifico che il file sia corretto e se lo è lo imposto come file corrente
    QFileInfo fileInfo(fileName);
    if ((fileInfo.exists()) && (fileInfo.isFile()) && (fileInfo.size() > 0)) {
        percorsoFile = fileName;

        load();
    } else { // Se il file non è valido
        // Mostro un messaggio di errore...
        QMessageBox message;
        message.warning(nullptr, "File non valido", "E' stato selezionato un file non valido");
    }
}

void MainWindow::exportNote() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("Esporta..."), "", tr("Note JSON (*.json)"));

    // L'utente ha premuto annulla...
    if (fileName.isEmpty()) return;

    // Cambio il percorso del file attualmente utilizzato
    percorsoFile = fileName;

    QMessageBox messageBox;
    try {
        SerializzaNote serializzatore(percorsoFile);
        list.serializza(serializzatore);
        mainWidget->setPath(percorsoFile);

        QString messaggio("Sono state correttamente salvate ");
        messaggio += QString::number(list.count()) + " note";

        messageBox.information(nullptr, "Note salvate con successo", messaggio);
    } catch (const SerializeException& ex) {
        messageBox.critical(nullptr, "Errore nel salvataggio delle note", ex.what());
    }
}
