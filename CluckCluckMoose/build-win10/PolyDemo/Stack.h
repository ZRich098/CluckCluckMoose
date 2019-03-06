/** The header file for Stack*/
#ifndef __STACK_H__
#define __STACK_H__
#include <cugl/cugl.h>

#include "Chicken.h"
class Stack {
	public:
		/** Does special chicken effects of the chicken on the top of stack1 */
		void specialChickenEffect(vector <Chicken> stack1, vector <Chicken> stack2);
		/** Determines which special chicken should have its effects go first */
		special whichSpecial(special s1, special s2);
};

#endif /* __STACK_H__ */