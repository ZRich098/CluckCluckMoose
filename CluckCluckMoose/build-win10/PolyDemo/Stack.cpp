#include "Stack.h"

using namespace cugl;

void Stack::specialChickenEffect(vector <Chicken> stack1, vector <Chicken> stack2) {
	special s1 = stack1.back().getSpecial();
	special s2 = stack2.back().getSpecial();

	// Reaper, Bomb, and None are all represented by element and damage and do not need special effects

	if (s1 == special::Null || s2 == special::Null) {
		Chicken target = stack2.back();
		s1 == special::Null ? target = stack2.back() : target = stack1.back();
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

	if (s1 == special::Cycle && stack1.size() >= 2)
		stack1.at(stack1.size() - 2).cycle();
	if (s2 == special::Cycle && stack2.size() >= 2)
		stack2.at(stack2.size() - 2).cycle();

	if (s1 == special::SelfSwap && stack1.size() >= 2)
		iter_swap(&stack1.at(stack1.size() - 2), &stack1.back());
	if (s2 == special::SelfSwap && stack2.size() >= 2)
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
