#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Coop.h"
#include "Chicken.h"

using namespace std;

/** Create a new Coop*/
Coop::Coop() {
	//TODO
	for (int c = 0; c < 5; c++) {
		chickens.push_back(Chicken(elements[1], specials[0])); //basic fire chicken
		chickens.push_back(Chicken(elements[2], specials[0])); //basic water chicken
		chickens.push_back(Chicken(elements[3], specials[0])); //basic grass chicken
	}
}

/** Draw a Chicken from the Coop*/
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
