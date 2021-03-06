﻿/**
  * @file Literal.h
  * @brief Fichier de référence pour les littérales simples.
  */

#ifndef LITERAL_H
#define LITERAL_H
#include <string>
#include <memory>
#include <complex>

class IntegerLiteral;
class RationalLiteral;
class RealLiteral;
class ExpressionLiteral;
class ComplexLiteral;
class ProgramLiteral;

/**
 * @brief Un objet Operand représente une littérale quelconque ou bien un opérateur.
 * @details Toute entrée individuelle valide sur la ligne de commande d'UTComputer est considérée comme un objet Operand.
 */
class Operand { 
public:
    /**
     * @brief Méthode permettant d'obtenir la représentation textuelle d'un objet Operand.
     * @return Une chaîne de caractères.
     */
	virtual std::string toString() const = 0;
    /**
     * @brief Ecriture sur un flux de la représentation de l'opérande.
     * @param Flux de sortie.
     * @param Operande à représenter.
     * @return Flux de sortie écrit.
     */
    friend std::ostream& operator<<(std::ostream& os, const Operand& op) { os << op.toString(); return os; }
    /**
     * @brief Destructeur virtuel.
     */
    virtual ~Operand() { }
};

/**
 * @brief Un objet Literal représente une donnée numérique ou textuelle manipulable par un ou plusieurs opérateurs.
 * @details Une littérale peut être composée d'autres littérales ou ne pas être exploitable en l'état. Ce sont les
 * opérateurs qui leur donne une signification.
 */
class Literal : public Operand {
public:
    /**
     * @brief Opérateur de cast d'un objet Literal en objet IntegerLiteral.
     * @details Echoue si non re-défini dans une classe fille.
     * @exception TypeError si non-implémenté.
     */
    virtual operator IntegerLiteral() const;
    /**
     * @brief Opérateur de cast d'un objet Literal en objet RationalLiteral.
     * @details Echoue si non re-défini dans une classe fille.
     * @exception TypeError si non-implémenté.
     */
    virtual operator RationalLiteral() const;
    /**
     * @brief Opérateur de cast d'un objet Literal en objet RealLiteral.
     * @details Echoue si non re-défini dans une classe fille.
     * @exception TypeError si non-implémenté.
     */
    virtual operator RealLiteral() const;
    /**
     * @brief Opérateur de cast d'un objet Literal en objet ComplexLiteral.
     * @details Echoue si non re-défini dans une classe fille.
     * @exception TypeError si non-implémenté.
     */
    virtual operator ComplexLiteral() const;
    /**
     * @brief Opérateur de cast d'un objet Literal en objet ExpressionLiteral.
     * @details Echoue si non re-défini dans une classe fille.
     * @exception TypeError si non-implémenté.
     */
    virtual operator ExpressionLiteral() const;
    /**
     * @brief Opérateur de cast d'un objet Literal en objet ProgramLiteral.
     * @details Echoue si non re-défini dans une classe fille.
     * @exception TypeError si non-implémenté.
     */
    virtual operator ProgramLiteral() const;
};

/**
 * @brief Un objet NumericLiteral représente n'importe quel type de nombre.
 */
class NumericLiteral : public Literal {
public:
    /**
     * @brief Opérateur de cast d'un objet NumericLiteral en objet ExpressionLiteral.
     * @details Toute littérale numérique peut être représentée sous la forme d'une
     * littérale expression via sa méthode toString.
     */
    operator ExpressionLiteral() const override;
};

/**
 * @brief Un objet IntegerLiteral représente un entier relatif.
 */
class IntegerLiteral : public NumericLiteral {
    /**
     * @brief Valeur de l'entier.
     */
	int value;
public:
    /**
     * @brief Construteur explicite d'objet IntegerLiteral.
     * @param value Un entier relatif.
     */
	explicit IntegerLiteral(int value) : value(value) {}
    /**
     * @brief Accesseur pour la valeur de la littérale.
     * @return Un entier relatif.
     */
	int getValue() const { return value; }
    /**
     * @brief Opérateur de cast d'un objet IntegerLiteral en objet RationalLiteral.
     * @details La valeur de la littérale entière devient numérateur de la littérale rationnelle
     * dont le dénominateur est fixé à 1, il n'y a pas de perte d'information.
     */
    operator RationalLiteral() const override;
    /**
     * @brief Opérateur de cast d'un objet IntegerLiteral en objet RealLiteral.
     * @details La valeur de la littérale entière devient la partie entière de la littérale réelle dont
     * la mantisse est fixée à 0, il n'y a pas de perte d'information.
     */
    operator RealLiteral() const override;
    /**
     * @brief Cast d'un objet IntegerLiteral en objet ComplexLiteral.
     * @details L'objet courant est défini comme partie réelle du ComplexLiteral, sa partie imaginaire est mise à 0.
     */
    operator ComplexLiteral() const override;
    /**
     * @brief Cast d'un objet IntegerLiteral en int.
     */
    operator int() const { return value; }
    std::string toString() const override { return std::to_string(value); }
};

/**
 * @brief Un objet RationalLiteral représente une fraction rationnelle.
 */
class RationalLiteral : public NumericLiteral {
    /**
     * @brief Objet IntegerLiteral représentant le numérateur de la littérale rationnelle.
     */
    IntegerLiteral num;
    /**
     * @brief Objet IntegerLiteral représentant le dénominateur de la littérale rationnelle.
     */
    IntegerLiteral den;
public:
    /**
     * @brief Constructeur d'objet RationalLiteral.
     * @param num Objet IntegerLiteral représentant le numérateur.
     * @param den Objet IntegerLiteral représentant le dénominateur.
     * @details Dans un souci de modularité, la simplification n'est pas faite dans le constructeur.
     * Elle est faite dans la fabrique, pour que la classe puisse être ré-utilisée sans cette contrainte forte.
     */
    RationalLiteral(int num, int den) : num(IntegerLiteral(num)), den(IntegerLiteral(den)) {}
    /**
     * @brief Accesseur pour le numérateur.
     * @return Un objet IntegerLiteral.
     */
	const IntegerLiteral& getNum() const { return num; }
    /**
     * @brief Accesseur pour le dénominateur
     * @return Un objet IntegerLiteral.
     */
	const IntegerLiteral& getDen() const { return den; }
    /**
     * @brief Cast d'un objet RationalLiteral en objet RealLiteral.
     * @details La valeur du réel est obtenue par division flottante du numérateur et du dénominateur.
     */
    operator RealLiteral() const override;
    /**
     * @brief Cast d'un objet RationalLiteral en objet ComplexLiteral.
     * @details L'objet courant est défini comme partie réelle du ComplexLiteral, sa partie imaginaire est mise à 0.
     */
    operator ComplexLiteral() const override;
    /**
     * @brief Cast d'un objet RationalLiteral en double, par division de ses deux parties.
     */
    operator double() const { return (double)num / (double)den; }
    std::string toString() const override { return num.toString() + '/' + den.toString(); }
};

/**
 * @brief Un objet RealLiteral représente un nombre réel.
 */
class RealLiteral : public NumericLiteral {
    /**
     * @brief Valeur du réel.
     */
	double value;
public:
    /**
     * @brief Constructeur explicite d'objet RealLiteral.
     * @param value Valeur du réel.
     */
    explicit RealLiteral(double value) : value(value) {}
    /**
     * @brief Accesseur pour la valeur du réel.
     * @return Un nombre à virgule flottante.
     */
    double getValue() const { return value; }
    /**
     * @brief Cast d'un objet RealLiteral en objet ComplexLiteral.
     * @details L'objet courant est défini comme partie réelle du ComplexLiteral, sa partie imaginaire est mise à 0.
     */
    operator ComplexLiteral() const override;
    /**
     * @brief Cast d'un objet RealLiteral en double.
     */
    operator double() const { return value; }
    std::string toString() const override { return std::to_string(value); }
};

/**
 * @brief Un objet ComplexLiteral représente un nombre complexe.
 * @details Cette classe ne compose pas de littérales numériques car il s'agit d'un type abstrait.
 * Elle contient uniquement des pointeurs, permettant l'application du polymorphisme.
 */
class ComplexLiteral : public NumericLiteral {
    /**
     * @brief Pointeur sur un nombre représentant la partie réelle.
     */
    std::shared_ptr<NumericLiteral> re;
    /**
     * @brief Pointeur sur un nombre représentant la partie imaginaire.
     */
    std::shared_ptr<NumericLiteral> im;
public:
    /**
     * @brief Constructeur d'objet ComplexLiteral.
     * @param re Pointeur sur la partie réelle.
     * @param im Pointeur sur la partie imaginaire.
     */
	ComplexLiteral(std::shared_ptr<NumericLiteral> re, std::shared_ptr<NumericLiteral> im) : re(re), im(im) { }
    /**
     * @brief Accesseur pour la partie réelle.
     * @return Référence constante sur nombre.
     */
    const std::shared_ptr<NumericLiteral>& getRe() const { return re; }
    /**
     * @brief Accesseur pour la partie imaginaire
     * @return Référence constante sur nombre.
     */
    const std::shared_ptr<NumericLiteral>& getIm() const { return im; }
    /**
     * @brief Opérateur de cast vers un complexe de la librairie standard.
     * @details On considère qu'un complexe est au plus composé de doubles, par conséquent on
     * caste la partie réelle et la partie imaginaire en réels.
     */
    operator std::complex<double>() const { return std::complex<double>((RealLiteral)*re, (RealLiteral)*im); }
    std::string toString() const override { return re->toString() + "$" + im->toString(); }
};

/**
 * @brief Un objet ExpressionLiteral représente une suite d'opérandes ordonnées.
 * @details Cette suite d'opérandes est destinée à produire un résultat numérique lors d'une évaluation externe.
 * En pratique, il ne s'agit que d'une simple chaîne de caractères car elle pourait contenir des références à des éléments
 * non-définis, qui le seront lors de l'évaluation.
 */
class ExpressionLiteral : public Literal {
    /**
     * @brief Représentation de l'expression.
     */
	std::string expr;
public:
    /**
     * @brief Constructeur d'objet ExpressionLiteral
     * @param expr L'expression.
     */
    explicit ExpressionLiteral(std::string expr) : expr(expr) {}
    /**
     * @brief Accesseur pour l'expression.
     * @return Chaîne de caractères.
     */
    std::string getExpression() const { return expr; }
    std::string toString() const override { return "\"" + expr + "\""; }
};
#endif
