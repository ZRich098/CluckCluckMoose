/** The header file for Chickens */
#include <cugl/cugl.h>

// TODO:: Entity Component elements and specials.  Or Enums
string elements[] = { "None", "Fire", "Water", "Grass" };
string specials[] = { "None",
					"Draw", // Draw a Card
					"Thicken", //Swap with bottom chicken
					"Mirror", //Become opposing chicken
					"Null", //Nullify opposing chicken
					"Reaper", //Ties with all chickens
					"Cycle" }; //Cycle chicken underneath

class Chicken{
	private:
		string element;
		string special;
	public:
		//Constructor
		/** Create a new Chicken*/
		Chicken();
		/** Create a Chicken of element e and special s */
		Chicken(string e, string s);

		//Access
		/** Returns the element of the Chicken */
		string getElement();
		/** Returns the special ability of the Chicken */
		string getSpecial();
		/** Returns a string of the Chicken's details */
		string toString();

		//Modify
		/** Set a Chicken's element to e and special to s */
		void setChicken(string e, string s);
};