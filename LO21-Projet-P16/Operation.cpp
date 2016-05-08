#include "Operation.h"

Arguments<std::shared_ptr<Literal>> Operation::eval(LiteralFactory& factory, Arguments<IntegerLiteral>& args) { 
	throw std::invalid_argument("Not implemented."); 
}
Arguments<std::shared_ptr<Literal>> Operation::eval(LiteralFactory& factory, Arguments<RationalLiteral>& args) { 
	throw std::invalid_argument("Not implemented."); 
}
Arguments<std::shared_ptr<Literal>> Operation::eval(LiteralFactory& factory, Arguments<RealLiteral>& args) { 
	throw std::invalid_argument("Not implemented."); 
}
Arguments<std::shared_ptr<Literal>> Operation::eval(LiteralFactory& factory, Arguments<ExpressionLiteral>& args) { 
	throw std::invalid_argument("Not implemented."); 
}

Arguments<std::shared_ptr<Literal>> PlusOperation::eval(LiteralFactory& factory, Arguments<IntegerLiteral>& args) {
	return Arguments<std::shared_ptr<Literal>>{factory.makeLiteral(args.at(0).getValue() + args.at(1).getValue())};
}

Arguments<std::shared_ptr<Literal>> PlusOperation::eval(LiteralFactory& factory, Arguments<RationalLiteral>& args) {
	IntegerLiteral a = args.at(0).getNum(), b = args.at(0).getDen(), c = args.at(1).getNum(), d = args.at(1).getDen();
	int e = a.getValue() * d.getValue() + b.getValue() * c.getValue(), f = b.getValue() * d.getValue();
	return Arguments<std::shared_ptr<Literal>>{factory.makeLiteral(e, f)};
}

Arguments<std::shared_ptr<Literal>> PlusOperation::eval(LiteralFactory& factory, Arguments<RealLiteral>& args) {
	return Arguments<std::shared_ptr<Literal>>{factory.makeLiteral(args.at(0).getValue() + args.at(1).getValue())};
}

Arguments<std::shared_ptr<Literal>> PlusOperation::eval(LiteralFactory& factory, Arguments<ExpressionLiteral>& args) {
	std::string res = "(" + args.at(0).getExpression() + ")+(" + args.at(1).getExpression() + ")";
	return Arguments<std::shared_ptr<Literal>>{factory.makeLiteral(res)};
}