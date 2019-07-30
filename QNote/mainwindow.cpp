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
    QList<ToDoItem> todoList;
    todoList.push_back(ToDoItem("testo", true));
    todoList.push_back(ToDoItem("uncompleted item", false));
    list.push_end(new SimpleNote("titolo", "descrizione", vector));
    list.push_end(new SimpleNote("aaaaah", "ok ok ok!!!", vector2));
    list.push_end(new ImgNote("immagine", "questa è una nota con immagine", vector, ""));
    list.push_end(new ToDoNote("todo", "to do list nota", vector2, todoList));


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
    QApplication::quit();
}

void MainWindow::importNote() {

}

void MainWindow::save() {
    QMessageBox messageBox;
    try {
        SerializzaNote serializzatore(percorsoFile);
        list.serializza(serializzatore);

        QString messaggio("Sono state correttamente salvate ");
        messaggio += QString::number(list.count()) + " note";

        messageBox.information(0, "Note salvate con successo", messaggio);
    } catch (const SerializeException& ex) {
        messageBox.critical(0, "Errore nel salvataggio delle note", ex.what());
    }
}

void MainWindow::exportNote() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("&Save As..."), "", tr("Calendario JSON (*.json)"));

    // L'utente ha premuto annulla...
    if (fileName.isEmpty()) return;

    // Cambio il percorso del file attualmente utilizzato
    percorsoFile = fileName;

    save();

    // Salvo la lista di eventi
    save();
}

vector<QAbstractButton*> fun (list<QWidget*>& lst, const QSize& sz, vector<const QWidget*>& w) {
    vector<QAbstractButton*> result;
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        if (*it != nullptr && (*it)->sizeHint() == sz) {
            w.push_back(*it);

            if (!dynamic_cast<QAbstractSlider*>(*it)) {
                delete(*it);
                it = lst.erase(it);
                --it;
            }
            else {
                if(dynamic_cast<QCheckBox*>(*it) || dynamic_cast<QPushButton*>(*it)){
                    result.push_back(static_cast<QAbstractButton*>(*it));
                    it = lst.erase(it);
                    --it;
                }
            }
        }
    }
    return result;
}
