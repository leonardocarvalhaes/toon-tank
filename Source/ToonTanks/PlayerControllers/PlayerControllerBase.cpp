#include "PlayerControllerBase.h"

void APlayerControllerBase::SetPlayerEnabledState(bool SetPlayerEnabled)
{
	if(SetPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
		bShowMouseCursor = SetPlayerEnabled;
	}
	// else
	// {
	// 	GetPawn()->DisableInput(this);
	// }
}
