#include "QuoridorWall.h"
#include "Components/StaticMeshComponent.h"

AQuoridorWall::AQuoridorWall()
{
	PrimaryActorTick.bCanEverTick = false; // Duvarlar hareket etmez, Tick gereksiz.

	// 1. Mesh Bileşeni
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;

	// Varsayılan Duvar Boyutu (Unreal küpü 100x100x100'dür)
	// Bizim duvarımız ince ve uzun olmalı. (Örn: 2 birim kareyi kapatacak)
	// Grid aralığı 100 birim. Duvar kalınlığı 20, uzunluğu 180 olsun.
	// Scale: X=1.8 (180cm), Y=0.2 (20cm), Z=1.0 (100cm)
	WallMesh->SetWorldScale3D(FVector(1.8f, 0.2f, 1.0f));

	bIsHorizontal = true; // Varsayılan yatay başlasın
	OwnerPlayerID = 0;
}

void AQuoridorWall::BeginPlay()
{
	Super::BeginPlay();
}

void AQuoridorWall::RotateWall()
{
	// Durumu tersine çevir
	bIsHorizontal = !bIsHorizontal;

	// Görseli 90 derece döndür
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += 90.0f;
	SetActorRotation(NewRotation);

	UE_LOG(LogTemp, Display, TEXT("Wall Rotated. IsHorizontal: %d"), bIsHorizontal);
}

void AQuoridorWall::SetWallColor(FLinearColor NewColor)
{
	// Materyal instance oluşturup rengi değiştireceğiz (Blueprint'te daha detaylı yapacağız)
	// Şimdilik boş bırakıyoruz, BP tarafında halletmek daha kolaydır.
}