//
//  AI.cpp
//
#include "AI.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new AI
 *
 * @return  true if the AI is initialized properly, false otherwise.
 */
bool AI::init(std::shared_ptr<Moose> p, std::shared_ptr<Moose> e, AIType at) {
    player = p;
    enemy = e;
    typ = at;
    return true;
}


/**
 * Disposes all resources and assets of this AI
 */
void AI::dispose() {
    player = nullptr;
    enemy = nullptr;
    typ = AIType::Dumb; // default to Dumb because it won't take a nullptr
}

int AI::getPlay() {
	switch (typ) {
		case AIType::Dumb:
			return 0;
		case AIType::Smart:
			return 0;
	}
}
