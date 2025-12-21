#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuoridorWall.generated.h"

class UStaticMeshComponent;

UCLASS()
class UE5_QUORIDOR_CPP_API AQuoridorWall : public AActor
{
	GENERATED_BODY()

public:
	AQuoridorWall();

protected:
	virtual void BeginPlay() override;

public:
	// --- COMPONENTS ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WallMesh;

	// --- STATE ---

	// Duvar Yatay mı (Horizontal) Dikey mi (Vertical)?
	// true = Horizontal (Yatay), false = Vertical (Dikey)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quoridor|State")
	bool bIsHorizontal;

	// Duvarın Kime Ait Olduğu (Player 1 veya 2)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quoridor|State")
	int32 OwnerPlayerID;

	// --- FUNCTIONS ---

	// Duvarın yönünü değiştirir (90 derece çevirir)
	UFUNCTION(BlueprintCallable, Category = "Quoridor|Action")
	void RotateWall();

	// Rengini değiştir (Önizleme için Yeşil/Kırmızı yapacağız)
	UFUNCTION(BlueprintCallable, Category = "Quoridor|Visual")
	void SetWallColor(FLinearColor NewColor);
};