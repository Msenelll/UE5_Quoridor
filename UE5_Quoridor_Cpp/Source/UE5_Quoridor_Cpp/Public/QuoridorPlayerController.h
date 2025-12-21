#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuoridorPlayerController.generated.h"

// --- FORWARD DECLARATIONS ---
class AQuoridorGridManager;
class AQuoridorWall;
class AQuoridorPawn;

// --- GİRİŞ MODU ENUM'I ---
UENUM(BlueprintType)
enum class EInputMode : uint8
{
	Movement        UMETA(DisplayName = "Movement Mode"),
	WallPlacement   UMETA(DisplayName = "Wall Placement Mode")
};

UCLASS()
class UE5_QUORIDOR_CPP_API AQuoridorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AQuoridorPlayerController();

	// --- INPUT MODE SYSTEM ---

	// Şu anki modumuz ne?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quoridor|State")
	EInputMode CurrentInputMode;

	// Modu değiştiren fonksiyon (UI'dan çağıracağız)
	UFUNCTION(BlueprintCallable, Category = "Quoridor|Action")
	void SetInputMode(EInputMode NewMode);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	// Mouse Tıklama İşlemleri
	void OnLeftClick();
	void OnRightClick();

	// Mantık Fonksiyonları
	void HandleMoveInput();
	void PlaceWall();
	void UpdateGhostWallPosition();

private:
	// Yönettiğimiz Piyonlar
	UPROPERTY()
	AQuoridorPawn* ControlledPawn;

	UPROPERTY()
	AQuoridorPawn* PawnP1;

	UPROPERTY()
	AQuoridorPawn* PawnP2;

	// Grid Yöneticisi
	UPROPERTY()
	AQuoridorGridManager* GridManagerRef;

	// --- WALL SYSTEM ---

protected:
	// Duvar Blueprint Sınıfı
	UPROPERTY(EditDefaultsOnly, Category = "Quoridor|Config")
	TSubclassOf<AQuoridorWall> WallClass;

	// Hayalet Duvar Referansı
	UPROPERTY()
	AQuoridorWall* GhostWallRef;
};