#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "QuoridorGameState.generated.h"

// Sıra kimde?
UENUM(BlueprintType)
enum class EQuoridorTurn : uint8
{
	Player1		UMETA(DisplayName = "Player 1"),
	Player2		UMETA(DisplayName = "Player 2"),
	None		UMETA(DisplayName = "None") // Oyun bittiğinde veya durduğunda
};

UCLASS()
class UE5_QUORIDOR_CPP_API AQuoridorGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AQuoridorGameState();

	// --- DATA ---

	// Şu anki sıra
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quoridor|State")
	EQuoridorTurn CurrentTurn;

	// --- FUNCTIONS ---

	// Sırayı değiştir (P1 -> P2 veya P2 -> P1)
	UFUNCTION(BlueprintCallable, Category = "Quoridor|Logic")
	void SwitchTurn();

	// Belirli bir oyuncunun sırası mı?
	bool IsPlayerTurn(int32 PlayerID);
};