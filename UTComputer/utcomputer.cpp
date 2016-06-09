﻿#include "utcomputer.h"
#include "Settings.h"
#include "calculator.h"
#include "WindowParam.h"
#include "Manager.h"
#include "MainFrame.h"
#include <QMenuBar>
#include <QtWidgets>
#include <QFile>
#include <QDataStream>

int const UTComputer::EXIT_CODE_REBOOT = -123456789;

UTComputer::UTComputer(QWidget *parent) : QMainWindow(parent){
    load();
    central = new Calculator(this);

    setCentralWidget(central);

    createActions();
    createMenus();

    connect(this, SIGNAL(destroyed(QObject*)), this, SLOT(save()));

    setWindowTitle(tr("Calculator"));
}

void UTComputer::undo() {
    Manager::getInstance().undo();
}
void UTComputer::redo() {
    Manager::getInstance().redo();
}

void UTComputer::save() {
    QString filename = "UTComputer.txt";
    QFile file(filename);
    auto settings = Manager::getInstance().getSettings();
    auto pile = Manager::getInstance().getPile();
    auto identifiers = Manager::getInstance().getIdentifiers();

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open " << filename;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);

    out << settings.getBeepMessage() << settings.getDisplayKeyboard() << settings.getNbLinesDisplayPile();
    out << pile.size();
    for(auto v: pile) {
        out << QString::fromStdString(v->toString());
    }

    out << (unsigned int)identifiers.size();
    for (auto v : identifiers) {
        out << QString::fromStdString(v.first);
        out << QString::fromStdString(v.second->toString());
    }

    file.flush();
    file.close();
}

void UTComputer::load() {
    QString filename = "UTComputer.txt";
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open " << filename;
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_1);
    unsigned int nbLine;
    bool display;
    bool beep;

    in >> beep >> display >> nbLine;
    Manager::getInstance().getSettings().setBeepMessage(beep);
    Manager::getInstance().getSettings().setDisplayKeyboard(display);
    Manager::getInstance().getSettings().setNbLinesDisplayPile(nbLine);

    unsigned int count;

    in >> count;
    QString key;
    QString value;
    QString command;
    for (unsigned int i = 0; i < count; i++) {
        in >> value;
        //command += " " + value;
        Manager::getInstance().handleOperandLine(value.toStdString());
    }
    //Manager::getInstance().handleOperandLine(command.toStdString());
    command = "";
    in >> count;
    for(unsigned int i = 0; i< count; i++){
        in >> key;
        in >> value;
        command += " " + value + " " + key + " STO ";
    }
    Manager::getInstance().handleOperandLine(command.toStdString());

    file.close();
}

void UTComputer::history() {
    HistoryWindow* window = new HistoryWindow(this);
    window->show();
}


void UTComputer::param() {
    WindowParam* window = new WindowParam(central);
    window->show();
}

void UTComputer::createActions() {
    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, &QAction::triggered, this, &UTComputer::undo);

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, &QAction::triggered, this, &UTComputer::redo);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the calculator"));
    connect(saveAct, &QAction::triggered, this, &UTComputer::save);

    loadAct = new QAction(tr("&Load"), this);
    //loadAct->setShortcuts(QKeySequence::Load);
    loadAct->setStatusTip(tr("Load the calculator"));
    connect(loadAct, &QAction::triggered, this, &UTComputer::load);

    paramAct = new QAction(tr("&Preferences"), this);
    paramAct->setShortcut(QKeySequence(tr("Ctrl+,")));
    paramAct->setStatusTip(tr("Open the preferences"));
    connect(paramAct, &QAction::triggered, this, &UTComputer::param);

    historyAct = new QAction(tr("&History"), this);
    historyAct->setShortcut(QKeySequence(tr("Ctrl+H")));
    historyAct->setStatusTip(tr("View commands history"));
    connect(historyAct, &QAction::triggered, this, &UTComputer::history);
}

void UTComputer::createMenus() {
    paramMenu = menuBar()->addMenu(tr("&Tools"));
    paramMenu->addAction(paramAct);
    paramMenu->addAction(historyAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addAction(saveAct);
    editMenu->addAction(loadAct);
}

