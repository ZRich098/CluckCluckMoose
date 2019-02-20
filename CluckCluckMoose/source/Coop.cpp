#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Coop.h"
#include "Decks.h"

using namespace cugl;

Coop::Coop() {
	loadDeck();
}

void Coop::loadDeck() {
	loadDeck(4);
}

void Coop::loadDeck(int versionNumber) {
	switch (versionNumber) {
	case 1:
		for (Chicken c : deckV1)
			chickens.push_back(c);
		return;
	case 2:
		for (Chicken c : deckV2)
			chickens.push_back(c);
		return;
	case 3:
		for (Chicken c : deckV3)
			chickens.push_back(c);
		return;
	case 4:
		for (Chicken c : deckV4)
			chickens.push_back(c);
		return;
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