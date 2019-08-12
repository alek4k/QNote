#include "notewidget.h"
#include "simplenote.h"
#include "imgnote.h"
#include "todonote.h"
using std::string;

NoteWidget::NoteWidget(ListaNote& note, QString& percorsoFile, QWidget *parent)
    : QWidget(parent),
      textArea(new QPlainTextEdit(this)),
      layout(new QGridLayout(this)),
      searchBar(new QLineEdit(this)),
      imageLabel(new QLabel(this)),
      colonnaSx(new QVBoxLayout(this)),
      colonnaDx(new QVBoxLayout(this)),
      barraTopLeft(new QGridLayout(this)),
      barraTopRight(new QGridLayout(this)),
      addNotaButton(new QToolButton(this)),
      deleteNotaButton(new QToolButton(this)),
      addImgButton(new QToolButton(this)),
      addToDoListButton(new QToolButton(this)),
      addTagButton(new QToolButton(this)),
      todoList(new ToDoListWidget(this)),
      note(note),
      lista(new NoteListWidget(this)),
      path(percorsoFile),
      modificato(false)
{
    addToDoListButton->setVisible(false);
    addImgButton->setVisible(false);
    addTagButton->setVisible(false);
    deleteNotaButton->setVisible(false);
    imageLabel->setVisible(false);
    todoList->setVisible(false);
    textArea->setReadOnly(true);

    //impostazioni grafiche ToDoList
    todoList->setFocusPolicy(Qt::NoFocus);
    QPalette p = todoList->palette();
    p.setColor(QPalette::Highlight, Qt::transparent);
    p.setColor(QPalette::Active, QPalette::Highlight, Qt::gray);
    todoList->setPalette(p);

    //ACTION SCRITTURA TESTO SU CAMPO DI RICERCA
    connect(searchBar, SIGNAL(textChanged(QString)), this, SLOT(cercaNote(QString)));

    //SALVATAGGIO AUTOMATICO SCRITTURA DESCRIZIONE NOTA
    connect(textArea, SIGNAL(textChanged()), this, SLOT(scritturaNota()));

    //SALVATAGGIO AUTOMATICO OBIETTIVI TO-DO LIST
    connect(todoList, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(highlightChecked(QListWidgetItem*)));

    //PRESSIONE PULSANTE ELIMINAZIONE NOTA
    connect(deleteNotaButton, SIGNAL(clicked()), this, SLOT(cancellaNota()));

    //PRESSIONE PULSANTE CREAZIONE NOTA
    connect(addNotaButton, SIGNAL(clicked()), this, SLOT(creaNota()));

    //PRESSIONE PULSANTE AGGIUNTA TAG
    connect(addTagButton, SIGNAL(clicked()), this, SLOT(addTag()));

    //PRESSIONE PULSANTE AGGIUNTA TODO LIST
    connect(addToDoListButton, SIGNAL(clicked()), this, SLOT(trasformaInObiettivo()));

    //PRESSIONE PULSANTE AGGIUNTA IMMAGINE
    connect(addImgButton, SIGNAL(clicked()), this, SLOT(imageOpen()));

    //CAMBIO SELEZIONE NOTA DALLA LISTA
    connect(lista, SIGNAL(itemSelectionChanged()), this, SLOT(selezioneNota()), Qt::QueuedConnection);

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
    barraTopRight->addWidget(deleteNotaButton, 0, 4);
    barraTopRight->setAlignment(Qt::AlignRight);
    barraTopRight->setColumnMinimumWidth(3, 35);

    colonnaSx->addLayout(barraTopLeft);
    colonnaSx->addWidget(lista);
    layout->addLayout(colonnaSx, 0, 0);

    colonnaDx->addLayout(barraTopRight);
    colonnaDx->addWidget(textArea);
    layout->addLayout(colonnaDx, 0, 1);

    layout->setColumnStretch(0, 10);
    layout->setColumnStretch(1, 20);

    //carico tutte le note nella lista
    refreshList();
}

NoteWidget::~NoteWidget() {
    const auto & sentinella = todoList->item(0);
    delete sentinella;
}

void NoteWidget::selezioneNota() {
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
        imageLabel->setVisible(false);
        todoList->setVisible(false);
    }
    else {
        Nota& t = *static_cast<NoteListWidgetItem*>(items[0])->getNota();

        QString titolo = static_cast<NoteListWidgetItem*>(items[0])->getNota()->getTitolo();
        QString descr = static_cast<NoteListWidgetItem*>(items[0])->getNota()->getDescrizione();
        textArea->setPlainText(titolo + "\n" + descr);

        currentRowNota = lista->currentRow();

        auto imgNota = dynamic_cast<ImgNote*>(&t);
        if (imgNota) {
            QImage img;
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

            todoList->addEntry(new ToDoItem("Inserisci obiettivo..."));
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
}

void NoteWidget::cercaNote(QString ricerca) const {
    if (ricerca.compare("") == 0) {
        refreshList();
        return;
    }

    lista->clear();
    auto queryResult = this->note.simpleSearch(RicercaTesto(QString(ricerca)));
    for (auto it = queryResult.begin(); it != queryResult.end(); ++it) {
        lista->addEntry(*it);
    }
}

void NoteWidget::scritturaNota() {
    /* se non ho il focus sulla text area quando cambia il testo allora ha solo caricato la
     * visualizzazione della nota. Per sicurezza guardo anche se l'indice di riga è cambiato:
     * in questo caso sto arrivando da un'altra nota e non devo salvare nulla
     */
    if (lista->currentRow() != currentRowNota || !textArea->hasFocus()) return;

    auto items = lista->selectedItems();

    //la prima riga diventa il titolo della nota
    QTextDocument *doc = (*textArea).document();
    QTextBlock firstRow = doc->findBlockByLineNumber(0);
    QString titolo = firstRow.text();

    auto nota = static_cast<NoteListWidgetItem*>(items[0])->getNota();

    if (titolo.compare(nota->getTitolo()) != 0) {
        nota->setTitolo(titolo);
        static_cast<NoteListWidgetItem*>(items[0])->setText(titolo);
    }

    //tutto il resto diventa descrizione della nota
    QString testo = textArea->toPlainText();
    QString descrizione = testo.mid(titolo.length()+1,testo.length()-titolo.length());
    nota->setDescrizione(descrizione);
    nota->setDataModifica();
    modificato = true;

    //if (currentRowNota != 0) refreshList();
}

void NoteWidget::trasformaInObiettivo() {
    auto items = lista->selectedItems();
    ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();
    ToDoNote* nuovaToDo = new ToDoNote((*it).getTitolo(), (*it).getDescrizione(), (*it).getTag());
    aggiornaNota(it, nuovaToDo);
}

void NoteWidget::highlightChecked(QListWidgetItem *item) {
    auto changed = static_cast<ToDoListWidgetItem*>(item);
    auto items = lista->selectedItems();
    int row = todoList->row(item);

    ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();
    if (changed->text().length() == 0) {
        /* il testo è stato rimosso, quindi elimino l'obiettivo corrente;
         * all'indice 0 c'è il ToDo di default per creare un nuovo obiettivo: in quel caso evito!
         */
        if (row != 0) {
            auto old_list = static_cast<ToDoNote&>(*it).getToDoList();
            //rimuovo tenendo conto del todo di default all'indice 0
            old_list.removeAt(row - 1);
            static_cast<ToDoNote&>(*it).setToDoList(old_list);
        }
    }
    else {
        if (row == 0) {
            //devo creare il todoItem e aggiungerlo alla lista di obiettivi
            auto old_list = static_cast<ToDoNote&>(*it).getToDoList();
            old_list.push_front(ToDoItem(changed->text(), changed->checkState() == Qt::Checked ? true : false));
            static_cast<ToDoNote&>(*it).setToDoList(old_list);
        }
        else {
            //normale modifica di testo o checkbox su un item già esistente
            changed->getToDo()->updateItem(changed->text(), changed->checkState() == Qt::Checked ? true : false);
        }
    }

    it->setDataModifica();
    modificato = true;

    refreshList();
}

void NoteWidget::refreshList() const{
    lista->clear();
    textArea->clear();
    imageLabel->clear();
    todoList->clear();

    auto queryResult = note.simpleSearch(VisualizzazioneOrdinata());
    for (auto it = queryResult.begin(); it != queryResult.end(); ++it) {
        lista->addEntry(*it);
    }

    lista->setCurrentRow(0);
}

void NoteWidget::creaNota() {
    this->note.push_front(SimpleNote("Nuova nota..."));
    modificato = true;
    refreshList();
}

void NoteWidget::cancellaNota() {
    auto items = lista->selectedItems();

    ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, (*it).getTitolo(), "Cancellare la nota selezionata?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
            note.remove(it);
            modificato = true;
            refreshList();
    }
}

void NoteWidget::aggiornaNota(ListaNote::Iterator& it, Nota* nota) {
    note.remove(it);
    note.push_back(*nota);

    modificato = true;

    refreshList();
}

void NoteWidget::addTag() {
    auto items = lista->selectedItems();
    ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();

    bool ok;
    QString tag = QInputDialog::getText(this, "Nuovo tag",
                                             "Descrizione tag:", QLineEdit::Normal,
                                             "", &ok);
    if (ok && !tag.isEmpty()) {
        QVector<QString> t = it->getTag();
        for (auto i = t.cbegin(); i != t.cend(); ++i) {
            if (i->toLower().compare(tag.toLower()) == 0) {
                QMessageBox::information (nullptr, "Attenzione", "Tag già presente nella nota");
                return;
            }
        }

        QVector<QString> temp(t);
        temp.push_back(tag);
        it->setTag(temp);
        it->setDataModifica();
        modificato = true;

        refreshList();
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

    QPixmap pixmap = QPixmap::fromImage(QImage(fileName));
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG"); // writes pixmap into bytes in PNG format

    ImgNote* nuovaImgNota = new ImgNote(it->getTitolo(),
                                        it->getDescrizione(),
                                        it->getTag(),
                                        bytes.toBase64());
    aggiornaNota(it, nuovaImgNota);

    return true;
}

void NoteWidget::imageOpen() {
    auto items = lista->selectedItems();
    ListaNote::Iterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();

    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    //tengo aperto il dialog fino a quando l'utente non lo chiude o inserisce un file valido
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first(), it)) {}
}

void NoteWidget::setPath(QString& percorsoFile) {
    path = percorsoFile;
}

void NoteWidget::salvato() {
    modificato = false;
}

bool NoteWidget::modificheInSospeso() const {
    return modificato;
}
