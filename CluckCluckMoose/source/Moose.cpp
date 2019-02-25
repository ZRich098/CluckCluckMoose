#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Moose.h"

using namespace cugl;

Moose::Moose(int h, int hsize) {
	health = h;
	handSize = hsize;
}

Moose::~Moose() {
	CULog("Moose Destroyed");
}


void Moose::changeChickenInStack(element e, special s, int d, int pos) {
	stack.at(pos).setChicken(e, s, d);
}

void Moose::changeChickenInStackElement(element e, int pos) {
	Chicken c = stack.at(pos);
	c.setChicken(e, c.getSpecial());
}

void Moose::changeChickenInStackSpecial(special s, int pos) {
	Chicken c = stack.at(pos);
	c.setChicken(c.getElement(), s);
}

void Moose::changeChickenInStackDamage(int d, int pos) {
	Chicken c = stack.at(pos);
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

void Moose::refillHand() {
	//Draw from deck while there are still chickens in the deck and hand is not full
	while (deck.getSize() > 0 && hand.size() < handSize) {
		hand.push_back(deck.draw());
	}

	if (hand.size() == handSize) return;

	//refill deck from discard pile if empty and hand is still not full
	deck.fill(discard);
	discard.clear();

	//Redraw
	while (deck.getSize() > 0 && hand.size() < handSize) {
		hand.push_back(deck.draw());
	}
	
}

void Moose::draw(int num) {
	for (int i = 0; i < num; i++) {
		Chicken &c = deck.draw();
		if (hand.size() < handSize - 1) {
			hand.push_back(c);
		} else {
			discard.push_back(c);
		}
	}
}

string Moose::printMoose() const {
	stringstream ss;
	ss << "Moose Info:\n";
	for (int i = 0; i < hand.size(); i++) {
		ss << "Hand " << i << ": " << hand.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	for (int i = 0; i < stack.size(); i++) {
		ss << "Stack " << i << ": " << stack.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	for (int i = 0; i < discard.size(); i++) {
		ss << "Discard " << i << ": " << discard.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	ss << deck.printCoop();

	return ss.str();
}