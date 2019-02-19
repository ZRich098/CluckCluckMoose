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
		Chicken();
		Chicken(string e, string s);
		void setChicken(string e, string s);
		string getElement();
		string getSpecial();
		string getChicken();
};