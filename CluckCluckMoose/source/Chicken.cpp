#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Chicken.h"

using namespace cugl;

Chicken::Chicken() {
	Chicken(element::None, special::None);
}

Chicken::Chicken(element el, special sp) {
	e = el;
	s = sp;
}

void Chicken::setChicken(element el, special sp) {
	e = el;
	s = sp;
}

element Chicken::getElement() {
	return e;
}

special Chicken::getSpecial() {
	return s;
}

string eString(element e) {
	switch (e) {
		case element::None:
			return "None";
		case element::Fire:
			return "Fire";
		case element::Water:
			return "Water";
		case element::Grass:
			return "Grass";
	}
}

string sString(special s) {
	switch (s) {
	case special::None:
		return "None";
	case special::Draw:
		return "Draw";
	case special::Thicken:
		return "Thicken";
	case special::Mirror:
		return "Mirror";
	case special::Null:
		return "Null";
	case special::Reaper:
		return "Reaper";
	case special::Cycle:
		return "Cycle";
	}
}

string Chicken::toString() {
    if(s == special::None) { //If Chicken special is none
        return "Basic " + eString(e) + " Chicken. ";
	}
	return sString(s) + " " + eString(e) + " Chicken. ";
}