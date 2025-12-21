#include "QuoridorPlayerController.h"
#include "QuoridorPawn.h"
#include "QuoridorGridManager.h"
#include "QuoridorWall.h"
#include "QuoridorGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"

AQuoridorPlayerController::AQuoridorPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Varsayılan mod: Hareket
	CurrentInputMode = EInputMode::Movement;
}

void AQuoridorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 1. Piyonları Bul
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

	// Varsayılan Piyon (P1)
	ControlledPawn = PawnP1;

	// 2. Grid Manager'ı Bul
	AActor* FoundGridManager = UGameplayStatics::GetActorOfClass(GetWorld(), AQuoridorGridManager::StaticClass());
	GridManagerRef = Cast<AQuoridorGridManager>(FoundGridManager);

	// 3. Kamera Ayarı
	AActor* FoundCamera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	if (FoundCamera)
	{
		SetViewTargetWithBlend(FoundCamera, 0.0f);
		FoundCamera->SetActorEnableCollision(false);
	}

	// 4. Ghost Wall Oluşturma
	if (WallClass)
	{
		FVector SpawnLoc(0.f, 0.f, -500.f);
		FRotator SpawnRot = FRotator::ZeroRotator;

		GhostWallRef = GetWorld()->SpawnActor<AQuoridorWall>(WallClass, SpawnLoc, SpawnRot);

		if (GhostWallRef)
		{
			GhostWallRef->SetActorEnableCollision(false);
			// Başlangıçta gizle (Movement modundayız)
			GhostWallRef->SetActorHiddenInGame(true);
		}
	}
}

void AQuoridorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AQuoridorPlayerController::OnLeftClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AQuoridorPlayerController::OnRightClick);
}

void AQuoridorPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Eğer Duvar Yerleştirme modundaysak hayalet duvarı güncelle
	if (CurrentInputMode == EInputMode::WallPlacement)
	{
		UpdateGhostWallPosition();
	}
}

// --- INPUT HANDLERS ---

void AQuoridorPlayerController::OnLeftClick()
{
	if (!GridManagerRef) return;

	// MODA GÖRE İŞLEM YAP
	if (CurrentInputMode == EInputMode::Movement)
	{
		HandleMoveInput();
	}
	else if (CurrentInputMode == EInputMode::WallPlacement)
	{
		PlaceWall();
	}
}

void AQuoridorPlayerController::OnRightClick()
{
	// Sadece Duvar modundaysak döndür
	if (CurrentInputMode == EInputMode::WallPlacement && GhostWallRef)
	{
		GhostWallRef->RotateWall();
	}
}

void AQuoridorPlayerController::SetGameInputMode(EInputMode NewMode)
{
	CurrentInputMode = NewMode;

	// Ghost Wall Görünürlüğünü Ayarla
	if (GhostWallRef)
	{
		bool bShowGhost = (CurrentInputMode == EInputMode::WallPlacement);

		GhostWallRef->SetActorHiddenInGame(!bShowGhost);

		// Eğer duvar moduna geçildiyse hemen mouse ucuna ışınla
		if (bShowGhost)
		{
			UpdateGhostWallPosition();
		}
	}

	FString ModeName = (CurrentInputMode == EInputMode::Movement) ? TEXT("MOVEMENT") : TEXT("WALL PLACEMENT");
	UE_LOG(LogTemp, Warning, TEXT("Input Mode Changed to: %s"), *ModeName);
}

// --- LOGIC FUNCTIONS ---

void AQuoridorPlayerController::HandleMoveInput()
{
	FHitResult HitResult;
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (bHit && ControlledPawn && GridManagerRef)
	{
		// SIRA KONTROLÜ
		UWorld* World = GetWorld();
		if (World)
		{
			AQuoridorGameState* MyGameState = World->GetGameState<AQuoridorGameState>();

			if (MyGameState && !MyGameState->IsPlayerTurn(ControlledPawn->PlayerID))
			{
				UE_LOG(LogTemp, Warning, TEXT("Sıra sende değil! Bekle..."));
				return;
			}

			// HESAPLAMA VE HAREKET
			FVector RawClickLocation = HitResult.Location;
			FVector SnappedLocation = GridManagerRef->GetClosestGridLocation(RawClickLocation);

			ControlledPawn->MoveToTarget(SnappedLocation);

			// SIRAYI DEVRET
			if (MyGameState)
			{
				MyGameState->SwitchTurn();

				if (MyGameState->CurrentTurn == EQuoridorTurn::Player1) ControlledPawn = PawnP1;
				else if (MyGameState->CurrentTurn == EQuoridorTurn::Player2) ControlledPawn = PawnP2;
			}
		}
	}
}

void AQuoridorPlayerController::PlaceWall()
{
	if (!GhostWallRef || !WallClass) return;

	// SIRA KONTROLÜ
	UWorld* World = GetWorld();
	AQuoridorGameState* MyGameState = World ? World->GetGameState<AQuoridorGameState>() : nullptr;

	// --- VALIDATION (YENİ) ---
	FVector CandidateLoc = GhostWallRef->GetActorLocation();
	bool bIsHorz = GhostWallRef->bIsHorizontal; // Ghost Wall'a bu değişkeni eklemiştik (Public ise erişebilirsin)
	// Eğer bIsHorizontal erişilemiyorsa rotasyondan anlarız:
	// FRotator Rot = GhostWallRef->GetActorRotation();
	// bIsHorz = FMath::IsNearlyEqual(Rot.Yaw, 0.0f, 1.0f) || FMath::IsNearlyEqual(Rot.Yaw, 180.0f, 1.0f); 
	// Ama QuoridorWall.h'da bIsHorizontal public tanımlıydı, direkt erişebilmen lazım.

	if (!GridManagerRef->IsWallPlacementValid(CandidateLoc, GhostWallRef->bIsHorizontal, GhostWallRef))
	{
		UE_LOG(LogTemp, Warning, TEXT("Geçersiz Hamle: Duvarlar çakışıyor!"));
		return;
	}

	if (MyGameState && ControlledPawn)
	{
		if (!MyGameState->IsPlayerTurn(ControlledPawn->PlayerID))
		{
			UE_LOG(LogTemp, Warning, TEXT("Duvar koymak için sıranı bekle!"));
			return;
		}
	}

	// DUVAR KOYMA
	FVector SpawnLoc = GhostWallRef->GetActorLocation();
	FRotator SpawnRot = GhostWallRef->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AQuoridorWall* NewWall = World->SpawnActor<AQuoridorWall>(WallClass, SpawnLoc, SpawnRot, SpawnParams);

	if (NewWall)
	{
		if (ControlledPawn)
		{
			NewWall->OwnerPlayerID = ControlledPawn->PlayerID;
		}

		NewWall->SetActorEnableCollision(true);

		// SIRAYI DEVRET
		if (MyGameState)
		{
			MyGameState->SwitchTurn();

			if (MyGameState->CurrentTurn == EQuoridorTurn::Player1) ControlledPawn = PawnP1;
			else if (MyGameState->CurrentTurn == EQuoridorTurn::Player2) ControlledPawn = PawnP2;
		}
	}
}

void AQuoridorPlayerController::UpdateGhostWallPosition()
{
	if (!GhostWallRef || !GridManagerRef) return;

	FHitResult HitResult;
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (bHit)
	{
		FVector SnappedPos = GridManagerRef->GetClosestWallLocation(HitResult.Location);
		GhostWallRef->SetActorLocation(SnappedPos);
	}
}