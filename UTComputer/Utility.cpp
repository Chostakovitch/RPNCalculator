﻿#include "Utility.h"
#include <cctype>
#include <algorithm>
#include <cstdlib> //avec <cmath>, problème d'ambiguité sur certains compilateurs
#include <cctype>

unsigned int Utility::computeGcd(int num, int den) {
	unsigned int r;
	num = std::abs(num);
	den = std::abs(den);
	while (den != 0) {
		r = num % den;
		num = den;
		den = r;
	}
	return num;
}

std::string Utility::getOutside(std::string s, char left, char right) {
    unsigned int leftPos = s.find_first_of(left);
    unsigned int rightPos = s.find_last_of(right);
    //Il existe une sous-chaîne délimitée
    if(leftPos != std::string::npos && rightPos != std::string::npos) {
        return s.substr(0, leftPos) + ' ' + s.substr(rightPos, s.size() - rightPos);
    }
    return s;
}

bool Utility::isSymbol(char c) {
    return !std::isalnum(c) && std::isgraph(c);
}
