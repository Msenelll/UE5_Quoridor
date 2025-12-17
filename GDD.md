# GAME DESIGN DOCUMENT (GDD)
# Project: Digital Quoridor

**Version:** 1.3
**Status:** Approved for Development
**Platform:** PC (Steam) & Mobile (iOS/Android)
**Engine:** Unreal Engine 5 (C++ & Blueprint)

---

## 1. Executive Summary
"Digital Quoridor", klasik masa oyununun modern, rekabetçi ve çapraz platform destekli bir dijital uyarlamasıdır. Proje, "Önce Ürün, Sonra Servis" (Product First, Service Later) prensibiyle, ölçeklenebilir bir mimari üzerine inşa edilecektir.

---

## 2. Game Rules & Logic (Algorithmic Definitions)
Oyunun temel kuralları, kod tarafında "State Machine" ve "Validation Logic" olarak birebir uygulanacaktır.

### 2.1. The Board (Grid System)
* **Boyut:** 9x9 Kare (Node). Toplam 81 Node.
* **Koordinat Sistemi:** `(x, y)` formatında, `0 <= x <= 8` ve `0 <= y <= 8`.
* **Başlangıç Pozisyonları:**
    * **Oyuncu 1 (White/Bottom):** Başlangıç `(4, 0)`. Hedef `y = 8` satırındaki herhangi bir kare.
    * **Oyuncu 2 (Black/Top):** Başlangıç `(4, 8)`. Hedef `y = 0` satırındaki herhangi bir kare.

### 2.2. Turn System (Sıra Mantığı)
* Oyun sıra tabanlıdır (Turn-Based).
* Her turda oyuncu **tek bir** aksiyon seçmek zorundadır:
    1.  **Piyon Hareketi (Move Pawn)**
    2.  **Duvar Koyma (Place Wall)** (Eğer duvar hakkı > 0 ise).

### 2.3. Pawn Movement Logic (Piyon Hareketi)
Piyonlar sadece ortogonal (Dikey ve Yatay) hareket edebilir. Çapraz hareket sadece özel durumlarda geçerlidir.

* **Temel Hareket:** Mevcut `(x, y)` konumundan; `(x+1, y)`, `(x-1, y)`, `(x, y+1)`, `(x, y-1)` karelerine gidilebilir.
* **Engel Kontrolü:** Eğer hedef kare ile mevcut kare arasında **Duvar** varsa, hareket geçersizdir.
* **Jump Rule (Atlama Kuralı - Edge Case):**
    * Eğer piyon, rakip piyonla bitişikse (Face-to-Face):
        * *Durum A (Arka Boş):* Rakibin arkasındaki kare boşsa ve aralarında duvar yoksa, piyon rakibin üzerinden atlayabilir (`+2` birim hareket).
        * *Durum B (Arka Dolu/Duvarlı):* Rakibin arkasında duvar varsa veya oyun alanı bitiyorsa; piyon rakibin sağına veya soluna **çapraz** (Diagonal) hareket edebilir.

### 2.4. Wall Placement Logic (Duvar Koyma)
* **Envanter:** Her oyuncu 10 adet duvar ile başlar.
* **Boyut:** Duvarlar 2 kare uzunluğundadır (2x1 gap).
* **Yerleşim:** Duvarlar karelerin üzerine değil, aralarındaki boşluklara (Grooves) konur.
* **Validasyon Kuralları (Invalid Moves):**
    1.  **Overlap:** Başka bir duvarın üzerine konamaz.
    2.  **Intersect:** Başka bir duvarı kesemez (Haç şeklinde).
    3.  **Path Blocking (Critical):** Konulan duvar, **HERHANGİ** bir oyuncunun hedefe giden yolunu tamamen kapatamaz.
        * *Algoritma:* Her duvar koyma denemesinde `Pathfinding Check` çalışır. Eğer `PathCount == 0` dönerse, hamle reddedilir.

---

## 3. Development Phases (Roadmap)

### FAZ 1: The Foundation (MVP - Single Player)
* **Odak:** Temel mekanikler, AI ve UI/UX.
* **Modes:** Player vs AI (Easy, Medium, Hard), Local PvP (Pass & Play).
* **Monetization:** Interstitial & Rewarded Ads.
* **Architecture:** Skin ve Multiplayer altyapısına uygun kod mimarisi (Interface-based). Market kapalı.

### FAZ 2: Connected World (Multiplayer Update)
* **Odak:** Online bağlantı ve rekabet.
* **New Features:** Online Matchmaking, Friend Lobby.
* **Backend:** PlayFab/Firebase Auth & Matchmaking.

### FAZ 3: The Economy (Live-Ops & Store)
* **Odak:** Gelir modeli ve özelleştirme.
* **New Features:** In-Game Store, Inventory System, IAP (Satın alımlar).
* **Content:** Yeni Skinler, VFX paketleri, Arena temaları.

---

## 4. Interaction & Controls (UX)

### 4.1. Fluid Mode (Drag & Drop) - Default Mobile
* **Aksiyon:** Sürükle-Bırak.
* **Feedback:** Anlık "Ghost Wall" gösterimi ve manyetik "Snapping".

### 4.2. Precision Mode (Select & Confirm) - Default PC
* **Aksiyon:** Tıkla -> Önizle -> Döndür -> Onayla.
* **Feedback:** Yeşil/Kırmızı renk kodları ile hamle geçerliliği (Valid/Invalid state).

---

## 5. Technical Constraints
* **FPS Target:** 60 FPS (Mobile & PC).
* **Poly Count:** Low-poly estetik, Mobile performansı için optimize edilmiş shaderlar.
* **Network:** Server-Authoritative Logic (Faz 2 hazırlığı için yerel oyunda bile inputlar validate edilerek işlenir).