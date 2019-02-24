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
		int getSize();

		//Modify
		/** Draw a Chicken, removing it from the Coop*/
		Chicken draw();
		/** Clear the coop of all Chickens*/
		void clear();
		/** Shuffles the coop so all chickens are in random order*/
		void shuffle();
		/** Add Chicken c to the end of Coop (c will be next chicken drawn)*/
		void add(Chicken& c);
		/** Add the c Chickens to the Coop */
		void fill(vector <Chicken> c);
};

#endif /* __COOP_H__ */