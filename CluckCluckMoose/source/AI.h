//
//  AI.h
//

#ifndef __AI_H__
#define __AI_H__

#include "Moose.h"
#include <map>

enum class AIType {
	Dumb, //Only plays 1st card
	Basic, //Plays non-trivially
    Smart, // Randomly chooses between decent moves
	Expert //Hits up that minimax tree
};


/**
 * This class is the AI.
 *
 */
class AI {
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
//    CU_DISALLOW_COPY_AND_ASSIGN(AI);

	AIType type;

	//Self
	Stack stack;
	Stack stackProcessed;
	vector <Chicken> hand;

	//Opponent
	Stack oppStack;
	Stack oppStackProcessed;
	int oppHandSize;
	vector <int> distribution; //opponent elemental chicken distribution

	//Setup Play
	void setup();
	void addPermutationsToMap(map<int, Stack> &stackValueMap, vector <int> pos, Stack currentStack);

	int typeBonus(Chicken &c);
	int defeatOpponentBottom(Chicken &c);

	int basicPlay();
	int smartPlay();
	int expertPlay();

protected:
	std::shared_ptr<Moose> player; //renamed bc 'self' was registering as keyword
	std::shared_ptr<Moose> enemy;

public:
    
#pragma mark Constructors
    /**
     * Creates a new AI.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a model on
     * the heap, use one of the static constructors instead.
     */
    // AI(void) { }
    
    /**
     * Destroys this AI, releasing all resources.
     */
    virtual ~AI(void) { dispose(); }
    
    /**
     * Disposes all resources and assets of this AI
     */
    void dispose();
    
    /**
     * Initializes a new AI.
     *
     * @return  true if the AI is initialized properly, false otherwise.
     */
    bool init(std::shared_ptr<Moose> p, std::shared_ptr<Moose> e, AIType at);
    
    
#pragma mark Static Constructors
    
    /**
     * Returns a newly allocated AI.
     *
     * @return a newly allocated AI.
     */
    static std::shared_ptr<AI> alloc(std::shared_ptr<Moose> p, std::shared_ptr<Moose> e, AIType at) {
        std::shared_ptr<AI> result = std::make_shared<AI>();
        return (result->init(p, e, at) ? result : nullptr);
    }
    
#pragma mark -
#pragma mark Accessors
    /** Gets the position of the next card in hand to play
		i.e. 0 means play card at position 0 in hand */
	int getPlay();
};

#endif /* __AI_H__ */
