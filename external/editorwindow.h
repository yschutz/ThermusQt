#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QTextEdit>
#include <QWidget>

#include "highlighter.h"

class EditorWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EditorWindow(const QString& title, QWidget* parent = nullptr);

    void    attach(QFile& file) { mEditor->setPlainText(file.readAll());}
    void    attach(const QString& text) { mEditor->setPlainText(text);}
    QString text() const { return mEditor->toPlainText(); }

private:
    void setupEditor();
    QTextEdit*   mEditor;
    Highlighter* mHighlighter;
};

#endif // EDITORWINDOW_H










