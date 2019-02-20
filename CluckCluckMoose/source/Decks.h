#ifndef __DECKS_H__
#define __DECKS_H__
#include "Chicken.h"

Chicken deckV1[30] = {
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),

	Chicken(element::Water, special::Ninja),
	Chicken(element::Water, special::Ninja),
	Chicken(element::Fire, special::Thicken),
	Chicken(element::Fire, special::Thicken),
	Chicken(element::Grass, special::SelfSwap),
	Chicken(element::Grass, special::SelfSwap),
	Chicken(element::Water, special::Reaper, 0),
	Chicken(element::Water, special::Reaper, 0),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Fire, special::Clash),
	Chicken(element::Fire, special::Clash),
	Chicken(element::Grass, special::Discard),
	Chicken(element::Grass, special::Discard)
};

Chicken deckV2[28] = { //actually 30 but 1 deprecated chicken type
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),

	Chicken(element::Water, special::Cycle),
	Chicken(element::Water, special::Cycle),
	//2 fusion chicken
	Chicken(element::Water, special::Ninja),
	Chicken(element::Water, special::Ninja),
	Chicken(element::Fire, special::Bomb, 2),
	Chicken(element::Fire, special::Bomb, 2),
	Chicken(element::Fire, special::CycleAll),
	Chicken(element::Fire, special::CycleAll),
	Chicken(element::Water, special::Reaper, 0),
	Chicken(element::Water, special::Reaper, 0),
	Chicken(element::Grass, special::Draw),
	Chicken(element::Grass, special::Draw),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Grass, special::SelfSwap),
	Chicken(element::Grass, special::SelfSwap),
	Chicken(element::Fire, special::Thicken),
	Chicken(element::Fire, special::Thicken)
};

Chicken deckV3[18] = {
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),

	Chicken(element::Fire, special::Thicken),
	Chicken(element::Grass, special::Ninja),
	Chicken(element::Water, special::Null),
	Chicken(element::Fire, special::Mirror),
	Chicken(element::Water, special::Reaper, 0),
	Chicken(element::Grass, special::Draw),
	Chicken(element::Fire, special::Bomb, 2),
	Chicken(element::Water, special::Cycle),
	Chicken(element::Grass, special::Peek)
};

Chicken deckV4[30] = {
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Fire, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Water, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),
	Chicken(element::Grass, special::Null),

	Chicken(element::Water, special::Cycle),
	Chicken(element::Water, special::Extra),
	Chicken(element::Grass, special::Ninja),
	Chicken(element::LoseAll, special::Bomb, 2),
	Chicken(element::Fire, special::Search),
	Chicken(element::TieAll, special::Reaper, 0),
	Chicken(element::Grass, special::Draw),
	Chicken(element::Water, special::Refresh),
	Chicken(element::Water, special::Null),
	Chicken(element::Unset, special::Mirror),
	Chicken(element::Grass, special::Peek),
	Chicken(element::Fire, special::Hide),
	Chicken(element::Fire, special::Thicken),
	Chicken(element::LoseAll, special::Draw2),
	Chicken(element::Grass, special::Discard)
};

#endif /* __DECKS_H__ */