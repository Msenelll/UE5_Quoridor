#include "QuoridorGameState.h"

AQuoridorGameState::AQuoridorGameState()
{
	// Varsayılan olarak Player 1 başlar
	CurrentTurn = EQuoridorTurn::Player1;
}

void AQuoridorGameState::SwitchTurn()
{
	if (CurrentTurn == EQuoridorTurn::Player1)
	{
		CurrentTurn = EQuoridorTurn::Player2;
		UE_LOG(LogTemp, Warning, TEXT("Turn Switched: PLAYER 2"));
	}
	else
	{
		CurrentTurn = EQuoridorTurn::Player1;
		UE_LOG(LogTemp, Warning, TEXT("Turn Switched: PLAYER 1"));
	}
}

bool AQuoridorGameState::IsPlayerTurn(int32 PlayerID)
{
	if (PlayerID == 1 && CurrentTurn == EQuoridorTurn::Player1) return true;
	if (PlayerID == 2 && CurrentTurn == EQuoridorTurn::Player2) return true;

	return false;
}