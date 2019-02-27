#include "AI.h"

using namespace cugl;

AI::AI(Moose s, Moose e, AIType at) {
	self = s;
	enemy = e;
	type = at;
}

AI::~AI() {
	//CULog("AI Destroyed");
}

int AI::getPlay() {
	switch (type) {
		case AIType::Dumb:
			return 0;
		case AIType::Smart:
			return 0;
	}
}