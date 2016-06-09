#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QEvent>
#include <QObject>
#include <QKeyEvent>

class UTException;
class Button;
class Settings;
class MainFrame;
class Manager;
class UTComputer;

class Calculator : public QWidget
{
private:
    Q_OBJECT
    QTextEdit* message;
    QTableWidget* viewPile;
    QLineEdit* command;
    QVBoxLayout* layout;
    MainFrame* keyBoard;
    std::string messageDetail;
    std::vector<QString> commands;
    int commands_pos;
    UTComputer* parentComputer;

    void setMessage(const UTException&);
    void deleteMessage();
    /**
     * @brief Seems like you're taking a look at something hidden...
     * @param s Some magic word
     * @return kappa
     */
    bool damnBoyWhatIsThisMysteriousFunction(const std::string& s);
protected:
    bool eventFilter (QObject *obj, QEvent *event);
public:

    std::vector<QString> getCommands() const { return commands; }
    void addCommand (QString& value) { commands.push_back(value);}
    QString formatLiteralString(QString value);

    explicit Calculator(UTComputer *parent = 0);

    std::vector<QString> getCommands() { return commands; }
public slots:
    void refreshPile();
    void addDigitToCommand();
    void addSpaceToCommand();
    void addOperatorToCommand();
    void backspaceToCommand();
    void calculate();
    void nbLineDisplayPileChanged(int newValue);
    void displayKeyboardChanged(int newValue);
    void beepMessageChanged(int newValue);
    void openDetailErrorWindow();
    void openMoreOperatorsWindow();
    //void getNextCommande();

};

#endif // CALCULATOR_H
