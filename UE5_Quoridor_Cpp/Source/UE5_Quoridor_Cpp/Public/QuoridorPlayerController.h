#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuoridorPlayerController.generated.h"

// --- FORWARD DECLARATIONS (İLERİ BİLDİRİM) ---
// Derleyiciye "Merak etme, böyle sınıflar var, detayları .cpp'de" diyoruz.
class AQuoridorGridManager;
class AQuoridorWall;          // <--- BU EKSİKTİ! Hatanın sebebi bu.
class AQuoridorPawn;

UCLASS()
class UE5_QUORIDOR_CPP_API AQuoridorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AQuoridorPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override; // Tick fonksiyonunu unutmuyoruz

	// Mouse Tıklama İşlemi
	void OnLeftClick();

	// Tıklanan yerdeki koordinatı bulma
	void HandleMoveInput();

	// Ghost Wall Pozisyonunu Güncelleme
	void UpdateGhostWallPosition();

private:
	// Yönettiğimiz Piyonlar
	UPROPERTY()
	AQuoridorPawn* ControlledPawn;

	UPROPERTY()
	AQuoridorPawn* PawnP1;

	UPROPERTY()
	AQuoridorPawn* PawnP2;

	// Grid Yöneticisi Referansı
	UPROPERTY()
	AQuoridorGridManager* GridManagerRef;

	// --- WALL SYSTEM ---

protected:
	// Editörden seçeceğimiz Duvar Blueprint'i
	// TSubclassOf kullanabilmek için yukarıda class AQuoridorWall; yazmalıydık.
	UPROPERTY(EditDefaultsOnly, Category = "Quoridor|Config")
	TSubclassOf<AQuoridorWall> WallClass;

	// Sahnedeki hayalet duvar örneği
	UPROPERTY()
	AQuoridorWall* GhostWallRef;
};