#include "QuoridorPlayerController.h"
#include "QuoridorPawn.h"
#include "Engine/World.h"

AQuoridorPlayerController::AQuoridorPlayerController()
{
	// 1. Mouse İmlecini Göster
	bShowMouseCursor = true;

	// 2. Tıklama olaylarını aç
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AQuoridorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Oyun başladığında kontrol ettiğimiz piyonu tanıyalım
	ControlledPawn = Cast<AQuoridorPawn>(GetPawn());

	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("QuoridorPlayerController: No Pawn Found!"));
	}
}

void AQuoridorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Sol tık basıldığında "OnLeftClick" fonksiyonunu çalıştır
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AQuoridorPlayerController::OnLeftClick);
}

void AQuoridorPlayerController::OnLeftClick()
{
	if (ControlledPawn)
	{
		HandleMoveInput();
	}
}

void AQuoridorPlayerController::HandleMoveInput()
{
	// Mouse'un altındaki objeyi ve konumu bul (Raycast)
	FHitResult HitResult;

	// Ekranda tıkladığımız noktadan dünyaya bir ışın gönderir
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (bHit)
	{
		// Çarpılan nokta (Vector)
		FVector ClickLocation = HitResult.Location;

		// Debug Log (Nereye tıkladık?)
		UE_LOG(LogTemp, Display, TEXT("Clicked at: %s"), *ClickLocation.ToString());

		// Piyona git komutu ver
		ControlledPawn->MoveToTarget(ClickLocation);
	}
}