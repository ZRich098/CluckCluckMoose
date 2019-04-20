/** The header file for Coops*/

#ifndef __COOP_H__
#define __COOP_H__
#include <cugl/cugl.h>

#include "Chicken.h"

/** The Deck of Chickens */
class Coop {
	private:
		int deckNumber;
		vector <Chicken> specials;
		vector <Chicken> basics;
		bool specialShuffled;
		bool basicShuffled;
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
		/** Get the sizes of the Coop*/
		const int getSize() const { return basics.size() + specials.size(); }
		const int getSizeS() const { return specials.size(); };
		const int getSizeB() const { return basics.size(); };
		/** Returns whether the Coop has been shuffled yet 
			(Chickens are in random order after shuffling)*/
		const bool isShuffled() const { return specialShuffled && basicShuffled; };
		/** Returns the Chicken at position pos,
			0 is the first basic
			getSizeB() is the first special*/
		Chicken at(int pos);
		/** Returns copy of random chicken from basic or special */
		Chicken getBasic() { return basics.at(rand() % basics.size()); };
		Chicken getSpecial() { return specials.at(rand() % specials.size()); };

		//Modify
		/** Draw a Chicken, removing it from the Coop
			REQUIRES Coop not empty*/
		Chicken drawSpecial();
		Chicken drawBasic();
		/** Clear the coop of all Chickens*/
		void clear() { specials.clear(); basics.clear(); };
		/** Shuffles the coop so all chickens are in random order*/
		void shuffle();
		/** Add Chicken c to the end of Coop (c will be next chicken drawn)
			The boolean tells the deck to shuffle if true */
		void add(const Chicken& c, bool shouldShuffle = false);
		/** Add the c Chickens to the Coop */
		void fill(const vector <Chicken> c);
		/** Add the c Chickens to the Coop */
		void fill(const vector <int> cArray);

		//Info
		/** Prints the contents of the coop in draw order (next to draw is first card printed, etc.)
			If deck is unshuffled then will print "Unshuffled" first; what is printed is not in draw order
			CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
		string coopString() const;
};

#endif /* __COOP_H__ */