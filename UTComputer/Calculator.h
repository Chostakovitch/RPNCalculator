#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDebug>

class Button;

class Calculator : public QWidget
{
private:
    Q_OBJECT
    QLineEdit* message;
    QTableWidget* viewPile;
    QLineEdit* commande;
    QVBoxLayout* layout;
    //Pile* pile;
    //Controleur* controleur;

public:
    explicit Calculator(QWidget *parent = 0);

public slots:
    //void refresh();
    //void getNextCommande();

};

#endif // CALCULATOR_H