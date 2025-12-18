// Fill out your copyright notice in the Description page of Project Settings.

#include "QuoridorGridManager.h"

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

	// Debug Mesajı (Ekrana Yazar - Viewport Sol Üst)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
			FString::Printf(TEXT("Grid Initialized: %d Nodes Created!"), GridNodes.Num()));
	}

	// YENİ EKLEME: Log Dosyasına Yazar (Output Log Panelinde görünür)
	// LogTemp kategorisini kullanıyoruz, Warning renginde (Sarı) çıkacak.
	UE_LOG(LogTemp, Warning, TEXT("SUCCESS: Grid Initialized with %d Nodes!"), GridNodes.Num());
}

void AQuoridorGridManager::InitGrid()
{
	GridNodes.Empty(); // Temizle (Garanti olsun)

	// Grid'in merkezini aktörün sahnedeki konumu yapalım
	FVector OriginLocation = GetActorLocation();

	// 9x9 Döngü
	for (int32 y = 0; y < GridSize; y++)
	{
		for (int32 x = 0; x < GridSize; x++)
		{
			FGridNode NewNode;

			// Matematiksel ID hesaplama: y * Genişlik + x
			NewNode.Index = GetIndexFromCoordinates(x, y);
			NewNode.X = x;
			NewNode.Y = y;

			// Dünya pozisyonunu hesapla
			// Örn: x=0 -> 0, x=1 -> 100, x=2 -> 200...
			float WorldX = x * TileSize;
			float WorldY = y * TileSize;

			// Actor'ün konumuna göre offset ekle
			NewNode.WorldLocation = OriginLocation + FVector(WorldX, WorldY, 0.0f);

			// Array'e ekle
			GridNodes.Add(NewNode);
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