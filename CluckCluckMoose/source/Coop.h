/** The header file for Coops*/
#include <cugl/cugl.h>

/** The Deck of Chickens */
class Coop {
	private:
		vector <Chicken> chickens;
	public:
		Coop();
		Chicken draw();
		int getSize();
};
