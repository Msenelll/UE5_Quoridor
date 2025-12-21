#include "QuoridorPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Constructor
AQuoridorPawn::AQuoridorPawn()
{
	// Hareket (Tick) açık olmalı
	PrimaryActorTick.bCanEverTick = true;

	// 1. Root Collision
	RootCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCollision"));
	RootComponent = RootCollision;
	RootCollision->SetCapsuleHalfHeight(40.0f);
	RootCollision->SetCapsuleRadius(20.0f);

	// 2. Mesh
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(RootComponent);
	PawnMesh->SetRelativeLocation(FVector(0.f, 0.f, -40.f));

	// Varsayılan Değerler
	CurrentGridIndex = -1;
	PlayerID = 1;
	bIsMoving = false;
	MovementSpeed = 5.0f; // Hızı biraz düşürdüm, hareketi net görelim
}

void AQuoridorPawn::BeginPlay()
{
	Super::BeginPlay();

	// Başlangıçta hedef mevcut konum olsun
	TargetLocation = GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("Pawn Spawned: Player %d"), PlayerID);
}

void AQuoridorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		FVector CurrentLoc = GetActorLocation();

		// Süzülme işlemi (Interpolation)
		FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLocation, DeltaTime, MovementSpeed);

		SetActorLocation(NewLoc);

		// Hedefe çok yaklaştıysak durdur
		if (FVector::Dist(NewLoc, TargetLocation) < 1.0f)
		{
			SetActorLocation(TargetLocation); // Tam oturt
			bIsMoving = false;
			UE_LOG(LogTemp, Display, TEXT("Movement Finished!"));
		}
	}
}

void AQuoridorPawn::MoveToTarget(FVector NewTargetLocation)
{
	TargetLocation = NewTargetLocation;

	// ESKİ KOD (Hatalı olan):
	// TargetLocation.Z = GetActorLocation().Z;

	// YENİ KOD (Düzeltme):
	// Piyon nerede olursa olsun, hareket ederken Z=50 (Yerden biraz yukarı) hizasına insin.
	TargetLocation.Z = 50.0f;

	bIsMoving = true;

	UE_LOG(LogTemp, Display, TEXT("Moving to: %s"), *TargetLocation.ToString());
}

void AQuoridorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}