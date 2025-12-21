// Fill out your copyright notice in the Description page of Project Settings.

#include "QuoridorGridManager.h"
#include "DrawDebugHelpers.h"
// Constructor
AQuoridorGridManager::AQuoridorGridManager()
{
	// Bu aktörün her karede (frame) güncelleme almasına gerek yok. Performans için kapattık.
	PrimaryActorTick.bCanEverTick = false;

	// Varsayılan değerler
	GridSize = 9;
	TileSize = 100.0f;
}

// Oyun başladığında çalışır
void AQuoridorGridManager::BeginPlay()
{
	Super::BeginPlay();

	// Gridi oluştur
	InitGrid();

	// --- LOCALIZATION TEST ---
	// DÜZELTME: Tırnak içlerini TEXT() makrosuna aldık.
	// Hatalı: "Common_Loading" -> Doğru: TEXT("Common_Loading")

	FText LoadingText = FText::FromStringTable(
		TEXT("/Game/_Game/Localization/ST_UI.ST_UI"),
		TEXT("Common_Loading")
	);

	// 1. Ekrana Yazdır
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan,
			FString::Printf(TEXT("Loc Test: %s"), *LoadingText.ToString()));
	}

	// 2. Log'a Yazdır
	UE_LOG(LogTemp, Warning, TEXT("LOCALIZATION SUCCESS: %s"), *LoadingText.ToString());
}

void AQuoridorGridManager::InitGrid()
{
	GridNodes.Empty();

	FVector OriginLocation = GetActorLocation();

	for (int32 y = 0; y < GridSize; y++)
	{
		for (int32 x = 0; x < GridSize; x++)
		{
			FGridNode NewNode;
			NewNode.Index = GetIndexFromCoordinates(x, y);
			NewNode.X = x;
			NewNode.Y = y;

			float WorldX = x * TileSize;
			float WorldY = y * TileSize;

			// Z eksenini 10 birim yukarı kaldıralım ki yerin içine girmesin
			NewNode.WorldLocation = OriginLocation + FVector(WorldX, WorldY, 10.0f);

			GridNodes.Add(NewNode);

			// --- GÖRSELLEŞTİRME (DEBUG) ---
			// Her noktanın olduğu yere kırmızı bir küre çiz.
			// Radius: 15cm, Segments: 12, Color: Red, Persistent: True (Sürekli kalır)
			DrawDebugSphere(GetWorld(), NewNode.WorldLocation, 15.0f, 12, FColor::Red, true, -1.0f);

			// Koordinatları da yazalım (Opsiyonel)
			// DrawDebugString(GetWorld(), NewNode.WorldLocation, FString::Printf(TEXT("%d,%d"), x, y), nullptr, FColor::White, -1.0f, false, 1.0f);
		}
	}
}

int32 AQuoridorGridManager::GetIndexFromCoordinates(int32 x, int32 y) const
{
	// Sınır kontrolü (Safety Check - SegFault önlemek için)
	if (x < 0 || x >= GridSize || y < 0 || y >= GridSize)
	{
		return -1;
	}

	return (y * GridSize) + x;
}

bool AQuoridorGridManager::GetNodeByIndex(int32 Index, FGridNode& OutNode) const
{
	if (GridNodes.IsValidIndex(Index))
	{
		OutNode = GridNodes[Index];
		return true;
	}
	return false;
}

FVector AQuoridorGridManager::GetClosestGridLocation(FVector InputLocation)
{
	// 1. Gelen konumu Local konuma çevir (Grid'in kendi dünyasına)
	// Grid (0,0)'da olmayabilir, bu yüzden ActorLocation'ı çıkarıyoruz.
	FVector LocalPos = InputLocation - GetActorLocation();

	// 2. Matematiksel Yuvarlama (Quantization)
	// Örnek: Tıklanan 440, TileSize 100 ise -> 4.4 -> Yuvarla(4) -> 400.
	int32 X_Index = FMath::RoundToInt(LocalPos.X / TileSize);
	int32 Y_Index = FMath::RoundToInt(LocalPos.Y / TileSize);

	// 3. Sınır Kontrolü (Clamp)
	// Tıklama gridin çok dışındaysa bile en yakın kenara çek.
	X_Index = FMath::Clamp(X_Index, 0, GridSize - 1);
	Y_Index = FMath::Clamp(Y_Index, 0, GridSize - 1);

	// 4. Tekrar Dünya Koordinatına Çevir
	float WorldX = X_Index * TileSize;
	float WorldY = Y_Index * TileSize;

	// Z eksenini sabit tutuyoruz (Grid noktaları Z=10'daydı)
	return GetActorLocation() + FVector(WorldX, WorldY, 10.0f);
}