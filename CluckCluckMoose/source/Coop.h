/** The header file for Coops*/
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
		/** Fill the coop with Chickens */
		void refresh();

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
