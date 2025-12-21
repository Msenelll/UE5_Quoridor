#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuoridorPlayerController.generated.h"

// Forward Declaration
class AQuoridorPawn;
class AQuoridorGridManager;

UCLASS()
class UE5_QUORIDOR_CPP_API AQuoridorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AQuoridorPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Mouse Tıklama İşlemi
	void OnLeftClick();

	// Tıklanan yerdeki koordinatı bulma
	void HandleMoveInput();

private:
	// Yönettiğimiz Piyonun referansını tutalım (Cast maliyetinden kaçınmak için)
	UPROPERTY()
	AQuoridorPawn* ControlledPawn;
	UPROPERTY()
	AQuoridorGridManager* GridManagerRef;
	// İki piyonu da hafızada tutalım
	UPROPERTY()
	AQuoridorPawn* PawnP1;

	UPROPERTY()
	AQuoridorPawn* PawnP2;

	// ControlledPawn zaten var, onu aktif olanı göstermek için kullanacağız.

};