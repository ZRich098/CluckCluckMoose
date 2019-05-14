#ifndef __TUTORIAL_MOOSE_H__
#define __TUTORIAL_MOOSE_H__


#include <cugl/cugl.h>

#include "Moose.h"

/**
 * This class is for the Moose.
 */
class TutorialMoose : public Moose {
private:

protected:
	bool isPlayer;
	int handSize;
	int health;

public:


#pragma mark Constructors
	/**
	 * Creates a new moose.
	 *
	 * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a model on
	 * the heap, use one of the static constructors instead.
	 */
	TutorialMoose(void) : handSize(3), health(3) { }

	/**
	 * Destroys this moose, releasing some! resources.
	 */
	virtual ~TutorialMoose(void) { dispose(); }

	/**
	 * Disposes the moose
	 */
	void dispose();

	/**
	 * Initializes a new moose: health given, handsize given.
	 *
	 * @return  true if the obstacle is initialized properly, false otherwise.
	 */
	virtual bool init(int h, int hSize, bool isPlayer);


#pragma mark Static Constructors    
	/**
	 * Returns a newly allocated moose with default handsize given, health given.
	 *
	 * @return a newly allocated moose
	 */
	static std::shared_ptr<TutorialMoose> alloc(int h, int hSize, bool p) {
		std::shared_ptr<TutorialMoose> result = std::make_shared<TutorialMoose>();
		return (result->init(h, hSize, p) ? result : nullptr);
	}

#pragma mark -
#pragma mark Stack
	/** Refills the hand with Chickens from the Coop */
	void refillHand();
	void refillPlayerHand();
	void refillEnemyHand();
	/** Draws num card from the Coop and adds it to the hand
		Discards any cards drawn over the hand size (straight to discard pile)*/
	void draw(int num = 1);
};

#endif /* __TUTORIAL_MOOSE_H__ */