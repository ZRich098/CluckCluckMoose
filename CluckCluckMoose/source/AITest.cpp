#include "AITest.h"

using namespace cugl;
using namespace std;

void testAll() {
	std::shared_ptr<Moose> p1 = Moose::alloc(5, 6);
	std::shared_ptr<Moose> p2 = Moose::alloc(5, 6);

	std::shared_ptr<AI> dumb1 = AI::alloc(p1, p2, AIType::Dumb);
	std::shared_ptr<AI> intro1 = AI::alloc(p1, p2, AIType::Intro);
	std::shared_ptr<AI> basic1 = AI::alloc(p1, p2, AIType::Basic);
	std::shared_ptr<AI> smart1 = AI::alloc(p1, p2, AIType::Smart);

	std::shared_ptr<AI> intro2 = AI::alloc(p2, p1, AIType::Intro);
	std::shared_ptr<AI> basic2 = AI::alloc(p2, p1, AIType::Basic);
	std::shared_ptr<AI> smart2 = AI::alloc(p2, p1, AIType::Smart);
	std::shared_ptr<AI> expert2 = AI::alloc(p2, p1, AIType::Expert);

	int p1win = playGames(p1, basic1, p2, smart2, 100);
	CULog("Basic vs Smart: %i to %i", p1win, 100 - p1win);

	/*
	int NUM_GAMES = 1000;
	int p1win;

	p1win = playGames(p1, dumb1, p2, intro2, NUM_GAMES);
	CULog("Dumb vs Intro: %i to %i", p1win, NUM_GAMES - p1win);
	p1win = playGames(p1, dumb1, p2, basic2, NUM_GAMES);
	CULog("Dumb vs Basic: %i to %i", p1win, NUM_GAMES - p1win);
	p1win = playGames(p1, dumb1, p2, smart2, NUM_GAMES);
	CULog("Dumb vs Smart: %i to %i", p1win, NUM_GAMES - p1win);
	p1win = playGames(p1, dumb1, p2, expert2, NUM_GAMES);
	CULog("Dumb vs Expert: %i to %i", p1win, NUM_GAMES - p1win);

	p1win = playGames(p1, intro1, p2, basic2, NUM_GAMES);
	CULog("Intro vs Basic: %i to %i", p1win, NUM_GAMES - p1win);
	p1win = playGames(p1, intro1, p2, smart2, NUM_GAMES);
	CULog("Intro vs Smart: %i to %i", p1win, NUM_GAMES - p1win);
	p1win = playGames(p1, intro1, p2, expert2, NUM_GAMES);
	CULog("Intro vs Expert: %i to %i", p1win, NUM_GAMES - p1win);

	p1win = playGames(p1, basic1, p2, smart2, NUM_GAMES);
	CULog("Basic vs Smart: %i to %i", p1win, NUM_GAMES - p1win);
	p1win = playGames(p1, basic1, p2, expert2, NUM_GAMES);
	CULog("Basic vs Expert: %i to %i", p1win, NUM_GAMES - p1win);

	p1win = playGames(p1, smart1, p2, expert2, NUM_GAMES);
	CULog("Smart vs Expert: %i to %i", p1win, NUM_GAMES - p1win);
	*/
}

int playGames(std::shared_ptr<Moose> p1, std::shared_ptr<AI> ai1, std::shared_ptr<Moose> p2, std::shared_ptr<AI> ai2, int num_games) {
	int p1win = 0;
	int p2win = 0;

	//play num_games games
	for (int games = 0; games < num_games; games++) {

		//start a single game
		p1->refillHand();
		p2->refillHand();

		while (p1->getHealth() > 0 && p2->getHealth() > 0) {
			srand(time(NULL));
			//while stack size < 5, add more
			while (p1->getStack().getSize() < 5) {
				p1->addToStackFromHand(ai1->getPlay());
				p2->addToStackFromHand(ai2->getPlay());

				Stack stack1 = p1->getStack();
				Stack stack2 = p2->getStack();

				special s1 = stack1.getTop().getSpecial();
				special s2 = stack2.getTop().getSpecial();

				if (s1 == special::PartyFowl || s2 == special::PartyFowl) 
					s1 == special::PartyFowl ? partyHelper(p2->getStack().getTop()) : partyHelper(p1->getStack().getTop());

				if (s1 == special::Mirror && s2 == special::Mirror) {
					p1->getStack().getTop().setChicken(element::Grass, special::BasicGrass);
					p2->getStack().getTop().setChicken(element::Grass, special::BasicGrass);
				}
				else if (s1 == special::Mirror) {
					s1 = s2;
					p1->getStack().getTop().setChicken(stack2.getTop().getElement(), stack2.getTop().getSpecial(), stack2.getTop().getDamage());
				}
				else if (s2 == special::Mirror) {
					s2 = s1;
					p2->getStack().getTop().setChicken(stack1.getTop().getElement(), stack1.getTop().getSpecial(), stack1.getTop().getDamage());
				}

				if (s1 == special::Witchen && stack1.getSize() >= 2)
					stack1.at(stack1.getSize() - 2).cycle();
				if (s2 == special::Witchen && stack2.getSize() >= 2) 
					stack2.at(stack2.getSize() - 2).cycle();

				if (s1 == special::Scientist && stack1.getSize() >= 2) 
					stack1.swap(stack1.getSize() - 2, stack1.getSize() - 1);
				if (s2 == special::Scientist && stack2.getSize() >= 2) 
					stack2.swap(stack2.getSize() - 2, stack2.getSize() - 1);

				if (s1 == special::Thicken) {
					stack1.insert(0, stack1.getTop());
					stack1.removeTop();
				}
				if (s2 == special::Thicken) {
					stack2.insert(0, stack2.getTop());
					stack2.removeTop();
				}

				if (s1 == special::Spy)
					p1->draw();
				if (s2 == special::Spy)
					p2->draw();
			}


			//clash
			while (!p1->getStack().empty() && !p2->getStack().empty()) {
				p1->getStack().compare(p2->getStack());
			}

			p1->takeDamage(p2->getStack().getSize());
			p2->takeDamage(p1->getStack().getSize());

			p1->clearHandToDiscard();
			p2->clearHandToDiscard();

			p1->refillHand();
			p2->refillHand();
			p1->discardStack();
			p2->discardStack();
			p1->getStack().clear();
			p2->getStack().clear();
		}

		if (p1->getHealth() <= 0) {
			p2win++;
		}
		if (p2->getHealth() <= 0) {
			p1win++;
		}

		p1->setHealth(5);
		p2->setHealth(5);

		CULog("%i game done", games);
	}

	if (p1win + p2win != num_games) CULogError("That aint right");

	return p1win;
}
