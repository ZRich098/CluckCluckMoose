#include "Clash.h"

using namespace cugl;

void Clash::resolve(Moose player1, Moose player2) {
	bool empty1 = player1.getStack().empty();
	bool empty2 = player2.getStack().empty();
	while (!empty1 && !empty2)
	{
		int result = player1.getStack().front().compare(player2.getStack().front());
		if (result == -1)
		{
			player1.removeBottomFromStackToDiscard();
		}
		else if (result == 1)
		{
			player2.removeBottomFromStackToDiscard();
		}
		else
		{
			player1.removeBottomFromStackToDiscard();
			player2.removeBottomFromStackToDiscard();
		}

		empty1 = player1.getStack().empty();
		empty2 = player2.getStack().empty();
	}
}