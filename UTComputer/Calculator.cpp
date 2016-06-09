#include <QtWidgets>
#include "OperatorManager.h"
#include <cmath>
#include "WindowException.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSound>
#include <QString>
#include "UTException.h"
#include "Button.h"
#include "Calculator.h"
#include "Settings.h"
#include "MainFrame.h"
#include "UTComputer.h"
#include "Manager.h"

Calculator::Calculator(UTComputer *parent) : parentComputer(parent) {
    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *rightLayout = new QVBoxLayout;
    QVBoxLayout *topLayout = new QVBoxLayout;

    QHBoxLayout* errorLayout = new QHBoxLayout();
    message = new QTextEdit();
    message->setReadOnly(true);
    message->setStyleSheet("color:red;");
    message->setMaximumHeight(60);
    errorLayout->addWidget(message);
    QPushButton* detailErrorButton = new QPushButton("More");
    connect(detailErrorButton, SIGNAL(clicked(bool)), this, SLOT(openDetailErrorWindow()));
    errorLayout->addWidget(detailErrorButton);

    topLayout->addLayout(errorLayout);

    viewPile = new QTableWidget();
    viewPile->setEditTriggers(QAbstractItemView::NoEditTriggers);
    viewPile->setColumnCount(1);
    viewPile->horizontalHeader()->hide();
    viewPile->horizontalHeader()->setStretchLastSection(true);
    //viewPile->setStyleSheet("background-color:black; color:white;");
    viewPile->setRowCount(Manager::getInstance().getSettings().getNbLinesDisplayPile());
    topLayout->addWidget(viewPile);
    refreshPile();


    command = new QLineEdit();
    command->installEventFilter(this);
    //connect(command, SIGNAL(cursorPositionChanged(int,int)), this, SLOT())
    topLayout->addWidget(command);

    keyBoard = new MainFrame(this);
    keyBoard->setVisible(Manager::getInstance().getSettings().getDisplayKeyboard());

    rightLayout->addLayout(topLayout);
    rightLayout->addWidget(keyBoard);

    //mainLayout->addLayout(operatorsLayout,10);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}

bool Calculator::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        command->setCursorPosition(command->text().size());
        return true;
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        std::string commandStr = command->text().toStdString();
        if (keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Left) return true;
        //Restauration de la commande antérieure
        if (keyEvent->key() == Qt::Key_Up) {
            if(commandStr.size() == 0) { //On part de la fin s'il n'y a rien sur la ligne
                commands_pos = commands.size();
            }
            if(commands_pos > 0) { //On continue depuis la position courante sinon
                command->setText(commands.at(--commands_pos));
            }
        }
        else if(keyEvent->key() == Qt::Key_Down) {
            if(commands_pos < (int)(commands.size() - 1)) {
                command->setText(commands.at(++commands_pos));
            }
        }
        //Opérateur arithmétique (un caractère) et guillemets fermés s'ils existent et programmes fermés
        if (OperatorManager::getInstance().isArithmeticOperator(keyEvent->text().toStdString())
                && std::count(commandStr.begin(), commandStr.end(), '"') % 2 == 0
                && (std::count(commandStr.begin(), commandStr.end(), '[') == std::count(commandStr.begin(), commandStr.end(), ']'))) {
            command->setText(command->text()+ QString(" ") + keyEvent->text());
            calculate();
            return true;
        }
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            calculate();
            return true;
        }
        return false;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

void Calculator::openDetailErrorWindow() {
    WindowException* window = new WindowException(messageDetail);
    window->show();
}

void Calculator::openMoreOperatorsWindow() {
    WindowMoreOperators* window = new WindowMoreOperators(this);
    window->show();
}

void Calculator::addDigitToCommand() {
    Button *clickedButton = qobject_cast<Button *>(sender());
    command->setText(command->text() + clickedButton->text());
    command->setFocus();
}

void Calculator::addSpaceToCommand() {
    command->setText(command->text() + " ");
    command->setFocus();
}

void Calculator::backspaceToCommand() {
    if(command->text().size() > 0) {
        std::string newCommand = command->text().toStdString();
        newCommand.pop_back();
        command->setText(QString::fromStdString(newCommand));
    }
    command->setFocus();
}

void Calculator::displayKeyboardChanged(int newValue) {
    Manager::getInstance().getSettings().setDisplayKeyboard(newValue);
    keyBoard->setVisible(Manager::getInstance().getSettings().getDisplayKeyboard());
}

void Calculator::nbLineDisplayPileChanged(int newValue) {
    Manager::getInstance().getSettings().setNbLinesDisplayPile(newValue);
    viewPile->setRowCount(Manager::getInstance().getSettings().getNbLinesDisplayPile());
    refreshPile();
}

void Calculator::beepMessageChanged(int newValue) {
    Manager::getInstance().getSettings().setBeepMessage(newValue);
}

void Calculator::addOperatorToCommand() {
    Button *clickedButton = qobject_cast<Button *>(sender());
    command->setText(command->text() + " " + clickedButton->text());
    command->setFocus();
    calculate();
}

void Calculator::calculate() {
    try {
        std::string commandStr = command->text().toStdString();
        if(damnBoyWhatIsThisMysteriousFunction(commandStr));
        else Manager::getInstance().handleOperandLine(commandStr);
        deleteMessage();
        commands.push_back(command->text()); //Pour la navigation par flèches verticales
        commands_pos = commands.size();
        command->setText(QString());
    } catch (UTException e) {
        setMessage(e);
    }
    refreshPile();
}

void Calculator::setMessage(const UTException& e) {
    if (Manager::getInstance().getSettings().getBeepMessage()) QSound::play("/Users/aureliedigeon/Documents/UTC/P16/LO21/Projet-LO21/sound.wav");
    message->setText(QString::fromStdString(e.what()));
    messageDetail = e.details();
}

void Calculator::deleteMessage() {
    message->setText(QString());
    messageDetail = "";
}

void Calculator::refreshPile() {
    std::vector<std::string> pile = Manager::getInstance().getPileToString();
    unsigned int rowCount = Manager::getInstance().getSettings().getNbLinesDisplayPile() > pile.size() ?
                pile.size() : Manager::getInstance().getSettings().getNbLinesDisplayPile();
    for(unsigned int i=0; i < rowCount; i++)
    {
        QTableWidgetItem* newItem = new QTableWidgetItem(QString::fromStdString(pile[i]));
        viewPile->setItem(i, 0, newItem);
    }
    for(unsigned int i=rowCount; i < Manager::getInstance().getSettings().getNbLinesDisplayPile(); i++)
    {
        viewPile->takeItem(i,0);
    }
}

bool Calculator::damnBoyWhatIsThisMysteriousFunction(const std::string &s) {
    static bool pimped = false;
    static bool rotated = false;
    if(s == "PIMPMYCALC") {
        if(!pimped) parentComputer->setStyleSheet("QToolButton { background-color: #FE2E9A; } QMenuBar { background-color: #F881E6; } QMenuBar::item { background-color: transparent; } QStatusBar { background-color: #F881E6; } QMainWindow { background-color: #F881E6; }");
        else parentComputer->setStyleSheet("");
        pimped = !pimped;
        return true;
    }
    if(s == "ROTATE") {
        static QGraphicsView *view = new QGraphicsView();
        if(!rotated) {
            QGraphicsScene *scene = new QGraphicsScene();
            QGraphicsView *view = new QGraphicsView();
            scene->addWidget(parentComputer);
            view->setScene(scene);
            view->show();
            view->rotate(180);
        }
        else {
            qApp->exit(UTComputer::EXIT_CODE_REBOOT);
        }
        rotated = !rotated;
        return true;
    }
    return false;
}

