#include "QuoridorPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Constructor: Objeler burada yaratılır.
AQuoridorPawn::AQuoridorPawn()
{
	// Piyon hareket edeceği için Tick açık kalsın.
	PrimaryActorTick.bCanEverTick = true;

	// 1. Root Collision (Kapsül) Oluşturma
	RootCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCollision"));
	RootComponent = RootCollision;

	// Kapsül boyutları (Quoridor taşına uygun ince uzun)
	RootCollision->SetCapsuleHalfHeight(40.0f);
	RootCollision->SetCapsuleRadius(20.0f);

	// 2. Mesh (Görsel) Oluşturma
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(RootComponent);

	// Mesh'i kapsülün içine ortalamak için hafif aşağı çekelim
	PawnMesh->SetRelativeLocation(FVector(0.f, 0.f, -40.f));

	// Varsayılan Değerler
	CurrentGridIndex = -1;
	PlayerID = 1;
}

void AQuoridorPawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Pawn Spawned: Player %d"), PlayerID);
}

void AQuoridorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Hareket kodlarını bir sonraki adımda buraya yazacağız.
}

void AQuoridorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}