/** The header file for Coops*/
#include <cugl/cugl.h>

#include "Chicken.h"

/** The Deck of Chickens */
class Coop {
	private:
		vector <Chicken> chickens;
	public:
		//Constructor
		Coop();
		void refresh();

		//Access
		int getSize();

		//Modify
		Chicken draw();
		void clear();
		void fill(vector <Chicken> c);
};
