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

Chicken &Stack::getBottom() {
	return chickens.front();
}

Chicken &Stack::getTop() {
	//CULog("Pointer of chicken: %p", &chickens.back());
	return chickens.back();
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
	chickens.insert(chickens.begin(), c);
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
	special s1 = getTop().getSpecial();
	special s2 = opp.getTop().getSpecial();


	CULog(stackString().c_str());
	// Reaper, Bomb, and Basics are all represented by element and damage and do not need special effects

	if (s1 == special::PartyFowl || s2 == special::PartyFowl) {
		Chicken &target = getTop();
		s1 == special::PartyFowl ? target = opp.getTop() : target = getTop();
		switch (target.getSpecial()) {
		case special::Reaper:
			target.setChicken(element::Water,special::BasicWater);
			break;
		case special::Mirror:
			target.setChicken(element::Fire,special::BasicFire);
			break;
		case special::Bomb:
			target.setChicken(element::Fire,special::BasicFire);
			break;
		}
		return;
	}

	if (s1 == special::Mirror && s2 == special::Mirror) {
		getTop().setChicken(element::Fire,special::BasicFire);
		opp.getTop().setChicken(element::Fire,special::BasicFire);
	}
	else if (s1 == special::Mirror) {
		s1 = s2;
		getTop().setChicken(opp.getTop().getElement(),opp.getTop().getSpecial(),opp.getTop().getDamage());
	}
	else if (s2 == special::Mirror) {
		s2 = s1;
		opp.getTop().setChicken(getTop().getElement(), getTop().getSpecial(), getTop().getDamage());
	}

	//potentially TODO special::Peek

	if (s1 == special::Consigliere && getSize() >= 2)
		at(getSize() - 2).cycle();
	if (s2 == special::Consigliere && opp.getSize() >= 2)
		opp.at(opp.getSize() - 2).cycle();

	if (s1 == special::Scientist && getSize() >= 2)
		swap(getSize() - 2, getSize() - 1);
	if (s2 == special::Scientist && opp.getSize() >= 2)
		opp.swap(opp.getSize() - 2, opp.getSize() - 1);

	if (s1 == special::Thicken) {
		insert(0, getTop());
		removeTop();
	}
	if (s2 == special::Thicken) {
		opp.insert(0, opp.getTop());
		opp.removeTop();
	}

	//TODO special::Hide

	//potentially TODO special::Extra

	if (s1 == special::Ninja && s2 == special::Ninja) {
		swap(0, getSize() - 1);
		opp.swap(0, opp.getSize() - 1);
	}
	else if (s1 == special::Ninja)
		opp.swap(0, opp.getSize() - 1);
	else if (s2 == special::Ninja)
		swap(0, getSize() - 1);


	CULog(stackString().c_str());
}

string Stack::stackString() const {
	stringstream ss;
	for (int i = 0; i < chickens.size(); i++) {
		ss << "Stack " << i + 1 << ": " << chickens.at(i).toString().c_str() << "\n";
	}

	return ss.str();
}