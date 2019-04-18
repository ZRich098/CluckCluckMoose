//
//  Moose.h
//

#ifndef __MOOSE_H__
#define __MOOSE_H__
#include <cugl/cugl.h>

#include "Coop.h"
#include "Stack.h"

/**
 * This class is for the Moose.
 */
class Moose { 
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
//    CU_DISALLOW_COPY_AND_ASSIGN(Moose);

protected:
    //Pecking Order >:D This is actually a queue tho lol
	Stack stack;	
	vector <Chicken> hand;
	Coop deck;
	vector <Chicken> discard;
	vector <Chicken> playOrder;
	vector <vector<Chicken>> handPool;
	//nonEleDist is a seed for the non-elemental chickens in the element distribution to stay as one element
	vector <int> nonEleDist;
	int handSize;
	int health;
	void refillDeck();
	int numChickensWillDiePreview;
    
public:
   
    
#pragma mark Constructors
    /**
     * Creates a new moose.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a model on
     * the heap, use one of the static constructors instead.
     */
    Moose(void) : handSize(3), health(3) { }
    
    /**
     * Destroys this moose, releasing some! resources.
     */
    virtual ~Moose(void) { dispose(); }
    
    /**
     * Disposes the moose
     */
    void dispose();
    
    /**
     * Initializes a new moose: health given, handsize given.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(int h, int hSize);

	/**
	 * Initializes the moose with json style arguments
	 */
	void jsonInit(int health, vector<int> hand, vector<int> playOrder, vector<int> coop, string costume);
	void jsonInit(int health, vector<int> hand, vector<int> playOrder, vector<vector<int>> handPoolArray, string costume);
    
    
#pragma mark Static Constructors    
    /**
     * Returns a newly allocated moose with default handsize given, health given.
     *
     * @return a newly allocated moose
     */
    static std::shared_ptr<Moose> alloc(int h, int hSize) {
        std::shared_ptr<Moose> result = std::make_shared<Moose>();
        return (result->init(h,hSize) ? result : nullptr);
    }
    
#pragma mark -
#pragma mark Accessors
    
    /** Get the current pecking order of the Moose*/
	Stack& getStack() { return stack; };
	/** Get the Chicken at position pos in the stack of the Moose*/
	Chicken& getStackAt(int pos) { return stack.at(pos); };
	/** Get the order that chickens have been played in*/
	vector <Chicken> getOrder() { return playOrder; };
	/** Get the Chicken at position pos in the play order*/
	Chicken getOrderAt(int pos) { return playOrder.at(pos); };
	/** Get the current hand of the Moose*/
	vector <Chicken> getHand() { return hand; };
	/** Get the Chicken at position pos in the hand of the Moose*/
	Chicken getHandAt(int pos) { return hand.at(pos); };
	/** Get the current play order of the Moose */
	vector <Chicken> getPlayOrder() { return playOrder; };
	/** Get the Chicken at position pos in the play order of the Moose */
	Chicken getPlayOrderAt(int pos) { return playOrder.at(pos); };
	/** Get the current draw pile of the Moose*/
	Coop getDeck() { return deck; };
	/** Get the Chicken at position pos in the Coop of the Moose*/
	Chicken getDeckAt(int pos) { return deck.at(pos); };
	/** Get the current hand pool of the Moose*/
	vector<vector<Chicken>> getHandPool() { return handPool; };
	/** Get the hand at position pos in the handPool of the Moose*/
	vector<Chicken> getHandPoolAt(int pos) { return handPool.at(pos); };
	/** Get the current discard pile of the Moose*/
	vector <Chicken> getDiscard() { return discard; };
	/** Get the Chicken at position pos in the discard pile of the Moose*/
	Chicken getDiscardAt(int pos) { return discard.at(pos); };
	/** Get the current health of the Moose*/
	int getHealth() { return health; };

	/** Gets the number of chickens in the moose's stack that will die if clashes were to occur with the current stacks*/
	int getNumChickensWillDiePreview() { return numChickensWillDiePreview; };
	/** Sets the number of chickens in the moose's stack that will die if clashes were to occur with the current stacks*/
	void setNumChickensWillDiePreview(int num) { numChickensWillDiePreview = num; };

	/** Get number of {fire, water, grass, other}, chickens in hand, returned as a vector of ints*/
	vector<int> getChickenElementDistribution();

	/** Sets the opponent's possible hands.
		Input is a vector of chicken vectors
		(where the internal vectors are the hands and the external one is the pool of possible hands)*/
	void setOppHands(vector<vector<Chicken>> pool) { handPool = pool; }

    
#pragma mark -
#pragma mark Stack
    
    //Stack
	/** Add the Chicken at position pos in the hand to the stack.  DOES remove the chicken from the hand. */
	void addToStackFromHand(int pos);
	/** Puts the Chicken at the top of the stack back into the hand*/
	void removeTopFromStackToHand();
	/** Discards the chickens that were played in a round */
	void discardStack();
	/** Puts all Chickens in the stack into the discard pile */
	//void clearStackToDiscard();
	/** Sets the stack to be the equal to Stack s */
	void setStack(Stack s);

	//Hand
	/** Clears the hand of all Chickens */
	void clearHandToDiscard();
	/** Refills the hand with Chickens from the Coop */
	void refillHand();
	/** Shuffles the hand into a random order*/
	void shuffleHand() { random_shuffle(hand.begin(), hand.end()); };
	/** Draws num card from the Coop and adds it to the hand 
		Discards any cards drawn over the hand size (straight to discard pile)*/
	void draw(int num = 1);

	//Health
	/** Set the current health of the Moose*/
	void setHealth(int h) { health = h;};
	/** The moose loses $damage$ points of health*/
	void takeDamage(int damage) { health -= damage; };

	//Info
	/** returns a string of the relevant details about the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string mooseString() const;
	/** returns a string of the hand of the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string handString() const;
	/** return a string of the play order of the moose
		CAREFUL vector are 0 indexed but info printed is 1 indexed*/
	string playOrderString() const;
	/** returns a string of the discard pile of the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string discardString() const;
	/** returns a string of the stack of the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string stackString() const { return stack.stackString(); };
	/** returns a string of the deck of the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string deckString() const { return deck.coopString(); };

};

#endif /* __MOOSE_H__ */
