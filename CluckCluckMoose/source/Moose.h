/** The header file for Coops*/
#include <cugl/cugl.h>

#include "Coop.h"

/** The Deck of Chickens */
class Moose {
	private:
		vector <Chicken> stack; //Pecking Order >:D
		vector <Chicken> hatched; //Hand
		Coop unhatched; //Deck
		vector <Chicken> deceased; //Discard
		int health;
	public:
		Moose();
		vector <Chicken> getHatched(); //Hand
		Coop getUnhatched(); //Deck
		vector <Chicken> getDeceased(); //Discard
		int getHealth();
		void setHealth(int health);
		void takeDamage(int damage);
};
