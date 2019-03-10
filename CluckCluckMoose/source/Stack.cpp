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



void Stack::specialChickenEffect(vector <Chicken> stack1, vector <Chicken> stack2) {
	special s1 = stack1.back().getSpecial();
	special s2 = stack2.back().getSpecial();

	// Reaper, Bomb, and None are all represented by element and damage and do not need special effects

	if (s1 == special::PartyFowl || s2 == special::PartyFowl) {
		Chicken target = stack2.back();
		s1 == special::PartyFowl ? target = stack2.back() : target = stack1.back();
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
		stack1.back().setElement(element::Fire);
		stack2.back().setElement(element::Fire);
	}
	else if (s1 == special::Mirror) {
		s1 = s2;
		stack1.back().setDamage(stack2.back().getDamage());
	}
	else if (s2 == special::Mirror) {
		s2 = s1;
		stack2.back().setDamage(stack1.back().getDamage());
	}

	//potentially TODO special::Peek

	if (s1 == special::Consigliere && stack1.size() >= 2)
		stack1.at(stack1.size() - 2).cycle();
	if (s2 == special::Consigliere && stack2.size() >= 2)
		stack2.at(stack2.size() - 2).cycle();

	if (s1 == special::Scientist && stack1.size() >= 2)
		iter_swap(&stack1.at(stack1.size() - 2), &stack1.back());
	if (s2 == special::Scientist && stack2.size() >= 2)
		iter_swap(&stack2.at(stack2.size() - 2), &stack2.back());

	if (s1 == special::Thicken) {
		stack1.insert(stack1.begin(), stack1.back());
		stack1.pop_back();
	}
	if (s2 == special::Thicken) {
		stack2.insert(stack2.begin(), stack2.back());
		stack2.pop_back();
	}

	//TODO special::Hide

	//potentially TODO special::Extra

	if (s1 == special::Ninja && s2 == special::Ninja) {
		iter_swap(&stack1.front(), &stack1.back());
		iter_swap(&stack2.front(), &stack2.back());
	}
	else if (s1 == special::Ninja)
		iter_swap(&stack2.front(), &stack2.back());
	else if (s2 == special::Ninja)
		iter_swap(&stack1.front(), &stack1.back());
}

string Stack::stackString() const {
	stringstream ss;
	for (int i = 0; i < chickens.size(); i++) {
		ss << "Stack " << i + 1 << ": " << chickens.at(chickens.size() - i - 1).toString().c_str() << "\n";
	}

	return ss.str();
}