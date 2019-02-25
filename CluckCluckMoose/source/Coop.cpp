#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Coop.h"
#include "Decks.h"

using namespace cugl;

Coop::Coop(int dN) {
	deckNumber = dN;
	loadDeck();
	srand(time(NULL)); //seed RNJesus
}

Coop::~Coop() {
	CULog("Coop Destroyed");
}

void Coop::loadDeck() {
	switch (deckNumber) {
		case 0: //Empty Deck
			break;
		case 1:
			for (ChickenType c : deckV1)
				chickens.push_back(Chicken(c));
			break;
		case 2:
			for (ChickenType c : deckV2)
				chickens.push_back(Chicken(c));
			break;
		case 3:
			for (ChickenType c : deckV3)
				chickens.push_back(Chicken(c));
			break;
		case 4:
			for (ChickenType c : deckV4) 
				chickens.push_back(Chicken(c));
			break;
	}
	shuffled = false;
}

Chicken Coop::draw() {
	if (!shuffled) {
		random_shuffle(chickens.begin(), chickens.end());
		shuffled = true;
	}
	Chicken& c = chickens.back();
	chickens.pop_back();
	return c;
}

void Coop::shuffle() {
	random_shuffle(chickens.begin(), chickens.end());
	shuffled = true;
}

void Coop::add(const Chicken& c) {
	chickens.push_back(c);
}

void Coop::fill(const vector <Chicken> c) {
	//TODO just use a pointer to c instead
	for (const Chicken &ch : c) {
		chickens.push_back(ch);
	}
}

string Coop::printCoop() const {
	stringstream ss;
	if (!shuffled) {
		ss << "Unshuffled\n";
	}

	for (int i = 0; i < chickens.size(); i++) {
		ss << "Coop " << i << ": " << chickens.at(chickens.size() - i - 1).toString().c_str() << "\n";
	}

	return ss.str();
}