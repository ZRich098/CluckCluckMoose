#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Chicken.h"

using namespace cugl;

/** Create a new Chicken*/
Chicken::Chicken() {
	Chicken(elements[0], specials[0]);
}

/** Create a Chicken of element e and special s */
Chicken::Chicken(string e, string s) {
	element = e;
	special = s;
}

/** Set a Chicken's element to e and special to s */
void Chicken::setChicken(string e, string s) {
	element = e;
	special = s;
}

/** Returns the element of the Chicken */
string Chicken::getElement() {
	return element;
}

/** Returns the special ability of the Chicken */
string Chicken::getSpecial() {
	return special;
}

/** Returns a string of the Chicken's details */
string Chicken::toString() {
    if(!special.compare("None")) { //If Chicken special is none
        return "Basic " + element + " Chicken. ";
	}
	return special + " " + element + " Chicken. ";
}