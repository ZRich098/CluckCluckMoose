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
	vector<Chicken> chickens;
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
	fill(chickens);
}

Chicken Coop::at(int pos) {
	if (pos >= getSize())
		CULogError("Deck position out of bounds");
	return (pos < basics.size()) ? basics.at(pos) : specials.at(pos - basics.size());
}

Chicken Coop::drawSpecial() {
	if (specials.size() == 0)
		CULogError("No special chicken to draw");
	if (!specialShuffled)
		shuffle();
	Chicken& c = specials.back();
	specials.pop_back();
	return c;
}

Chicken Coop::drawBasic() {
	if (basics.size() == 0)
		CULogError("No basic chicken to draw");
	if (!basicShuffled)
		shuffle();
	Chicken& c = basics.back();
	basics.pop_back();
	return c;
}

void Coop::shuffle() {
	if (!specialShuffled) {
		random_shuffle(specials.begin(), specials.end());
		specialShuffled = true;
	}
	if (!basicShuffled) {
		random_shuffle(specials.begin(), specials.end());
		basicShuffled = true;
	}
}

void Coop::add(const Chicken& c, bool shouldShuffle) {
	if (c.isBasic) {
		basics.push_back(c);
		if (shouldShuffle)
			basicShuffled = false;
	}
	else {
		specials.push_back(c);
		if (shouldShuffle)
			specialShuffled = false;
	}
}

void Coop::fill(const vector <Chicken> c) {
	for (const Chicken &ch : c)
		add(ch,true);
}

void Coop::fill(const vector <int> cArray) {
	for (int i : cArray)
		add(Chicken(intToSpecial(i)));
}

string Coop::coopString() const {
	stringstream ss;
	if (!basicShuffled || !specialShuffled) {
		ss << "Unshuffled\n";
	}

	for (int i = 0; i < basics.size(); i++) {
		ss << "Coop Basics " << i + 1 << ": " << basics.at(basics.size() - i - 1).toString().c_str() << "\n";
	}

	for (int i = 0; i < specials.size(); i++) {
		ss << "Coop Specials " << i + 1 << ": " << specials.at(specials.size() - i - 1).toString().c_str() << "\n";
	}

	return ss.str();
}