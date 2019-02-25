/** The header file for Coops*/

#ifndef __COOP_H__
#define __COOP_H__
#include <cugl/cugl.h>

#include "Chicken.h"

/** The Deck of Chickens */
class Coop {
	private:
		int deckNumber;
		vector <Chicken> chickens;
		bool shuffled;
		/** Loads all chickens in Deck deckNumber into the Coop
			DO NOT USE IF COOP NOT EMPTY*/
		void loadDeck();
	public:
		//Constructor
		/** Create a new Coop*/
		Coop(int deckNumber = 4);
		/***Destroys the Coop*/
		~Coop();

		//Access
		/** Get the size of the Coop*/
		const int getSize() const { return chickens.size(); };
		/** Returns whether the Coop has been shuffled yet 
			(Chickens are in random order after shuffling)*/
		const bool isShuffled() const { return shuffled; };
		/** Returns the Chicken at position pos, where 0 is next to be drawn
			If deck is shuffled, chicken will change position */
		Chicken at(int pos);

		//Modify
		/** Draw a Chicken, removing it from the Coop
			REQUIRES Coop not empty*/
		Chicken draw();
		/** Clear the coop of all Chickens*/
		void clear() { chickens.clear(); };
		/** Shuffles the coop so all chickens are in random order*/
		void shuffle();
		/** Add Chicken c to the end of Coop (c will be next chicken drawn)*/
		void add(const Chicken& c);
		/** Add the c Chickens to the Coop */
		void fill(const vector <Chicken> c);

		//Info
		/** Prints the contents of the coop in draw order (next to draw is first card printed, etc.)
			If deck is unshuffled then will print "Unshuffled" first; what is printed is not in draw order
			CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
		string coopString() const;
};

#endif /* __COOP_H__ */