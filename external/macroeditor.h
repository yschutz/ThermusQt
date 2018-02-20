#ifndef MACROEDITOR_H
#define MACROEDITOR_H

#include <QWidget>
#include<QPluginLoader>

#include "macrointerface.h"

class QLabel;
class QPushButton;
class EditorWindow;
class MacroEditor : public QObject
{
    Q_OBJECT

public:
    MacroEditor(QObject* parent = 0);
    ~MacroEditor(){;}

    void start();

private slots:
    void closeEditor();
    void copyFiles() const;
    void newMacro(const QString& className);
    void openFile(const QString& fileName);
    void saveMacro();

private:
    void    editMacro();
    QString findQt() const;
    void    loadLibrary(const QString& library);

    QPushButton*       mCancelButton;     // triggers cancelling all
    QString            mClassName;        // the name of the macro Class
    QString            mClassFileName;    // the base name of the macro Class file including path
    QWidget*           mEditor;           // the editor window
    EditorWindow*      mEditorcpp;        // editor window of the cpp file
    EditorWindow*      mEditorh;          // editor window of the h file
    QString            mExecutableDir;    // application directory
    QLabel*            mLabelh;           // label for edit h window
    QLabel*            mLabelcpp;         // label for edit cpp window
    QString            mLibSuffix;        // lib suffix depending on OS
    QString            mMacroDirName;     // dir name where macro is stored
    MacroInterface*    mMacroInterface;   // the macro interface
    bool               mNeuf;             // flag for new (true) or existing (false) macro
    QPluginLoader      mPluginLoader;     // the pluginloader;
    QString            mQtPath;           // path to Qt bin
    QPushButton*       mSaveButton;       // triggers saving the plugin files
};

#endif // MACROEDITOR_H
