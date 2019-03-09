/** The header file for Stack*/
#ifndef __STACK_H__
#define __STACK_H__
#include <cugl/cugl.h>

#include "Chicken.h"
class Stack {
	public:
		/** Does special chicken effects of the chickens on the top of both stacks */
		void specialChickenEffect(vector <Chicken> stack1, vector <Chicken> stack2);
};

#endif /* __STACK_H__ */