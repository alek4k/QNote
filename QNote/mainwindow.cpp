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
      salva(new QAction(this)),
      quit(new QAction(this)),
      percorsoFile("qNote.json")
{
    // Impostazioni finestra
    setWindowIcon(icon);
    setWindowTitle("QNote - Gestore di note");
    resize(900, 650);

    // File Menu
    salva->setText("Salva");
    importa->setText("Importa");
    esporta->setText("Esporta");
    quit->setText("Uscita");

    salva->setShortcut(QKeySequence::Save);
    salva->setStatusTip("Salva");
    connect(salva, SIGNAL(triggered()), this, SLOT(save()));
    fileMenu->addAction(salva);

    fileMenu->addSeparator();

    importa->setShortcuts(QKeySequence::Open);
    importa->setStatusTip("Importa da file");
    connect(importa, SIGNAL(triggered()), this, SLOT(importNote()));
    fileMenu->addAction(importa);

    esporta->setShortcuts(QKeySequence::SaveAs);
    esporta->setStatusTip("Esporta su file");
    connect(esporta, SIGNAL(triggered()), this, SLOT(exportNote()));
    fileMenu->addAction(esporta);

    fileMenu->addSeparator();

    quit->setShortcuts(QKeySequence::Quit);
    quit->setStatusTip("Esci dal programma");
    connect(quit, SIGNAL(triggered()), this, SLOT(exit()));
    fileMenu->addAction(quit);

    load(false);
    if (list.empty()) {
        list.push_back(SimpleNote("QNote", "Con QNote puoi organizzare le tue note, aggiungendo tag, immagini e liste di obiettivi!"));

        mainWidget = new NoteWidget(list, percorsoFile, this);

        // Imposto il widget principale sulla finestra
        setCentralWidget(mainWidget);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);
    exit();
}

void MainWindow::exit() {
    if (mainWidget->modificheInSospeso()) {
        //ho delle modifiche eventualmente da salvare prima di chiudere
        if (QMessageBox::Yes == QMessageBox::question(nullptr, "Modifiche in sospeso", "Salvare tutte le modifiche prima di uscire?"))
            save();
    }
    QApplication::quit();
}

void MainWindow::load(bool showError) {
    // Carico il file con le note
    try {
        DeserializzaNote lettoreNote(percorsoFile);
        ListaNote risultatoDeserializzazione(
                    ListaNote::deserializza(lettoreNote));
        list.swap(risultatoDeserializzazione);

        QWidget *theWidget = centralWidget();
        mainWidget = new NoteWidget(list, percorsoFile, this);
        setCentralWidget(mainWidget);
        theWidget->deleteLater();
        mainWidget->salvato();
    } catch (const DeserializeException& ex) {
        if (showError) {
            QMessageBox messageBox;
            messageBox.critical(nullptr, "Impossibile caricare le note", ex.what());
        }
    }
}

void MainWindow::importNote() {
    if (mainWidget->modificheInSospeso()) {
        //ho delle modifiche eventualmente da salvare prima di importare un altro file
        if (QMessageBox::Yes == QMessageBox::question(nullptr, "Modifiche in sospeso", "Salvare tutte le modifiche prima di importare un altro file?"))
            save();
    }

    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Importa..."), "",
            tr("Note JSON (*.json);;All Files (*)"));

    // Viene premuto annulla... non faccio niente
    if (fileName.isEmpty()) return;

    // Verifico che il file sia corretto e se lo è lo imposto come file corrente
    QFileInfo fileInfo(fileName);
    if ((fileInfo.exists()) && (fileInfo.isFile()) && (fileInfo.size() > 0)) {
        percorsoFile = fileName;

        load();
    } else { // Se il file non è valido
        QMessageBox message;
        message.warning(nullptr, "File non valido", "E' stato selezionato un file non valido");
    }
}

void MainWindow::exportNote() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("Esporta..."), "", tr("Note JSON (*.json)"));

    // Viene premuto annulla... non faccio niente
    if (fileName.isEmpty()) return;

    // Aggiorno il path attualmente in utilizzo
    percorsoFile = fileName;

    QMessageBox messageBox;
    try {
        save();
        mainWidget->setPath(percorsoFile);
        mainWidget->salvato();

        QString messaggio("Sono state esportate ");
        messaggio += QString::number(list.count()) + " note";

        messageBox.information(nullptr, "Note salvate con successo", messaggio);
    } catch (const SerializeException& ex) {
        messageBox.critical(nullptr, "Errore durante l'esportazione delle note", ex.what());
    }
}

void MainWindow::save() {
    SerializzaNote serializzatore(percorsoFile);
    list.serializza(serializzatore);
    mainWidget->salvato();
}
