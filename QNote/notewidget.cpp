#include "notewidget.h"
#include "simplenote.h"

NoteWidget::NoteWidget(ListaNote& note, QWidget *parent)
    : QWidget(parent),
      textArea(new QPlainTextEdit),
      lineText(new QLineEdit),
      layout(new QGridLayout(this)),
      searchBar(new QLineEdit),
      note(note),
      lista(new NoteListWidget(this))
{
    //carico tutte le note nella lista
    refreshList();

    //connetto la lista delle note all'area di testo
    connect(lista, &NoteListWidget::itemSelectionChanged, [this] () {
        auto items = lista->selectedItems();
        if (items.length() != 1) {
            textArea->clear();
            textArea->setPlainText("");
        }
        else {
            textArea->setPlainText(static_cast<NoteListWidgetItem*>(items[0])->getNota()->getDescrizione());
            //noteDetailWidget->showNota(*static_cast<NoteListWidgetItem*>(items[0])->getNota());
        }
    });

    //connetto il campo di ricerca alla lista delle note
    connect(searchBar, &QLineEdit::textChanged, [this] () {
        QString lookingFor = searchBar->text();
        if (lookingFor.compare("") == 0) {
            refreshList();
            return;
        }

        lista->clear();
        Container<const ListaNote::ConstIterator> queryResult = this->note.search(RicercaTesto(QString(lookingFor)));
        for (auto it = queryResult.cbegin(); it != queryResult.cend(); ++it) {
            lista->addEntry(*it);
        }
    });

    //connetto l'area di testo alle note per salvare le modifiche
    connect(textArea, &QPlainTextEdit::textChanged, [this] () {
        auto items = lista->selectedItems();
        if (items.length() != 1) {
            return;
        }
        else {
            //ListaNote::ConstIterator it = static_cast<NoteListWidgetItem*>(items[0])->getNota();


            //textArea->setPlainText(static_cast<NoteListWidgetItem*>(items[0])->getNota()->getDescrizione());
            //noteDetailWidget->showNota(*static_cast<NoteListWidgetItem*>(items[0])->getNota());
        }
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

    //QTextEdit* textEdit = new QTextEdit;
    //QString imagePath = QApplication::applicationDirPath() + "/logo.png";
    //QString html = QString("<img src='bella.jpg'>");
    //textEdit->setHtml(html);

    //settings campo di ricerca
    searchBar->setPlaceholderText("Tutte le note...");

    //carico componenti nella grid-layout
    layout->addWidget(searchBar, 1, 1);
    layout->addWidget(lista, 2, 1);
    layout->addWidget(textArea, 1, 2, 2, 1);
    //layout->addWidget(textEdit, 2, 2);
    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 20);


}

void NoteWidget::refreshList() {
    lista->clear();

    for (auto it = note.cbegin(); it != note.cend(); ++it) {
        lista->addEntry(it);
    }
}
