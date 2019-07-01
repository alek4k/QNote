#include "notewidget.h"
#include "simplenote.h"

NoteWidget::NoteWidget(ListaNote& note, QWidget *parent)
    : QWidget(parent),
      textArea(new QPlainTextEdit),
      lineText(new QLineEdit),
      layout(new QGridLayout(this)),
      note(note),
      lista(new NoteListWidget(this))
{

    /*QVector<QString> vector;
    vector << "alpha" << "beta" << "delta";
    ListaNote test;
    test.push_back(new SimpleNote("titolo1", "descrizione", vector));
    test.push_back(new SimpleNote("aaaaah1", "ok ok ok!!!", vector));
*/
    lista->addEntry(note.begin());
    lista->addEntry(++note.begin());


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

    //lista->setStyleSheet( "QListWidget::item { border-bottom: 1px solid black; }" );
    //lista->setStyleSheet("QListWidget::item:alternate{border-bottom: 3px solid green;} QListWidget{background-color: black;}");
    //lista->setAlternatingRowColors(true);

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

    layout->addWidget(lista, 1, 1);
    layout->addWidget(textArea, 1, 2);
    //layout->addWidget(textEdit, 2, 2);
    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 20);

    layout->setMargin(0);
    layout->setSpacing(0);

    //TEST RICERCA
    //ListaNote::ConstIterator* it = new ListaNote::ConstIterator(note.cbegin());
    RicercaTesto searchQ (QString("ok"));
    Container<const ListaNote::ConstIterator> gh = note.search(searchQ);
    /*for (auto it = gh.cbegin(); it != gh.cend(); ++it) {
        QString temp = "dd";
    }*/

    QString fsdfds = "";
}
