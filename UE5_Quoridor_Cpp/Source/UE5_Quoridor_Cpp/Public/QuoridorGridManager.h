// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuoridorGridManager.generated.h"

// TDD 3.1: Grid Node Veri Yapısı
// BlueprintType: Bu struct'ın Blueprint (Görsel Kodlama) içinde görünmesini sağlar.
USTRUCT(BlueprintType)
struct FGridNode
{
	GENERATED_BODY()

	// Grid üzerindeki 0-80 arasındaki ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Index;

	// Matris Koordinatları (x, y)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 X;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Y;

	// Dünyadaki fiziksel koordinat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector WorldLocation;

	// Komşuluk verileri (TDD: EEdgeState kullanımı)
	// True = Duvar var (Blocked), False = Yol açık
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bNorthBlocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bSouthBlocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bEastBlocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bWestBlocked = false;

	// Constructor (Varsayılan değerler)
	FGridNode() : Index(-1), X(0), Y(0), WorldLocation(FVector::ZeroVector) {}
};

UCLASS()
class UE5_QUORIDOR_CPP_API AQuoridorGridManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuoridorGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Grid oluşturma fonksiyonu (Sprint 1 Goal)
	void InitGrid();

	// Helper: Koordinattan index bulma
	// BlueprintCallable: Bu fonksiyonu BP içinde çağırabilirsin.
	UFUNCTION(BlueprintCallable, Category = "Quoridor|Grid")
	int32 GetIndexFromCoordinates(int32 x, int32 y) const;

	// Helper: Index'ten Node verisine ulaşma
	UFUNCTION(BlueprintCallable, Category = "Quoridor|Grid")
	bool GetNodeByIndex(int32 Index, FGridNode& OutNode) const;

protected:
	// TDD 3.2: Grid Storage
	// Bellekte tutulan ana veri (Flat Array - Cache Friendly)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quoridor|Grid")
	TArray<FGridNode> GridNodes;

	// Grid Ayarları (Editörden değiştirilebilir)
	UPROPERTY(EditAnywhere, Category = "Quoridor|Config")
	int32 GridSize = 9;

	UPROPERTY(EditAnywhere, Category = "Quoridor|Config")
	float TileSize = 100.0f; // 1 metre aralık
};