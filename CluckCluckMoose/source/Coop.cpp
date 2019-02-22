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

Coop::~Coop() {
	CULog("Coop Destroyed");
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
	if (chickens.empty()) {
		loadDeck();
		random_shuffle(chickens.begin(), chickens.end());
		shuffled = true;
	}
	if (!shuffled) {
		random_shuffle(chickens.begin(), chickens.end());
		shuffled = true;
	}
	Chicken c = chickens.back();
	CULog("chicken %s, deck size %i", c.toString(), chickens.size());	//CULog("Chicken at %i is %s", pos, c.toString());
	chickens.pop_back();
	//CULog("chicken %s", c, chickens.size());	//CULog("Chicken at %i is %s", pos, c.toString());
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