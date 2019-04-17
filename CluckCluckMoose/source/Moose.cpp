//
//  Moose.cpp
//

#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Moose.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Constructors

void Moose::refillDeck() {
	deck.fill(discard);
	discard.clear();
}

/**
 * Initializes a new moose with the given health and hand size.
 *
 * @return  true if the Moose is initialized properly, false otherwise.
 */
bool Moose::init(int h, int hSize) {
    health = h;
    handSize = hSize;
	numChickensWillDiePreview = 0;
    return true;
}


void Moose::jsonInit(int h, vector<int> handArray, vector<int> playOrderArray, vector<int> coopArray, vector<int> discardArray, string cost) {
	health = h;
	for (int i : handArray) {
		hand.push_back(Chicken(intToSpecial(i)));
	}

	for (int i : playOrderArray) {
		playOrder.push_back(Chicken(intToSpecial(i)));
	}

	deck.clear();
	deck.fill(coopArray);

	for (int i : discardArray) {
		discard.push_back(Chicken(intToSpecial(i)));
	}

	//not yet supported
	//costume = cost;
}


/**
 * Disposes all resources and assets of this moose
 *
 * Does not currently actually dispose all assets!
 */
void Moose::dispose() {
    health = 0;
    handSize = 0;
    // deck = Coop(4);
}


vector<int> Moose::getChickenElementDistribution() {
	int fire = 0;
	int water = 0;
	int grass = 0;
	int other = 0;

	for (Chicken c : hand) {
		switch (c.getElement()) {
		case element::Fire:
			fire += 1;
			break;
		case element::Water:
			water += 1;
			break;
		case element::Grass:
			grass += 1;
			break;
		default:
			int random = rand() % 3;
			if (random == 0)
				fire += 1;
			if (random == 1)
				water += 1;
			if (random == 2)
				grass += 1;
			break;
		}
	}

	vector<int> res;
	res.push_back(fire);
	res.push_back(water);
	res.push_back(grass);
	res.push_back(other);

	return res;
}


#pragma mark -
#pragma mark Stack

void Moose::addToStackFromHand(int pos) {
	//CULog("playing %s", hand.at(pos).toString().c_str());
	stack.add(hand.at(pos));
	hand.erase(hand.begin() + pos);
	playOrder.push_back(getStack().getTop());
}

void Moose::removeTopFromStackToHand() {
	hand.push_back(stack.removeTop());
}

void Moose::discardStack() {
	while (playOrder.size() > 0) {
		switch (playOrder.front().getSpecial()) {
			case special::BasicFire:
			case special::BasicGrass:
			case special::BasicWater:
				break;
			default:
				discard.push_back(playOrder.front());
		}
		playOrder.erase(playOrder.begin());
	}
	playOrder.clear();
}

/*void Moose::clearStackToDiscard() {
	while(stack.getSize() > 0){
		discard.push_back(stack.removeBottom());
	}
	stack.clear();
}*/

void Moose::setStack(Stack s) {
	stack.clear();
	while ((s.getSize() > 0)) {
		stack.add(s.getBottom());
		s.removeBottom();
	}
}

void Moose::clearHandToDiscard() {
	for (Chicken &c : hand) {
		switch (c.getSpecial()) {
		case special::BasicFire:
		case special::BasicGrass:
		case special::BasicWater:
			break;
		default:
			discard.push_back(c);
		}
	}
	hand.clear();
}

void Moose::refillHand() {
	//Draw from deck while there are still chickens in the deck and hand is not full
	if (hand.size() == 0) {
		hand.push_back(Chicken(element::Fire, special::BasicFire));
		hand.push_back(Chicken(element::Grass, special::BasicGrass));
		hand.push_back(Chicken(element::Water, special::BasicWater));
	}
	random_shuffle(hand.begin(),hand.end());
	if (handPool.size() != 0) {
		//Uses hands that are given if one is available
		random_shuffle(handPool.begin(), handPool.end());
		for (Chicken i : handPool.front()) {
			hand.push_back(i);
		}
	}
	else {
		hand.push_back(Chicken(hand.front().getElement(), hand.front().getSpecial()));
		while (hand.size() < handSize) {
			//refill deck if hand not full yet but deck is empty
			//if (deck.getSize() == 0) refillDeck();
			hand.push_back(deck.draw());
		}
		// Pool system
		refillDeck();
		deck.shuffle();
	}
}

void Moose::refillOppHand(vector<vector<Chicken>> cardPool) {
	if (hand.size() == 0) {
		hand.push_back(Chicken(element::Fire, special::BasicFire));
		hand.push_back(Chicken(element::Grass, special::BasicGrass));
		hand.push_back(Chicken(element::Water, special::BasicWater));
	}
	random_shuffle(cardPool.begin(), cardPool.end());
	for (Chicken i : cardPool.front()) {
		hand.push_back(i);
	}
}

void Moose::draw(int num) {
	for (int i = 0; i < num; i++) {
		//refill deck if empty
		if (deck.getSize() == 0) refillDeck();
		Chicken c = deck.draw();
		if (hand.size() < handSize) {
			hand.push_back(c);
		} else {
			discard.push_back(c);
		}
	}
}

string Moose::handString() const {
	stringstream ss;
	for (int i = 0; i < hand.size(); i++) {
		ss << "Hand " << i + 1 << ": " << hand.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	return ss.str();
}

string Moose::playOrderString() const {
	stringstream ss;
	for (int i = 0; i < playOrder.size(); i++) {
		ss << "PlayOrder " << i + 1 << ": " << playOrder.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	return ss.str();
}

string Moose::discardString() const {
	stringstream ss;
	for (int i = 0; i < discard.size(); i++) {
		ss << "Discard " << i + 1 << ": " << discard.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	return ss.str();
}

string Moose::mooseString() const {
	stringstream ss;
	ss << "Moose Info:\n Health " << health << "\n" <<
		handString() << stackString() << discardString() <<
		deck.coopString();
	return ss.str();
}