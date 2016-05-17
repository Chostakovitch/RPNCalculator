/**
  * @file ExpressionParser.h
  * @brief Fichier de référence pour le parsing d'expressions arithmétiques.
  */

#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include <string>
#include <memory>
#include <stack>
#include <queue>

class Operand;
class Literal;
class Operator;

/**
 * @brief Un objet ExpressionParser est un objet utilitaire de parsing d'expressions arithmétiques.
 * @details Cette classe utilise l'algorithme Shunting-yard. Il permet, étant donné une chaîne de caractères en notation infixe,
 * de produire un ensemble d'objets Operand en notation postfixe.
 */
class ExpressionParser
{
    /**
     * @brief Chaîne de caractères à parser, sans espaces, en notation infixe.
     */
    std::string expr;
    /**
     * @brief Délimiteur de sous-expression à gauche.
     */
    char left_del;
    /**
     * @brief Délimiteur de sous-expression à droite
     */
    char right_del;
    /**
     * @brief Pile temporaire servant au parsing.
     */
    std::stack<std::shared_ptr<Operand>> stack;
    /**
     * @brief File de sortie postfixe.
     */
    std::queue<std::shared_ptr<Operand>> queue;
public:
    /**
     * @brief Constructeur standard.
     * @param s Chaîne à parser, sans espaces, en notation infixe.
     */
    explicit ExpressionParser(const std::string& s, char left_del = '(', char right_del = ')') \
        : expr(s), left_del(left_del), right_del(right_del), stack(std::stack<std::shared_ptr<Operand>>()), queue(std::queue<std::shared_ptr<Operand>>()) { }
    /**
     * @brief Lance la transformation en suite d'opérandes en notation postfixe.
     * @details Tous les opérateurs symboliques sont associatifs à gauche. On ne considère pas les cas d'opérateurs
     * associatifs à droite qui seront des fonctions.
     */
    void parse();
    /**
     * @brief Consume une entité dans l'expression courante.
     * @return Chaîne de caractère correspondant à une entité.
     */
    std::string readToken();
    /**
     * @brief Fabrique un objet Literal correspondant au jeton d'entrée.
     * @param token Chaîne de caractères représentant une littérale __numérique__, ou bien son identificateur.
     * @exception invalid_argument si token ne représente pas une littérale numérique.
     * @return Pointeur sur Literal.
     */
    std::shared_ptr<Literal> getLiteral(std::string token);
    /**
     * @brief Récupère un objet Operator __parenthésé__ existant.
     * @param token Chaîne de caractères représentant un opérateur fonction.
     * @exception invalid_argument si token ne représente pas une fonction.
     * @return Pointeur sur Operator.
     */
    std::shared_ptr<Operator> getFunction(std::string token);
    /**
     * @brief Récupère un objet Operator __symbolique__ existant.
     * @param token Chaîne de caractères représentant un opérateur infixe.
     * @exception invalid_argument si token ne représente pas un opération symbolique.
     * @return Pointeur sur Operator.
     */
    std::shared_ptr<Operator> getOperator(std::string token);
};

#endif // EXPRESSIONPARSER_H
