#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "QuoridorPawn.generated.h"

// Forward Declaration
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

	// Input binding
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- COMPONENTS ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* RootCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PawnMesh;

	// --- MOVEMENT SYSTEM ---

	// Bu fonksiyonu dışarıdan çağıracağız
	UFUNCTION(BlueprintCallable, Category = "Quoridor|Movement")
	void MoveToTarget(FVector NewTargetLocation);

	// --- DATA ---

	// Piyonun şu an hangi karede olduğu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quoridor|State")
	int32 CurrentGridIndex;

	// Oyuncu ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quoridor|Config")
	int32 PlayerID;

protected:
	// Hareket ediyor muyuz?
	bool bIsMoving;

	// Hedef koordinat
	FVector TargetLocation;

	// Hareket hızı
	UPROPERTY(EditAnywhere, Category = "Quoridor|Config")
	float MovementSpeed = 10.0f;
};