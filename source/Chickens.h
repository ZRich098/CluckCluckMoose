
/** The header file for Chickens and Coops*/
#include <cugl/cugl.h>

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

/** The Deck of Chickens */
class Coop {
private:
    vector <Chicken> chickens;
public:
    Coop();
    Chicken draw();
    int getSize();
};
