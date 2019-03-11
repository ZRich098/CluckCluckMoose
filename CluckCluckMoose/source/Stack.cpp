#include <sstream>

#include "Stack.h"

using namespace cugl;

Stack::Stack() {

}

Stack::~Stack() {
	//CULog("Stack Destroyed");
}

void Stack::add(const Chicken &c) {
	play_order.push_back(c.getSpecial());
	chickens.push_back(c);
}

Chicken Stack::getBottom() {
	return chickens.front();
}

Chicken Stack::getTop() {
	return chickens.back();
}

Chicken Stack::getAt(int index) {
	return chickens.at(index);
}

int Stack::getHeight() {
	return chickens.size();
}

Chicken Stack::removeBottom() {
	special s = chickens.front().getSpecial();
	chickens.erase(chickens.begin());
	return Chicken(s);
}

Chicken Stack::removeTop() {
	special s = chickens.back().getSpecial();
	chickens.erase(chickens.begin());
	return Chicken(s);
}

void Stack::swap(int pos1, int pos2) {
	iter_swap(&chickens.at(pos1), &chickens.at(pos2));
}

void Stack::insert(int pos, const Chicken &c) {
	chickens.insert(chickens.begin(), c)
}

void Stack::changeChickenInStack(element e, special s, int d, int pos) {
	chickens.at(pos).setChicken(e, s, d);
}

void Stack::changeChickenInStackElement(element e, int pos) {
	Chicken &c = chickens.at(pos);
	c.setChicken(e, c.getSpecial());
}

void Stack::changeChickenInStackSpecial(special s, int pos) {
	Chicken &c = chickens.at(pos);
	c.setChicken(c.getElement(), s);
}

void Stack::changeChickenInStackDamage(int d, int pos) {
	Chicken &c = chickens.at(pos);
	c.setChicken(c.getElement(), c.getSpecial(), d);
}


void Stack::specialChickenEffect(Stack opp) {
	special s1 = this->getTop().getSpecial();
	special s2 = opp.getTop().getSpecial();

	// Reaper, Bomb, and None are all represented by element and damage and do not need special effects

	if (s1 == special::PartyFowl || s2 == special::PartyFowl) {
		Chicken &target = this->getTop();
		s1 == special::PartyFowl ? target = opp.getTop() : target = this->getTop();
		switch (target.getSpecial()) {
		case special::Reaper:
			target.setElement(element::Water);
			break;
		case special::Mirror:
			target.setElement(element::Fire);
			break;
		case special::Bomb:
			target.setElement(element::Fire);
			break;
		}
		target.setDamage(1);
		return;
	}

	if (s1 == special::Mirror && s2 == special::Mirror) {
		this->getTop().setElement(element::Fire);
		opp.getTop().setElement(element::Fire);
	}
	else if (s1 == special::Mirror) {
		s1 = s2;
		this->getTop().setDamage(opp.getTop().getDamage());
	}
	else if (s2 == special::Mirror) {
		s2 = s1;
		opp.getTop().setDamage(this->getTop().getDamage());
	}

	//potentially TODO special::Peek

	if (s1 == special::Consigliere && this->getHeight() >= 2)
		this->getAt(this->getHeight() - 2).cycle();
	if (s2 == special::Consigliere && opp.getHeight() >= 2)
		opp.getAt(opp.getHeight() - 2).cycle();

	if (s1 == special::Scientist && this->getHeight() >= 2)
		this->swap(this->getHeight() - 2, this->getHeight() - 1);
	if (s2 == special::Scientist && opp.getHeight() >= 2)
		opp.swap(opp.getHeight() - 2, opp.getHeight() - 1);

	if (s1 == special::Thicken) {
		this->insert(0, this->getTop());
		this->removeTop();
	}
	if (s2 == special::Thicken) {
		opp.insert(0, opp.getTop());
		opp.removeTop();
	}

	//TODO special::Hide

	//potentially TODO special::Extra

	if (s1 == special::Ninja && s2 == special::Ninja) {
		this->swap(0, this->getHeight() - 1);
		opp.swap(0, opp.getHeight() - 1);
	}
	else if (s1 == special::Ninja)
		opp.swap(0, opp.getHeight() - 1);
	else if (s2 == special::Ninja)
		this->swap(0, this->getHeight() - 1);
}

string Stack::stackString() const {
	stringstream ss;
	for (int i = 0; i < chickens.size(); i++) {
		ss << "Stack " << i + 1 << ": " << chickens.at(chickens.size() - i - 1).toString().c_str() << "\n";
	}

	return ss.str();
}