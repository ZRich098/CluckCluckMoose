/** The header file for Chickens */
#include <cugl/cugl.h>

// TODO:: Entity Component elements and specials.
enum class element { None, Fire, Water, Grass };

enum class special { None,
					Draw, // Draw a Card
					Thicken, //Swap with bottom chicken
					Mirror, //Become opposing chicken
					Null, //Nullify opposing chicken
					Reaper, //Ties with all chickens
					Cycle }; //Cycle chicken underneath

class Chicken{
	private:
		element e;
		string eString(element e);
		special s;
		string sString(special s);
	public:
		//Constructor
		/** Create a new Chicken*/
		Chicken();
		/** Create a Chicken of element e and special ability s */
		Chicken(element e, special s);

		//Access
		/** Returns the element of the Chicken */
		element getElement();
		/** Returns the special ability of the Chicken */
		special getSpecial();
		/** Returns a string of the Chicken's details */
		string toString();

		//Modify
		/** Set a Chicken's element to e and special to s */
		void setChicken(element e, special s);
};