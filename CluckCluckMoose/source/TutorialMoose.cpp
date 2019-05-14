#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "TutorialMoose.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new moose with the given health and hand size.
 *
 * @return  true if the Moose is initialized properly, false otherwise.
 */
bool TutorialMoose::init(int h, int hSize, bool player) {
	isPlayer = player;
	health = h;
	handSize = hSize;
	numChickensWillDiePreview = 0;
	for (int i = 0; i < 5; i++) vecChickensClashPreview.push_back(-1);
	return true;
}


/**
 * Disposes all resources and assets of this moose
 *
 * Does not currently actually dispose all assets!
 */
void TutorialMoose::dispose() {
	health = 0;
	handSize = 0;
	// deck = Coop(4);
}

#pragma mark -
#pragma mark Stack
void TutorialMoose::refillHand() {
	if (isPlayer) {
		//CULog("tutorial refilling player hand");
		refillPlayerHand();
	} else {
		refillEnemyHand();
	}
}

void TutorialMoose::refillEnemyHand() {
	hand.push_back(Chicken(element::Fire, special::BasicFire));
	hand.push_back(Chicken(element::Water, special::BasicWater));
	hand.push_back(Chicken(element::Grass, special::BasicGrass));
	hand.push_back(Chicken(element::Grass, special::BasicGrass));
	hand.push_back(Chicken(special::Spy));
	hand.push_back(Chicken(special::Spy));
}

void TutorialMoose::refillPlayerHand() {
	hand.push_back(Chicken(element::Fire, special::BasicFire));
	hand.push_back(Chicken(element::Grass, special::BasicGrass));
	hand.push_back(Chicken(element::Water, special::BasicWater));
	hand.push_back(Chicken(element::Fire, special::BasicFire));
	hand.push_back(Chicken(special::Spy));
	hand.push_back(Chicken(special::Spy));
}

void TutorialMoose::draw(int num) {
	//CULog("tutorial drawing %i", num);
	for (int i = 0; i < num; i++) {
		switch( rand() % 3){
		case 0:
			hand.push_back(Chicken(element::Fire, special::BasicFire));
		case 1:
			hand.push_back(Chicken(element::Grass, special::BasicGrass));
		default:
			hand.push_back(Chicken(element::Water, special::BasicWater));
		}
	}
}