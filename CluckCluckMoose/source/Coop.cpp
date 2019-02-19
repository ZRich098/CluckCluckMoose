#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Coop.h"

using namespace cugl;

Coop::Coop() {
	refresh();
}

void Coop::refresh() {
	//TODO an actual deck
	for (int c = 0; c < 5; c++) {
		chickens.push_back(Chicken(elements[1], specials[0])); //basic fire chicken
		chickens.push_back(Chicken(elements[2], specials[0])); //basic water chicken
		chickens.push_back(Chicken(elements[3], specials[0])); //basic grass chicken
	}
}

Chicken Coop::draw() {
	srand(time(NULL));
	int pos = rand() % chickens.size();
	Chicken c = chickens.at(pos);
	chickens.erase(chickens.begin() + pos);
	return c;
}

int Coop::getSize() {
	return chickens.size();
}

void Coop::clear() {
	chickens.clear();
}

void Coop::fill(vector <Chicken> c) {
	//TODO just use a pointer to c instead
	for (Chicken &ch : c) {
		chickens.push_back(ch);
	}
}