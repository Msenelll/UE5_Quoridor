# TECHNICAL DESIGN DOCUMENT (TDD)
# Project: Digital Quoridor

**Version:** 1.0 (Phase 1 Baseline)
**Date:** 2025-12-17
**Engine:** Unreal Engine 5.x
**Language:** C++ (Core Logic) & Blueprints (UI/Visuals)
**Architecture Pattern:** MVC Adaptation (Managers as Controllers, Actors as View/Model)

---

## 1. Executive Summary
Bu döküman, Digital Quoridor projesinin teknik mimarisini tanımlar. Tasarım, **Faz 1** (Single Player) gereksinimlerini karşılarken, **Faz 2** (Multiplayer) ve **Faz 3** (Store/Inventory) için gerekli esnekliği ("Decoupled Systems") barındırır.

---

## 2. Class Architecture (C++ Hierarchy)

Oyun mantığı (Logic) ve görsel sunum (Presentation) birbirinden ayrılmıştır.

### 2.1. Core Managers (The Brain)
* **`AQuoridorGameMode`** (`AGameModeBase`)
    * **Responsibility:** Oyun kurallarının otoritesidir. State Machine yönetimini yapar.
    * **Key Functions:**
        * `bool ValidateMove(FVector From, FVector To)`: Hamle geçerliliğini kontrol eder.
        * `bool TryPlaceWall(FVector Loc, EWallOrientation Orient)`: Duvar koyma isteğini işler.
        * `void SwitchTurn()`: Sırayı diğer oyuncuya/bot'a geçirir.
    * **Future Proofing:** Faz 2'de Server-Side otorite burada çalışacaktır.

* **`AQuoridorGridManager`** (`AActor`)
    * **Responsibility:** 9x9 Grid verisini, node bağlantılarını ve pathfinding hesaplamalarını yönetir.
    * **Data:** Tüm `FGridNode` verisi burada tutulur.

* **`UQuoridorPathfinder`** (`UBlueprintFunctionLibrary`)
    * **Responsibility:** Statik yardımcı kütüphane. Algoritmaları barındırır.
    * **Algorithms:**
        * `BFS`: Hızlı "Yol Var mı?" kontrolü (Validation).
        * `A* (A-Star)`: En kısa yol bulma (Bot Intelligence).

### 2.2. Entities (The Actors)
* **`AQuoridorPawn`** (`APawn`)
    * **Responsibility:** Oyuncunun temsili.
    * **Components:** `USkeletalMeshComponent` (Görsel), `UFloatingPawnMovement` (Hareket).
    * **Skin System:** `UQuoridorSkinData` assetini referans alarak görselini oluşturur.

* **`AQuoridorWall`** (`AActor`)
    * **Responsibility:** Fiziksel ve görsel duvar objesi.
    * **Optimization:** **Object Pooling** kullanılır. Sürekli Spawn/Destroy yapılmaz. Sahne başında havuzda oluşturulur, aktif/pasif yapılır.

* **`AQuoridorPlayerController`** (`APlayerController`)
    * **Responsibility:** Input handling ve UI etkileşimi.
    * **Pattern:** Input stratejisini (`IInputStrategy`) kullanarak "Fluid" veya "Precision" modlarını yönetir.

---

## 3. Data Structures & Memory Layout

Grid sistemi, performans (CPU Cache Locality) için tek boyutlu dizi (Flat Array) olarak tutulacaktır.

### 3.1. Grid Node Definition
```cpp
// Kenar durumları (Edge States)
UENUM(BlueprintType)
enum class EEdgeState : uint8 {
    Open = 0,    // Geçiş serbest
    Blocked = 1  // Duvar var
};

USTRUCT(BlueprintType)
struct FGridNode {
    GENERATED_BODY()

    // Grid Index (0-80). Calculation: Index = (y * 9) + x
    UPROPERTY()
    int32 Index;

    // Dünya koordinatı (Piyonun gideceği yer)
    UPROPERTY()
    FVector WorldLocation;

    // Komşuluk Bağlantıları (Graph Edges)
    UPROPERTY()
    EEdgeState North;
    UPROPERTY()
    EEdgeState South;
    UPROPERTY()
    EEdgeState East;
    UPROPERTY()
    EEdgeState West;
};

### 3.2. Data Management
* **Grid Storage:** `TArray<FGridNode> GridNodes;`
    * **Neden:** `TMap` yerine `TArray` kullanımı bellek erişim hızını (CPU Cache Locality) artırır.
* **Wall Storage:** Duvarların koordinatları `TSet<FVector>` içinde tutulabilir (Hızlı lookup/sorgulama için).

---

## 4. Algorithms & Logic

### 4.1. Path Validation (Critical)
Duvar koyma anında çalışan "Yol Kapatma Kontrolü".

* **Algoritma:** Breadth-First Search (BFS).
* **Gerekçe:** Ağırlıksız grafta "Erişilebilirlik" (Reachability) testi için en hafif maliyetli yöntemdir.
* **Akış:**
    1. Duvar geçici olarak "Blocked" işaretlenir.
    2. P1 ve P2 (ve varsa diğer oyuncular) için hedefe BFS çalıştırılır.
    3. Yol yoksa (`PathFound == false`), işlem geri alınır (Revert) ve oyuncuya uyarı verilir.

### 4.2. Artificial Intelligence (AI)
* **Easy:** `RandomLegalMove()` - Rastgele geçerli hamle yapar.
* **Medium:** Greedy Best-First Search - Hedefe en çok yaklaştıran kareyi seçer.
* **Hard:** Minimax (Derinlik: 3-4).
* **Heuristic (Değerlendirme Fonksiyonu):**
    * Kendi yolunu kısalt (+ Puan).
    * Rakibin yolunu uzat (++ Puan).

---

## 5. Input System (Strategy Pattern)
PC ve Mobil deneyimini ayrıştırmak için Interface yapısı kullanılır.

* **Interface:** `IInputStrategy`
    * Fonksiyonlar: `HandleTouchStart()`, `HandleTouchMove()`, `HandleTouchEnd()`
* **Concrete Classes (Uygulayıcı Sınıflar):**
    * `UFluidInputStrategy`: Drag & Drop mantığı (Mobil odaklı).
    * `UPrecisionInputStrategy`: Click & Confirm mantığı (PC/Strateji odaklı).

---

## 6. Asset Management (Scalability)
Faz 3 (Store) hazırlığı için "Data-Driven" içerik yönetimi.

* **Class:** `UQuoridorSkinData : public UPrimaryDataAsset`
* **Properties:**
    * `Mesh`: Skeletal Mesh referansı.
    * `Material`: Instance Dynamic Material referansı.
    * `Portrait`: UI Icon (Texture 2D).
    * `VFX`: Move/Win parçacık efektleri.

---

## 7. Quality Assurance (QA) Tools

* **Debug Helpers:**
    * `DrawDebugGraph`: Grid bağlantılarını sahnede çizer (Yeşil: Açık, Kırmızı: Kapalı).
* **Automation:**
    * `Test_WallValidation`: Kritik duvar senaryolarını (Edge cases) otomatik test eden C++ Unit Test.