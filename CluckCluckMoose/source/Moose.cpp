#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Moose.h"

using namespace cugl;

/** Create a new Moose*/
Moose::Moose() {
	health = 5;
}


/** Get the current health of the Moose*/
int Moose::getHealth() {
	return health;
}

/** Set the current health of the Moose*/
void Moose::setHealth(int h) {
	health = h;
}

/** The moose loses $damage$ points of health*/
void Moose::takeDamage(int damage) {
	health = health - damage;
}