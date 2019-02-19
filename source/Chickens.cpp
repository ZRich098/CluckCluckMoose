#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Chickens.h"

using namespace std;

// TODO:: Entity Component elements and specials
string elements[] = { "None", "Fire", "Water", "Grass" };
string specials[] = { "None",
					"Draw", // Draw a Card
					"Thicken", //Swap with bottom chicken
					"Mirror", //Become opposing chicken
					"Null", //Nullify opposing chicken
					"Reaper", //Ties with all chickens
					"Cycle" }; //Cycle chicken underneath

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
string Chicken::getChicken() {
    if(!special.compare("None")) { //If Chicken special is none
        return "Basic " + element + " Chicken. ";
	}
	return special + " " + element + " Chicken. ";
}

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
