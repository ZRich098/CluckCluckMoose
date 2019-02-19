#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Moose.h"

using namespace cugl;

Moose::Moose() {
	health = 5;
	handSize = 5;
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

vector <Chicken> Moose::getStack() {
	return stack;
}

vector <Chicken> Moose::getHand() {
	return hand;
}

Coop Moose::getDeck() {
	return deck;
}

vector <Chicken> Moose::getDiscard() {
	return discard;
}

void Moose::clearHand() {
	hand.clear();
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

int Moose::getHealth() {
	return health;
}

void Moose::setHealth(int h) {
	health = h;
}

void Moose::takeDamage(int damage) {
	health = health - damage;
}