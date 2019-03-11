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
    // [Amanda] hi i didn't know what this did so i commented it out for now
    // deck = Coop(4);
    return true;
}


void Moose::jsonInit(int h, vector<int> handArray, vector<int> stackArray, vector<int> coopArray, vector<int> discardArray, string cost) {
	health = h;
	for (int i : handArray) {
		hand.push_back(Chicken(intToSpecial(i)));
	}

	for (int i : stackArray) {
		stack.add(Chicken(intToSpecial(i)));
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


#pragma mark -
#pragma mark Stack

void Moose::addToStackFromHand(int pos) {
	CULog("playing %s", hand.at(pos).toString().c_str());
	stack.add(hand.at(pos));
	hand.erase(hand.begin() + pos);
}

void Moose::removeTopFromStackToHand() {
	hand.push_back(stack.removeTop());
}

void Moose::removeBottomFromStackToDiscard() {
	discard.push_back(stack.removeBottom());
}

void Moose::clearStackToDiscard() {
	while(stack.getSize() > 0){
		discard.push_back(stack.removeBottom());
	}
	stack.clear();
}

void Moose::setStack(Stack s) {
	stack.clear();
	while ((s.getSize() > 0)) {
		stack.add(s.getBottom());
		s.removeBottom();
	}
}

void Moose::clearHandToDiscard() {
	for (Chicken &c : hand) {
		discard.push_back(c);
	}
	hand.clear();
}

void Moose::refillHand() {
	//Draw from deck while there are still chickens in the deck and hand is not full
	while (hand.size() < handSize) {
		//refill deck if hand not full yet but deck is empty
		if (deck.getSize() == 0) refillDeck();
		hand.push_back(deck.draw());
	}
}

void Moose::draw(int num) {
	for (int i = 0; i < num; i++) {
		//refill deck if empty
		if (deck.getSize() == 0) refillDeck();
		Chicken c = deck.draw();
		if (hand.size() < handSize - 1) {
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
