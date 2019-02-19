#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Coop.h"

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

//TODO Json decks instead of hard coding

Chicken deckV1 [30] = { //actually 30 but 3 deprecated chicken types
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),

	Chicken(element::Water, special::Ninja),
	Chicken(element::Water, special::Ninja),
	Chicken(element::Fire, special::Thicken),
	Chicken(element::Fire, special::Thicken),
	Chicken(element::Grass, special::SelfSwap),
	Chicken(element::Grass, special::SelfSwap),
	Chicken(element::Water, special::Reaper),
	Chicken(element::Water, special::Reaper),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Fire, special::Clash),
	Chicken(element::Fire, special::Clash),
	Chicken(element::Grass, special::Discard),
	Chicken(element::Grass, special::Discard)
};

Chicken deckV2[28] = { //actually 30 but 2 deprecated chicken types
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),

	Chicken(element::Water, special::Cycle),
	Chicken(element::Water, special::Cycle),
	//2 fusion chicken
	Chicken(element::Water, special::Ninja),
	Chicken(element::Water, special::Ninja),
	Chicken(element::Fire, special::Bomb),
	Chicken(element::Fire, special::Bomb),
	Chicken(element::Fire, special::CycleAll),
	Chicken(element::Fire, special::CycleAll),
	Chicken(element::Water, special::Reaper),
	Chicken(element::Water, special::Reaper),
	Chicken(element::Grass, special::Draw),
	Chicken(element::Grass, special::Draw),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Grass, special::SelfSwap),
	Chicken(element::Grass, special::SelfSwap),
	Chicken(element::Fire, special::Thicken),
	Chicken(element::Fire, special::Thicken)
};

Chicken deckV3[18] = {
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),

	Chicken(element::Fire, special::Thicken),
	Chicken(element::Grass, special::Ninja),
	Chicken(element::Water, special::Null),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Water, special::Reaper),
	Chicken(element::Grass, special::Draw),
	Chicken(element::Fire, special::Bomb),
	Chicken(element::Water, special::Cycle),
	Chicken(element::Grass, special::Peek)
};

Chicken deckV4[30] = {
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),

	Chicken(element::Water, special::Cycle),
	Chicken(element::Water, special::Extra),
	Chicken(element::Grass, special::Ninja),
	Chicken(element::LoseAll, special::Bomb),
	Chicken(element::Fire, special::Search),
	Chicken(element::TieAll, special::Reaper),
	Chicken(element::Grass, special::Draw),
	Chicken(element::Water, special::Refresh),
	Chicken(element::Water, special::Null),
	Chicken(element::Unset, special::Mirror),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Fire, special::Hide),
	Chicken(element::Fire, special::Thicken),
	Chicken(element::LoseAll, special::Draw2),
	Chicken(element::Grass, special::Discard)
};