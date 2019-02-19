#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Chicken.h"

using namespace cugl;

Chicken::Chicken() {
	Chicken(elements[0], specials[0]);
}

Chicken::Chicken(string e, string s) {
	element = e;
	special = s;
}

void Chicken::setChicken(string e, string s) {
	element = e;
	special = s;
}

string Chicken::getElement() {
	return element;
}

string Chicken::getSpecial() {
	return special;
}

string Chicken::toString() {
    if(!special.compare("None")) { //If Chicken special is none
        return "Basic " + element + " Chicken. ";
	}
	return special + " " + element + " Chicken. ";
}