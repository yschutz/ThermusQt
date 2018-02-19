#include <QApplication>
#include <QComboBox>
#include <QDate>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLibraryInfo>
#include <QLineEdit>
#include <QMessageBox>
#include <QProcess>
#include <QTextEdit>
#include <QPluginLoader>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "editorwindow.h"
#include "macroeditor.h"

//__________________________________________________________________________
MacroEditor::MacroEditor(QObject *parent) : QObject(parent),
    mMacroDirName(QDir::tempPath()), mMacroInterface(nullptr)
{
    // ctor: check if Qt installed
    mQtPath = QFileInfo(findQt()).absolutePath();
    if (mQtPath.isEmpty()) {
        QMessageBox::critical(nullptr, Q_FUNC_INFO, "Qt installation not found!");
        return;
    }

    QMessageBox::information(nullptr, "title", QLibraryInfo::location(QLibraryInfo::BinariesPath));

    mEditor = new QWidget();
    QHBoxLayout* editorLayout = new QHBoxLayout;

    mEditorh   = new EditorWindow(mEditor);
    mEditorcpp = new EditorWindow(mEditor);
    editorLayout->addWidget(mEditorh);
    editorLayout->addWidget(mEditorcpp);

    mLabelh   = new QLabel(mEditor);
    mLabelcpp = new QLabel(mEditor);
    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget(mLabelh, 0, Qt::AlignCenter);
    nameLayout->addWidget(mLabelcpp, 0, Qt::AlignCenter);

    mSaveButton = new QPushButton(tr("Save"));
    connect(mSaveButton, &QPushButton::pressed, this, [this] { saveMacro(); });
    mCancelButton = new QPushButton(tr("Cancel"));
    mCancelButton->setFixedSize(mCancelButton->sizeHint());
    mSaveButton->setFixedSize(mCancelButton->sizeHint());
    mCancelButton->setDefault(true);
    connect(mCancelButton, &QPushButton::pressed, this, [this] { closeEditor(); });

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(mCancelButton);
    buttonLayout->addWidget(mSaveButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(editorLayout);
    mainLayout->addLayout(buttonLayout);

    mEditor->setLayout(mainLayout);
}

//__________________________________________________________________________
void MacroEditor::closeEditor()
{
    // close the editor window
    mEditor->close();
}

//__________________________________________________________________________
void MacroEditor::editMacro()
{
     // edit a new (from a template) or an existing macro class
    QString hfileName;
    QString cppfileName;
    if (mNeuf) { // creates a new macro from template
        QDir plugintemplateDir(qApp->applicationDirPath());
        QMessageBox::information(nullptr, "info", plugintemplateDir.absolutePath());
        plugintemplateDir.cdUp();
        plugintemplateDir.cd("Resources/plugintemplate");
        hfileName =  plugintemplateDir.absoluteFilePath("plugintemplate.h");
        QFile hFile(hfileName);
        if (hFile.open(QFile::ReadOnly | QFile::Text)) {
            QString text = hFile.readAll();
            QRegularExpression re("XxXxxxxx");
            text.replace(re, mClassName);
            re.setPattern("xxxxxxxx");
            text.replace(re, mClassName.toLower());
            re.setPattern("XXXXXXXX");
            text.replace(re, mClassName.toUpper());
            text.prepend("//" + QDate::currentDate().toString() +"\n");
            hFile.close();
            mEditorh->attach(text);
        }
        cppfileName =  plugintemplateDir.absoluteFilePath("plugintemplate.cpp");
        QFile cppFile(cppfileName);
        if (cppFile.open(QIODevice::ReadOnly | QFile::Text)) {
            QString text = cppFile.readAll();
            QRegularExpression re("XxXxxxxx");
            text.replace(re, mClassName);
            re.setPattern("xxxxxxxx");
            text.replace(re, mClassName.toLower());
            re.setPattern("XXXXXXXX");
            text.replace(re, mClassName.toUpper());
            text.prepend("//" + QDate::currentDate().toString() +"\n");
            mEditorcpp->attach(text);
        }
    } else {
        mMacroDirName = QFileInfo(mClassFileName).absolutePath();
        if (QFileInfo(mClassFileName).suffix() == "h") {
            hfileName =  mClassFileName;
            cppfileName = mClassFileName.replace(".h", ".cpp");
        } else if (QFileInfo(mClassFileName).suffix() == "cpp") {
            cppfileName =  mClassFileName;
            hfileName   = mClassFileName.replace(".cpp", ".h");
        }
        QFile hFile(hfileName);
        if (hFile.open(QFile::ReadOnly | QFile::Text)) {
            mEditorh->attach(hFile);
            hFile.close();
        } else {
            QMessageBox::critical(nullptr, Q_FUNC_INFO, QString("File %1 not found").arg(hFile.fileName()));
            return;
        }
        QFile cppFile(cppfileName);
        if (cppFile.open(QFile::ReadOnly | QFile::Text)) {
            mEditorcpp->attach(cppFile);
            cppFile.close();
        }  else {
            QMessageBox::critical(nullptr, Q_FUNC_INFO, QString("File %1 not found").arg(cppFile.fileName()));
            return;
        }
    }
    mEditor->setWindowTitle("Class editor: " + mClassName);
    mEditorh->setTitle(mClassName + ".h");
    mEditorcpp->setTitle(mClassName + ".cpp");
    mLabelh->setText(mEditorh->windowTitle());
    mLabelcpp->setText(mEditorcpp->windowTitle());
    mEditor->show();
}

//__________________________________________________________________________
QString MacroEditor::findQt() const
{
    // search where Qt is installed
    QString rv("");
    QDir topDir("/usr/bin");
    QDirIterator it1(topDir.path(), QStringList() << "qmake", QDir::NoFilter, QDirIterator::Subdirectories);
    while (it1.hasNext())
        rv = it1.next();
    if (rv.isEmpty()) {
        topDir.setPath("/usr/local/bin");
        QDirIterator it(topDir.path(), QStringList() << "qmake", QDir::NoFilter, QDirIterator::Subdirectories);
        while (it.hasNext())
            rv = it.next();
        if (rv.isEmpty()) {
            topDir.setPath(QDir::homePath() + "/Qt");
            QDirIterator it(topDir.path(), QStringList() << "qmake", QDir::NoFilter, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                rv = it.next();
                if (QFileInfo(rv).isExecutable())
                    break;
            }
        }
    }
    return rv;
}

//__________________________________________________________________________
void MacroEditor::openFile(const QString& fileName)
{
    // gets library or class (.h &.cpp) as selected from the file dialog

    QString libSuffix("");
#ifdef Q_OS_MAC
    libSuffix = "dylib";
#else
    libSuffix = "so";
#endif

    QFileInfo f(fileName);
    mClassFileName = f.absoluteFilePath();
    if( f.suffix() == libSuffix) { // its a shared library
        loadLibrary(mClassFileName);
    } else if (f.suffix() == "cpp" || f.suffix() == "h") {
        mNeuf = false;
        editMacro();//false);
    } else {
        QMessageBox::critical(nullptr, Q_FUNC_INFO, QString("File %1 not found").arg(f.fileName()));
    }
}

//__________________________________________________________________________
void MacroEditor::saveMacro()//bool neuf)
{
    // save, compile(create a shared library) the macro
    closeEditor();
    QString fileName = QFileDialog::getSaveFileName(mEditor, tr("Save macro"), mMacroDirName + "/" + mClassName.toLower());
    if (QFileInfo(fileName).suffix() == "")
        fileName.append(".h");
    QFile hf( fileName );
    if (hf.open( QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream outh(&hf);
        outh << mEditorh->text();
        hf.close();
    }
    fileName.replace(".h", ".cpp");
    QFile cppf( fileName );
    if (cppf.open( QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream outcpp(&cppf);
        outcpp << mEditorcpp->text();
        cppf.close();
    }

    mMacroDirName = QFileInfo(fileName).absolutePath();
    QDir    srcDir      = qApp->applicationDirPath();
    srcDir.cdUp();
    if (mNeuf) {
        srcDir.cd("Resources/plugintemplate");
        QFile::copy(srcDir.absolutePath() + "/plugin_global.h", mMacroDirName + "/plugin_global.h");
        QFile::copy(srcDir.absolutePath() + "/plugintemplate.json", mMacroDirName + "/" + mClassName + ".json");
        QFile::copy(srcDir.absolutePath() + "/macrointerface.h", mMacroDirName + "/macrointerface.h");
        QFile::copy(srcDir.absolutePath() + "/makelibrary.sh", mMacroDirName + "/makelibrary.sh");
        QFile::setPermissions(mMacroDirName + "/makelibrary.sh", QFile::ReadUser | QFile::ExeUser | QFile::WriteUser);
        QFile fin(srcDir.absolutePath() + "/plugintemplate.pro");
        if (fin.open(QIODevice::ReadOnly | QFile::Text)) {
            QString text = fin.readAll();
            QRegularExpression re("XxXxxxxx");
            text.replace(re, mClassName);
            re.setPattern("xxxxxxxx");
            text.replace(re, mClassName.toLower());
            QFile fout(mMacroDirName + "/" + mClassName + ".pro");
            if (fout.open(QIODevice::WriteOnly | QFile::Truncate)) {
                QTextStream out(&fout);
                out << text;
                fout.close();
            }
            fin.close();
        } else {
            QMessageBox::critical(nullptr, Q_FUNC_INFO, QString("File %1 not found").arg(fin.fileName()));
            return;
        }
    }
    QDir save = QDir::current();
    QDir::setCurrent(mMacroDirName);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("QTDIR", mQtPath);
    QProcess p;
    p.setProcessChannelMode(QProcess::SeparateChannels);
    p.setProcessEnvironment(env);
    QStringList params;
    params << mClassName;
    p.start("./makelibrary.sh", params);
    p.waitForFinished();
    QString p_stdout = p.readAllStandardOutput();
    QStringList listout = p_stdout.split('\n');
    QString p_stderr = p.readAllStandardError();
    if (p.exitStatus() == QProcess::CrashExit || p_stderr != "") {
        QMessageBox msg(QMessageBox::Critical, "make abort", "Library creation aborted");
        msg.setInformativeText(msg.text() + " Error encountered in makelibrary.sh: " + p_stderr);
        msg.exec();
    } else {
        p.close();
        QString libName = mMacroDirName + "/lib" + mClassName + ".dylib";
        QFileInfo lib(libName);
        if (lib.exists() && lib.isFile())
            loadLibrary(libName);
        else {
            QMessageBox::critical(nullptr, Q_FUNC_INFO, QString("library %1 not created").arg(libName));
        }
        QDir::setCurrent(save.absolutePath());
    }
}

//__________________________________________________________________________
void MacroEditor::loadLibrary(const QString& library)
{
    // load a shared library
    if (mPluginLoader.isLoaded())
        mPluginLoader.unload();
    mPluginLoader.setFileName(library);
    QObject* macro = mPluginLoader.instance();
    if (macro) {
        mMacroInterface = qobject_cast<MacroInterface*>(macro);
        if (mMacroInterface) {
            QMessageBox::information(nullptr, "Running Macro", mMacroInterface->run("Hallo "));
        } else
            QMessageBox::critical(nullptr, Q_FUNC_INFO, QString("Macro library %1 not loaded").arg(library));
    }
}

//__________________________________________________________________________
void MacroEditor::start()
{
    // starts the macro editor (select file)
    QFileDialog diag(mEditor, tr("Open macro source or library"), mMacroDirName,
                     tr("libraries (*.dylib);; header files (*.h);;implemtation files (*.cpp)"));
    diag.setAcceptMode(QFileDialog::AcceptOpen);
    diag.setOption(QFileDialog::DontUseNativeDialog, true);

    QDialogButtonBox* button_box= diag.findChild<QDialogButtonBox *>("buttonBox");
    QPushButton* newButton = new QPushButton("New");
    connect(newButton, &QPushButton::pressed, this, [this, &diag] {diag.close(); newMacro(QFileInfo(diag.selectedFiles().at(0)).fileName());});
    button_box->addButton(newButton, QDialogButtonBox::ActionRole);

    for (QObject* o : diag.children())
        if (o->objectName() == "fileNameLabel")
            qobject_cast<QLabel*>(o)->setText("Class Name");

    if (diag.exec()) {
        QString fileName = diag.selectedFiles().at(0);
        mClassName    = QFileInfo(fileName).baseName();
        mMacroDirName = QFileInfo(fileName).absolutePath();
        openFile(fileName);
    }
}

//__________________________________________________________________________
void MacroEditor::newMacro(const QString& className)
{
    // creates a new macro class (.h & .cpp)
      mClassName = className;
      mNeuf = true;
      editMacro(); //true);
}
