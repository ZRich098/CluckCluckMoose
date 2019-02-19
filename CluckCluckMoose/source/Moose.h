/** The header file for Coops*/
#include <cugl/cugl.h>

/** The Deck of Chickens */
class Moose {
	private:
		int health;
		vector <Chicken> Stack;
	public:
		Moose();
		int getHealth();
};
