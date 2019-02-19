/** The header file for Coops*/

#ifndef __COOP_H__
#define __COOP_H__
#include <cugl/cugl.h>

#include "Chicken.h"

/** The Deck of Chickens */
class Coop {
	private:
		vector <Chicken> chickens;
	public:
		//Constructor
		/** Create a new Coop*/
		Coop();
		/** Fill the coop with Chickens, defaulting to deckV4 */
		void loadDeck();
		void loadDeck(int versionNumber);

		//Access
		/** Get the size of the Coop*/
		int getSize();

		//Modify
		/** Draw a Chicken, removing it from the Coop*/
		Chicken draw();
		/** Clear the coop of all Chickens*/
		void clear();
		/** Add the c Chickens to the Coop */
		void fill(vector <Chicken> c);
};

#endif /* __COOP_H__ */