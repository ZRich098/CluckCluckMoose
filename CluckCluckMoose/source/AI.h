/** The header file for AI*/
#ifndef __AI_H__
#define __AI_H__

#include "Moose.h"

enum class AIType {
	Dumb,
	Smart
};

/** The AI */
class AI {
private:
	Moose self;
	Moose enemy;
	AIType type;
public:
	//Constructor
	/** Create a new AI*/
	AI(Moose self, Moose enemy, AIType at = AIType::Dumb);
	/** Destroys the AI*/
	~AI();

	/** Gets the position of the next card in hand to play
		i.e. 0 means play card at position 0 in hand */
	int getPlay();
};

#endif /* __AI_H__ */