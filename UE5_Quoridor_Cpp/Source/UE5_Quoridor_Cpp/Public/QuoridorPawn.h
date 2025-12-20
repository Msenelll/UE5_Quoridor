#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "QuoridorPawn.generated.h"

// Forward Declaration (Hızlandırma için)
class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class UE5_QUORIDOR_CPP_API AQuoridorPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AQuoridorPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Input binding (İleride kullanacağız)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- COMPONENTS ---

	// Kök bileşen (Fizik ve Çarpışma için)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* RootCollision;

	// Görsel bileşen (Piyonun şekli)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PawnMesh;

	// --- DATA ---

	// Piyonun şu an hangi karede olduğu (0-80)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quoridor|State")
	int32 CurrentGridIndex;

	// Oyuncu ID (1 = Player 1, 2 = Player 2)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quoridor|Config")
	int32 PlayerID;
};