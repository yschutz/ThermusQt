#include <QTextEdit>
#include <QVBoxLayout>

#include "editorwindow.h"

//__________________________________________________________________________
EditorWindow::EditorWindow(QWidget *parent) : QWidget(parent)
{
    setupEditor();
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mEditor);
    setLayout(layout);
}

//__________________________________________________________________________
void EditorWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    mEditor = new QTextEdit;
    mEditor->setFont(font);

    mHighlighter = new Highlighter(mEditor->document());
}
