#include "QuoridorPlayerController.h"
// ÖNCE kendi header'ı, SONRA diğerleri:

#include "QuoridorPawn.h"
#include "QuoridorGridManager.h"        // <--- EKSİKTİ: GridManager'ın içini görmesini sağlar
#include "Kismet/GameplayStatics.h"     // <--- EKSİKTİ: GetActorOfClass fonksiyonunu barındırır
#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "QuoridorGameState.h"

// ... Kodun geri kalanı aynı ...

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



	// --- 1. TÜM PİYONLARI BUL ---
	TArray<AActor*> FoundPawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuoridorPawn::StaticClass(), FoundPawns);

	for (AActor* Actor : FoundPawns)
	{
		AQuoridorPawn* P = Cast<AQuoridorPawn>(Actor);
		if (P)
		{
			if (P->PlayerID == 1) PawnP1 = P;
			else if (P->PlayerID == 2) PawnP2 = P;
		}
	}

	// Varsayılan olarak Player 1 ile başla
	ControlledPawn = PawnP1;

	// 2. Grid Manager'ı Bul
	AActor* FoundGridManager = UGameplayStatics::GetActorOfClass(GetWorld(), AQuoridorGridManager::StaticClass());
	GridManagerRef = Cast<AQuoridorGridManager>(FoundGridManager);

	if (!GridManagerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("FATAL: GridManager not found!"));
	}

	// --- YENİ KOD: SABİT KAMERAYA GEÇİŞ ---

	// Sahnede bulunan ilk "CameraActor"ü bul.
	// (Zaten sahnede tek bir kamera var, o yüzden sorun olmaz)
	// --- KAMERA AYARLARI ---
	AActor* FoundCamera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());

	if (FoundCamera)
	{
		// 1. Görüşü Kameraya Geçir
		SetViewTargetWithBlend(FoundCamera, 0.0f);

		// --- YENİ EKLENEN SATIR ---
		// Kameranın fiziksel varlığını (Collision) tamamen kapatıyoruz.
		// Böylece Mouse ışınları içinden geçip yere ulaşacak.
		FoundCamera->SetActorEnableCollision(false);

		UE_LOG(LogTemp, Warning, TEXT("Camera Found & Collision DISABLED!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Camera Actor NOT FOUND in the level!"));
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
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (bHit && ControlledPawn && GridManagerRef)
	{
		// 1. GAME STATE VE SIRA KONTROLÜ
		UWorld* World = GetWorld();
		if (World)
		{
			AQuoridorGameState* MyGameState = World->GetGameState<AQuoridorGameState>();

			// GameState varsa VE Sıra bizde değilse -> DUR!
			if (MyGameState && !MyGameState->IsPlayerTurn(ControlledPawn->PlayerID))
			{
				UE_LOG(LogTemp, Warning, TEXT("Sıra sende değil! Bekle..."));
				return; // Fonksiyondan çık
			}

			// 2. HESAPLAMA (Bu kısım eksikti veya aşağıdaydı)
			FVector RawClickLocation = HitResult.Location;

			// GridManager'a sor: "Buna en yakın kare neresi?"
			// DEĞİŞKEN BURADA TANIMLANIYOR:
			FVector SnappedLocation = GridManagerRef->GetClosestGridLocation(RawClickLocation);

			// 3. HAREKET EMRİ
			UE_LOG(LogTemp, Display, TEXT("Click: %s -> Snap: %s"),
				*RawClickLocation.ToString(), *SnappedLocation.ToString());

			ControlledPawn->MoveToTarget(SnappedLocation);

			// 4. SIRAYI DEVRET VE PİYONU GÜNCELLE
			if (MyGameState)
			{
				MyGameState->SwitchTurn();

				// YENİ: Aktif piyonu değiştir (Hot Seat Logic)
				if (MyGameState->CurrentTurn == EQuoridorTurn::Player1)
				{
					ControlledPawn = PawnP1;
				}
				else if (MyGameState->CurrentTurn == EQuoridorTurn::Player2)
				{
					ControlledPawn = PawnP2;
				}

				UE_LOG(LogTemp, Display, TEXT("Control Switched to Player %d"), ControlledPawn ? ControlledPawn->PlayerID : 0);
			}
		}
	}
}