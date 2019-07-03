#include "notewidget.h"
#include "simplenote.h"
#include "imgnote.h"
#include "todonote.h"

NoteWidget::NoteWidget(ListaNote& note, QWidget *parent)
    : QWidget(parent),
      textArea(new QPlainTextEdit),
      layout(new QGridLayout(this)),
      searchBar(new QLineEdit),
      image(new QPixmap),
      imageLabel(new QLabel),
      colonnaSx(new QVBoxLayout(this)),
      colonnaDx(new QVBoxLayout(this)),
      barraTopLeft(new QGridLayout(this)),
      barraTopRight(new QGridLayout(this)),
      addNotaButton(new QToolButton),
      deleteNotaButton(new QToolButton),
      addImgButton(new QToolButton),
      addToDoListButton(new QToolButton),
      addTagButton(new QToolButton),
      note(note),
      lista(new NoteListWidget(this))
{

    addToDoListButton->setVisible(false);
    addImgButton->setVisible(false);
    deleteNotaButton->setVisible(false);
    textArea->setReadOnly(true);

    //ACTION SCRITTURA TESTO SU CAMPO DI RICERCA
    connect(searchBar, &QLineEdit::textChanged, [this] () {
        QString lookingFor = searchBar->text();
        if (lookingFor.compare("") == 0) {
            refreshList();
            return;
        }

        lista->clear();
        Container<const ListaNote::Iterator> queryResult = this->note.simpleSearch(RicercaTesto(QString(lookingFor)));
        for (auto it = queryResult.begin(); it != queryResult.end(); ++it) {
            lista->addEntry(*it);
        }
    });

    //SALVATAGGIO AUTOMATICO SCRITTURA DESCRIZIONE NOTA
    connect(textArea, &QPlainTextEdit::textChanged, [this] () {
        auto items = lista->selectedItems();
        if (items.length() != 1) {
            return;
        }
        else {
            static_cast<NoteListWidgetItem*>(items[0])->getNota()->setDescrizione(textArea->toPlainText());
        }
    });

    //PRESSIONE PULSANTE ELIMINAZIONE NOTA
    connect(deleteNotaButton, &QToolButton::clicked, [this] () {
        auto items = lista->selectedItems();
        if (items.length() == 1) {
            ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();
            cancellaNota(it);
        }
    });

    //PRESSIONE PULSANTE CREAZIONE NOTA
    connect(addNotaButton, &QToolButton::clicked, [this] () {
        this->note.push_front(new SimpleNote("Nuova nota..."));
        refreshList();
    });

    //opzioni grafiche lista note
    //lista->setStyleSheet( "QListWidget::item { border-bottom: 1px solid black; }" );
    //lista->setAlternatingRowColors(true);

    //qualche miglioria grafica per l'area di testo
    textArea->setStyleSheet("QPlainTextEdit { "
                               "padding-left:20; "
                               "padding-top:20; "
                               "padding-bottom:20; "
                               "padding-right:10; "
                               "background-color: white}"
                               );

    image = new QPixmap(QPixmap::fromImage(QImage("bella.jpg")));

    // set a scaled pixmap to a w x h window keeping its aspect ratio

    imageLabel->setAlignment(Qt::AlignCenter);


    //settings campo di ricerca
    searchBar->setPlaceholderText("Tutte le note...");
    searchBar->setToolTip("Cerca");

    addNotaButton->setIcon(QIcon("addNota.png"));
    //addNotaButton->setIconSize(QSize(20, 20));
    addNotaButton->setToolTip("Nuova nota");


    addImgButton->setIcon(QIcon("addPicture.png"));
    addImgButton->setToolTip("Aggiungi immagine");

    deleteNotaButton->setIcon(QIcon("deleteNota.png"));
    deleteNotaButton->setToolTip("Elimina nota");

    addToDoListButton->setIcon(QIcon("addToDoList.png"));
    addToDoListButton->setToolTip("Aggiungi lista di obiettivi");

    addTagButton->setIcon(QIcon("tags2.png"));
    addTagButton->setToolTip("Aggiungi tag");

    barraTopLeft->addWidget(searchBar, 0, 0);
    barraTopLeft->addWidget(addNotaButton, 0, 1);
    barraTopLeft->setColumnStretch(0, 90);
    barraTopLeft->setColumnStretch(1, 10);

    barraTopRight->addWidget(addTagButton, 0, 0);
    barraTopRight->addWidget(addImgButton, 0, 1);
    barraTopRight->addWidget(addToDoListButton, 0, 2);
    barraTopRight->addWidget(nullptr, 0, 3);
    barraTopRight->addWidget(deleteNotaButton, 0, 4);
    barraTopRight->setAlignment(Qt::AlignRight);
    barraTopRight->setColumnMinimumWidth(3, 35);

    colonnaSx->addLayout(barraTopLeft);
    colonnaSx->addWidget(lista);
    layout->addLayout(colonnaSx, 0, 0);

    //colonnaDx->addWidget(imageLabel);
    colonnaDx->addLayout(barraTopRight);
    colonnaDx->addWidget(textArea);
    layout->addLayout(colonnaDx, 0, 1);

/*
    //carico componenti nella grid-layout
    layout->addWidget(searchBar, 0, 0);
    layout->addWidget(lista, 1, 0);
    //layout->addWidget(textArea, 0, 1, 1, 0);
    layout->addWidget(textArea, 1, 1);
    layout->addWidget(imageLabel, 0, 1);
*/
    //layout->addWidget(textEdit, 2, 2);
    layout->setColumnStretch(0, 10);
    layout->setColumnStretch(1, 20);

    //layout->addLayout(colonnaSx,0,0);


    //CAMBIO SELEZIONE NOTA DALLA LISTA
    connect(lista, &NoteListWidget::itemSelectionChanged, [this] () {
        auto items = lista->selectedItems();
        if (items.length() != 1) {
            imageLabel->clear();
            colonnaDx->removeWidget(imageLabel);
            textArea->clear();
            deleteNotaButton->setVisible(false);
            textArea->setReadOnly(true);
            addImgButton->setVisible(false);
            addToDoListButton->setVisible(false);
            addTagButton->setVisible(false);
            deleteNotaButton->setVisible(false);
        }
        else {
            Nota& t = *static_cast<NoteListWidgetItem*>(items[0])->getNota();

            textArea->setPlainText(static_cast<NoteListWidgetItem*>(items[0])->getNota()->getDescrizione());

            if (dynamic_cast<ImgNote*>(&t)) {
                imageLabel->setPixmap((*image).scaled(400,400,Qt::KeepAspectRatio));
                colonnaDx->removeWidget(textArea);
                colonnaDx->addWidget(imageLabel);
                colonnaDx->addWidget(textArea);
                imageLabel->setVisible(true);
                addImgButton->setVisible(false);
                addToDoListButton->setVisible(false);
            }
            else {
                imageLabel->clear();
                colonnaDx->removeWidget(imageLabel);
                imageLabel->setVisible(false);
            }

            if (dynamic_cast<ToDoNote*>(&t)) {
                addToDoListButton->setVisible(false);
            }

            if (dynamic_cast<SimpleNote*>(&t)) {
                addToDoListButton->setVisible(true);
                addImgButton->setVisible(true);
            }

            textArea->setReadOnly(false);
            deleteNotaButton->setVisible(true);
            addTagButton->setVisible(true);
        }
    });

    //carico tutte le note nella lista
    refreshList();
}

void NoteWidget::refreshList() {
    lista->clear();
    textArea->clear();
    imageLabel->clear();

    for (auto it = note.begin(); it != note.end(); ++it) {
        lista->addEntry(it);
    }

    lista->setCurrentRow(0);
}

void NoteWidget::cancellaNota(ListaNote::Iterator& it) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, (*it).getTitolo(), "Cancellare la nota selezionata?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
            note.remove(it);
            refreshList();
    }
}
