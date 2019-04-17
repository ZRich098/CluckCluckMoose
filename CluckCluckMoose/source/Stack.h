/** The header file for Stack*/
#ifndef __STACK_H__
#define __STACK_H__
#include <cugl/cugl.h>

#include "Chicken.h"


/** Nullifies a chicken */
void partyHelper(Chicken& target);

class Stack {
private:
	/** Return the list of chicken special abilities in order the chickens were added to the stack. */
	vector <special> play_order;
	/** List of current Chickens on stack in order of bottom to top
		chickens can have elements modified*/
	vector <Chicken> chickens;

public:
	//Constructor
	/** Create a new Stack*/
	Stack();
	/** Copy Constructor */
	Stack(const Stack& s) {
		play_order = s.play_order;
		chickens = s.chickens;
	}
	/***Destroys the Stack*/
	~Stack();

	/** Creates a substack of chickens, starting from the bottom, with a size of size.*/
	Stack substack(int size);

	//Access
	/** Get the size of the Coop*/
	const int getSize() const { return chickens.size(); };
	/** Returns the chicken at position pos, where 0 is next to be drawn
		If deck is shuffled, chicken will change position */
	Chicken &at(int pos) { return chickens.at(pos); };
	/** Gets the order the chickens were played on this stack
		Does not get cleared until clear() is called*/
	vector <special> getPlayOrder() { return play_order; };
	/** Returns the chicken at the bottom of the stack */
	Chicken &getBottom();
	/** Returns the chicken at the top of the stack */
	Chicken &getTop();

	/** Returns the total damage from all the chickens in this stack*/
	int const getDamage();


	//Modify
	/** Add a chicken to the top of the stack*/
	void add(const Chicken &c);
	/** Remove the chicken at the bottom of the stack
		Returns the chicken removed (with the original element)*/
	Chicken removeBottom();
	/** Removes the chicken at the top of the stack
		Returns the chicken removed (with the original element)*/
	Chicken removeTop();
	/**Swaps two Chickens at the positions given in the stack */
	void swap(int pos1, int pos2);
	/** Inserts a chicken anywhere in the stack */
	void insert(int pos, const Chicken &c);


	/** Clear the stack of all Chickens*/
	void clear() { play_order.clear();  chickens.clear(); };

	// Change Chickens
	/**Sets the Chicken at position pos in stack to element e, special s, and damage d */
	void changeChickenInStack(element e, special s, int d = 1, int pos = 0);
	/**Sets the Chicken at position pos in stack to element e */
	void changeChickenInStackElement(element e, int pos = 0);
	/**Sets the Chicken at position pos in stack to special s */
	void changeChickenInStackSpecial(special s, int pos = 0);
	/**Sets the Chicken at position pos in stack to damage d */
	void changeChickenInStackDamage(int d, int pos = 0);

	//Special
	/** Does special chicken effects of the chickens on the top of both stacks.
		Arguments: the opponent stack(both stacks must not be empty) and a state in the interval [0,2]
			where 0 = no skip, 1 = skip player, and 2 = skip opponent.
		Returns: a tuple of the skipState and the length in frames of a special chicken's effect. */
	tuple<int,int> specialChickenEffect(Stack &opp, int skipState = 0);
	/** Does special chicken effect on single stack (ignore opponent) for AI*/
	void specialChickenEffect();

	//Info
	/** Returns whether the stack is empty*/
	bool empty() { return chickens.size() == 0; };
	/** Prints the contents of the stack in order of bottom to top */
	string stackString() const;

	//Gameplay
	/** Compares this Stack with another Stack and runs a clash between them once. */
	void compare(Stack &opp);

	/** int of a stack, used for AI only*/
	int value;

};

#endif /* __STACK_H__ */