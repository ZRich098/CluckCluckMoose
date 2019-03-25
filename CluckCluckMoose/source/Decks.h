#ifndef __DECKS_H__
#define __DECKS_H__
#include "Chicken.h"

special deckV1[30] = {
	special::BasicFire,
	special::BasicFire,
	special::BasicFire,
	special::BasicWater,
	special::BasicWater,
	special::BasicWater,
	special::BasicGrass,
	special::BasicGrass,
	special::BasicGrass,

	special::Ninja,
	special::Ninja,
	special::Thicken,
	special::Thicken,
	//special::Scientist,
	//special::Scientist,
	special::Reaper,
	special::Reaper,
	special::Mirror,
	special::Mirror,
	special::PartyFowl,
	special::PartyFowl,
	special::Spy,
	special::Spy,
	//special::Angry,
	//special::Angry,
	//special::Lichen,
	//special::Lichen
};

special deckV2[28] = { //actually 30 but 1 deprecated chicken type
	special::BasicFire,
	special::BasicFire,
	special::BasicWater,
	special::BasicWater,
	special::BasicGrass,
	special::BasicGrass,

	special::Consigliere,
	special::Consigliere,
	//2 fusion chicken
	special::Ninja,
	special::Ninja,
	special::Bomb,
	special::Bomb,
	//special::Alchemist,
	//special::Alchemist,
	special::Reaper,
	special::Reaper,
	special::BirdBrain,
	//special::BirdBrain,
	//special::PartyFowl,
	special::PartyFowl,
	special::Mirror,
	special::Mirror,
	special::Spy,
	special::Spy,
	//special::Scientist,
	//special::Scientist,
	special::Thicken,
	special::Thicken
};

special deckV3[18] = {
	special::BasicFire,
	special::BasicFire,
	special::BasicFire,
	special::BasicWater,
	special::BasicWater,
	special::BasicWater,
	special::BasicGrass,
	special::BasicGrass,
	special::BasicGrass,

	special::Thicken,
	special::Ninja,
	special::PartyFowl,
	special::Mirror,
	special::Reaper,
	//special::BirdBrain,
	special::Bomb,
	special::Consigliere,
	special::Spy
};

special deckV4[8] = {
	/*special::BasicFire,
	special::BasicFire,
	special::BasicFire,
	special::BasicFire,
	special::BasicFire,
	special::BasicWater,
	special::BasicWater,
	special::BasicWater,
	special::BasicWater,
	special::BasicWater,
	special::BasicGrass,
	special::BasicGrass,
	special::BasicGrass,
	special::BasicGrass,
	special::BasicGrass,*/

	special::Consigliere,
	//special::WingMan,
	special::Ninja,
	special::Bomb,
	//special::PoultryPals,
	special::Reaper,
	//special::BirdBrain,
	//special::Clicken,
	special::PartyFowl,
	special::Mirror,
	special::Spy,
	//special::Smoked,
	special::Thicken,
	//special::PartridgePilferer,
	//special::Lichen
};

#endif /* __DECKS_H__ */