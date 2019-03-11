#include "Clash.h"

using namespace cugl;

void Clash::resolve(Moose player1, Moose player2) {
	while (!player1.getStack().empty() && !player2.getStack().empty())
	{
		int result = player1.getStack().getBottom()->compare(*player2.getStack().getBottom());
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
	}
}
