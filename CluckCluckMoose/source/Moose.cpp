#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Moose.h"

using namespace cugl;

void Moose::refillDeck() {
	deck.fill(discard);
	discard.clear();
}

Moose::Moose(int h, int hsize, int dNum) {
	health = h;
	handSize = hsize;
	deck = Coop(dNum);
}

Moose::~Moose() {
	//CULog("Moose Destroyed");
}


void Moose::changeChickenInStack(element e, special s, int d, int pos) {
	stack.at(pos).setChicken(e, s, d);
}

void Moose::changeChickenInStackElement(element e, int pos) {
	Chicken &c = stack.at(pos);
	c.setChicken(e, c.getSpecial());
}

void Moose::changeChickenInStackSpecial(special s, int pos) {
	Chicken &c = stack.at(pos);
	c.setChicken(c.getElement(), s);
}

void Moose::changeChickenInStackDamage(int d, int pos) {
	Chicken &c = stack.at(pos);
	c.setChicken(c.getElement(), c.getSpecial(), d);
}

void Moose::addToStackFromHand(int pos) {
	stack.push_back(hand.at(pos));
	hand.erase(hand.begin() + pos);
}

void Moose::removeTopFromStackToHand() {
	hand.push_back(stack.back());
	stack.pop_back();	
}

void Moose::removeAllFromStackToHand() {
	for (Chicken &c : stack) {
		hand.push_back(c);
	}
	stack.clear();
}

void Moose::removeBottomFromStackToDiscard() {
	discard.push_back(stack.front());
	stack.erase(stack.begin());
}

void Moose::clearStackToDiscard() {
	for (Chicken &c : stack) {
		discard.push_back(c);
	}
	stack.clear();
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
		Chicken &c = deck.draw();
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

string Moose::stackString() const {
	stringstream ss;
	for (int i = 0; i < stack.size(); i++) {
		ss << "Stack " << i + 1 << ": " << stack.at(i).toString().c_str() << "\n";
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