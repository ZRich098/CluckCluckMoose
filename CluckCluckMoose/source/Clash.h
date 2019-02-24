/** The header file for Clash*/
#ifndef __CLASH_H__
#define __CLASH_H__
#include <cugl/cugl.h>

#include "Moose.h"

/** The Clash Resolution */
class Clash {
public:
	//Stack
	/** Resolves the clash of the two players passed in */
	void resolve(Moose player1, Moose player2);
};

#endif /* __CLASH_H__ */