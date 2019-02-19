#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Coop.h"

using namespace cugl;

/** Create a new Coop*/
Coop::Coop() {
	refresh();
}

/** Fill the coop with Chickens */
void Coop::refresh() {
	//TODO
	for (int c = 0; c < 5; c++) {
		chickens.push_back(Chicken(elements[1], specials[0])); //basic fire chicken
		chickens.push_back(Chicken(elements[2], specials[0])); //basic water chicken
		chickens.push_back(Chicken(elements[3], specials[0])); //basic grass chicken
	}
}

/** Draw a Chicken, removing it from the Coop*/
Chicken Coop::draw() {
	srand(0); //Actually Seed later
	int pos = rand() % chickens.size();
	Chicken c = chickens.at(pos);
	chickens.erase(chickens.begin() + pos);
	return c;
}

/** Get the size of the Coop*/
int Coop::getSize() {
	return chickens.size();
}

/** Clear the coop of all Chickens*/
void Coop::clear() {
	chickens.clear();
}

/** Fill the coop with Chickens */
void Coop::fill(vector <Chicken> c) {
	//TODO just use a pointer to c instead
	for (Chicken &ch : c) {
		chickens.push_back(ch);
	}
}