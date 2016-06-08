#ifndef WINDOWPARAM_H
#define WINDOWPARAM_H

#include <QWindow>
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QTextEdit>

class WindowParam;

class ParamTab : public QWidget
{
    Q_OBJECT
public:
    explicit ParamTab(QWidget* parent = 0);
};

class VariableTab : public QWidget
{
    Q_OBJECT
    QTableWidget *viewVariable;
    WindowParam* parent;
public slots:
    void deleteIdentifier();

public:
    explicit VariableTab(WindowParam* parent = 0);
    void refresh();
};

class ProgramTab : public QWidget
{
    Q_OBJECT
    QTableWidget *viewProgram;
    WindowParam* parent;
public slots:
    void deleteIdentifier();
    //void editIdentifier();
public:
    explicit ProgramTab(WindowParam* parent = 0);
    void refresh();
};

class WindowParam : public QWidget
{
    Q_OBJECT
    ParamTab *paramTab;
    VariableTab *variableTab;
    ProgramTab *programTab;
public slots:
    void addIdentifier();
    void editIdentifier();
    void refreshVariable();
    void refreshProgram();
public:
    explicit WindowParam(QWidget* parent = 0);
};

class WindowAddIdentifier : public QWidget {
    Q_OBJECT
    QLineEdit* keyLineEdit;
    QTextEdit* valueTextEdit;
    QLabel* messageError;
public slots:
    void save();
signals:
    void needRefresh();
public :
    explicit WindowAddIdentifier(WindowParam* parent = 0);
};

class WindowEditIdentifier : public QWidget {
    Q_OBJECT
    std::string key;
    QTextEdit* valueTextEdit;
    QLabel* messageError;
public slots:
    void save();
signals:
    void needRefresh();
public :
    explicit WindowEditIdentifier(std::string key ,WindowParam* parent = 0);
};


#endif // WINDOWPARAM_H
