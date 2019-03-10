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
	//CULog("Coop Destroyed");
}

void Coop::loadDeck() {
	switch (deckNumber) {
		case 0: //Empty Deck
			break;
		case 1:
			for (special c : deckV1)
				chickens.push_back(Chicken(c));
			break;
		case 2:
			for (special c : deckV2)
				chickens.push_back(Chicken(c));
			break;
		case 3:
			for (special c : deckV3)
				chickens.push_back(Chicken(c));
			break;
		case 4:
			for (special c : deckV4)
				chickens.push_back(Chicken(c));
			break;
	}
	shuffled = false;
}

Chicken Coop::at(int pos) {
	return chickens.at(chickens.size() - pos);
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
	for (const Chicken &ch : c) {
		chickens.push_back(ch);
	}
	shuffled = false;
}

void Coop::fill(const vector <int> cArray) {
	for (int i : cArray) {
		chickens.push_back(Chicken(intToSpecial(i)));
	}
	shuffled = false;
}

string Coop::coopString() const {
	stringstream ss;
	if (!shuffled) {
		ss << "Unshuffled\n";
	}

	for (int i = 0; i < chickens.size(); i++) {
		ss << "Coop " << i + 1 << ": " << chickens.at(chickens.size() - i - 1).toString().c_str() << "\n";
	}

	return ss.str();
}