#include "notewidget.h"
#include "simplenote.h"
#include "imgnote.h"
#include "todonote.h"
using std::string;

NoteWidget::NoteWidget(ListaNote& note, QWidget *parent)
    : QWidget(parent),
      textArea(new QPlainTextEdit),
      layout(new QGridLayout(this)),
      searchBar(new QLineEdit),
      image(new QPixmap),
      imageLabel(new QLabel),
      colonnaSx(new QVBoxLayout()),
      colonnaDx(new QVBoxLayout()),
      barraTopLeft(new QGridLayout()),
      barraTopRight(new QGridLayout()),
      addNotaButton(new QToolButton),
      deleteNotaButton(new QToolButton),
      addImgButton(new QToolButton),
      addToDoListButton(new QToolButton),
      addTagButton(new QToolButton),
      todoList(new ToDoListWidget(this)),
      note(note),
      lista(new NoteListWidget(this))
{
    addToDoListButton->setVisible(false);
    addImgButton->setVisible(false);
    deleteNotaButton->setVisible(false);
    textArea->setReadOnly(true);

    //abilito drag and drop sulla ToDoList
    todoList->setDragDropMode(QAbstractItemView::InternalMove);
    //impostazioni grafiche ToDoList
    todoList->setFocusPolicy(Qt::NoFocus);
    QPalette p = todoList->palette();
    p.setColor(QPalette::Highlight, Qt::transparent);
    p.setColor(QPalette::Active, QPalette::Highlight, Qt::gray);
    todoList->setPalette(p);

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
            //la prima riga diventa il titolo della nota
            QTextDocument *doc = (*textArea).document();
            QTextBlock firstRow = doc->findBlockByLineNumber(0);
            QString titolo = firstRow.text();
            if (titolo.compare(static_cast<NoteListWidgetItem*>(items[0])->getNota()->getTitolo()) != 0) {
                static_cast<NoteListWidgetItem*>(items[0])->getNota()->setTitolo(titolo);
                static_cast<NoteListWidgetItem*>(items[0])->setText(titolo);
            }

            //tutto il resto diventa descrizione della nota
            QString testo = textArea->toPlainText();
            QString descrizione = testo.mid(titolo.length()+1,testo.length()-titolo.length());
            static_cast<NoteListWidgetItem*>(items[0])->getNota()->setDescrizione(descrizione);
        }
    });


    //SALVATAGGIO AUTOMATICO OBIETTIVI TO-DO LIST
    connect(todoList, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));

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

    //PRESSIONE PULSANTE AGGIUNTA TAG
    connect(addTagButton, &QToolButton::clicked, [this] () {
        auto items = lista->selectedItems();
        if (items.length() == 1) {
            ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();
            addTag(it);
        }
    });

    //PRESSIONE PULSANTE AGGIUNTA TODO LIST
    connect(addToDoListButton, &QToolButton::clicked, [this] () {
       auto items = lista->selectedItems();
       if(items.length() == 1) {
           ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();
           ToDoNote* nuovaToDo = new ToDoNote((*it).getTitolo(), (*it).getDescrizione(), (*it).getTag());
           aggiornaNota(it, nuovaToDo);
       }
    });

    //PRESSIONE PULSANTE AGGIUNTA IMMAGINE
    connect(addImgButton, &QToolButton::clicked, [this] () {
        auto items = lista->selectedItems();
        if(items.length() == 1) {
            ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();
            imageOpen(it);
        }
    });

    //qualche miglioria grafica per l'area di testo
    textArea->setStyleSheet("QPlainTextEdit { "
                               "padding-left:20; "
                               "padding-top:20; "
                               "padding-bottom:20; "
                               "padding-right:10; "
                               "background-color: white}"
                               );

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
    //barraTopRight->addWidget(nullptr, 0, 3);
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
            colonnaDx->removeWidget(todoList);
            textArea->clear();
            todoList->clear();
            deleteNotaButton->setVisible(false);
            textArea->setReadOnly(true);
            addImgButton->setVisible(false);
            addToDoListButton->setVisible(false);
            addTagButton->setVisible(false);
            deleteNotaButton->setVisible(false);
        }
        else {
            Nota& t = *static_cast<NoteListWidgetItem*>(items[0])->getNota();

            QString titolo = static_cast<NoteListWidgetItem*>(items[0])->getNota()->getTitolo();
            QString descr = static_cast<NoteListWidgetItem*>(items[0])->getNota()->getDescrizione();
            textArea->setPlainText(titolo + "\n" + descr);

            auto imgNota = dynamic_cast<ImgNote*>(&t);
            if (imgNota) {
                QImage img;
                //img.loadFromData(QByteArray::fromBase64(imgNota->getImage().toStdString().c_str()));
                img.loadFromData(QByteArray::fromBase64(imgNota->getImage().toLocal8Bit()));
                imageLabel->setPixmap(QPixmap::fromImage(img).scaled(400, 400, Qt::KeepAspectRatio));

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
                todoList->clear();
                auto currentToDoList = static_cast<ToDoNote*>(&t)->getToDoList();

                //todoList->addEntry(new ToDoItem("Inserisci obiettivo..."));
                for (auto it = currentToDoList.cbegin(); it != currentToDoList.cend(); ++it) {
                    todoList->addEntry(&const_cast<ToDoItem&>(*it));
                }

                colonnaDx->removeWidget(textArea);
                colonnaDx->addWidget(todoList);
                colonnaDx->addWidget(textArea);
                todoList->setVisible(true);
                addToDoListButton->setVisible(false);
            }
            else {
                todoList->clear();
                colonnaDx->removeWidget(todoList);
                todoList->setVisible(false);
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

void NoteWidget::highlightChecked(QListWidgetItem *item){
    auto changed = static_cast<ToDoListWidgetItem*>(item);
    changed->getToDo()->updateItem(changed->text(), changed->checkState() == Qt::Checked ? true : false);

    if(item->checkState() == Qt::Checked)
        item->setBackground(QColor("#ffffb2"));
    else
        item->setBackground(QColor("#ffffff"));
}

void NoteWidget::refreshList() const{
    lista->clear();
    textArea->clear();
    imageLabel->clear();
    todoList->clear();

    for (auto it = note.begin(); it != note.end(); ++it) {
        lista->addEntry(it);
    }

    lista->setCurrentRow(0);
}

void NoteWidget::cancellaNota(const ListaNote::Iterator& it) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, (*it).getTitolo(), "Cancellare la nota selezionata?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
            note.remove(it);
            refreshList();
    }
}

void NoteWidget::aggiornaNota(ListaNote::Iterator& it, Nota* nota) {
    note.remove(it);
    ++it;
    note.insert(it, nota);

    int old_index = lista->currentRow();
    refreshList();
    lista->setCurrentRow(old_index);
}

void NoteWidget::addTag(const ListaNote::Iterator& it) {
    bool ok;
    QString tag = QInputDialog::getText(this, "Nuovo tag",
                                             "Descrizione tag:", QLineEdit::Normal,
                                             "", &ok);
    if (ok && !tag.isEmpty()) {
        QVector<QString> t = (*it).getTag();
        for (auto i = t.cbegin(); i != t.cend(); ++i) {
            if ((*i).toLower().compare(tag.toLower()) == 0) {
                QMessageBox::information (nullptr, "Attenzione", "Tag già presente nella nota");
                return;
            }
        }

        QVector<QString>* temp = new QVector<QString>(t);
        temp->push_back(tag);
        (*it).setTag(*temp);
        delete temp;

        int old_index = lista->currentRow();
        refreshList();
        lista->setCurrentRow(old_index);
    }
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

bool NoteWidget::loadFile(const QString& fileName, ListaNote::Iterator& it)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    image = new QPixmap(QPixmap::fromImage(QImage(fileName)));

    QPixmap pixmap = QPixmap::fromImage(QImage(fileName));
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG"); // writes pixmap into bytes in PNG format

    ImgNote* nuovaImgNota = new ImgNote((*it).getTitolo(),
                                        (*it).getDescrizione(),
                                        (*it).getTag(),
                                        bytes.toBase64());
    aggiornaNota(it, nuovaImgNota);

    return true;
}

void NoteWidget::imageOpen(ListaNote::Iterator& it) {
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    //tengo aperto il dialog fino a quando l'utente non lo chiude o inserisce un file valido
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first(), it)) {}
}
