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
}

Coop::~Coop() {
	CULog("Coop Destroyed");
}

void Coop::loadDeck() {
	switch (deckNumber) {
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
	if (chickens.empty()) {
		loadDeck();
	}
	if (!shuffled) {
		random_shuffle(chickens.begin(), chickens.end());
		shuffled = true;
	}
	Chicken& c = chickens.back();
	chickens.pop_back();
	return c;
}

int Coop::getSize() {
	return chickens.size();
}

void Coop::clear() {
	chickens.clear();
}

void Coop::shuffle() {
	random_shuffle(chickens.begin(), chickens.end());
	shuffled = true;
}

void Coop::add(Chicken& c) {
	chickens.push_back(c);
}

void Coop::fill(vector <Chicken> c) {
	//TODO just use a pointer to c instead
	for (Chicken &ch : c) {
		chickens.push_back(ch);
	}
}