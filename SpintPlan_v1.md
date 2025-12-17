\# DEVELOPMENT ROADMAP (SPRINT PLAN)

\# Project: Digital Quoridor



\*\*Version:\*\* 1.1

\*\*Date:\*\* 2025-12-17

\*\*Phase:\*\* Phase 1 (Single Player MVP)

\*\*Total Duration:\*\* 14 Weeks (7 Sprints x 2 Weeks)



---



\## 🌍 Supported Locales (Tier 1 Markets)

Oyun mimarisi aşağıdaki 20 dili destekleyecek şekilde (Font \& UI Layout) tasarlanacaktır:

1\. English (Base)

2\. Turkish

3\. Spanish

4\. Portuguese (Brazil)

5\. French

6\. German

7\. Italian

8\. Russian

9\. Chinese (Simplified)

10\. Chinese (Traditional)

11\. Japanese

12\. Korean

13\. Arabic (RTL)

14\. Indonesian

15\. Vietnamese

16\. Thai

17\. Hindi

18\. Polish

19\. Dutch

20\. Swedish



---



\## 🏁 SPRINT 1: The Foundation \& Data Architecture

\*\*Goal:\*\* Oyun dünyasının (Grid) matematiksel ve görsel olarak oluşturulması, dil altyapısının kurulması.



\* \*\*Setup \& Config\*\*

&nbsp;   \* \[ ] Unreal Engine 5 Mobile Project Setup (Scalable settings).

&nbsp;   \* \[ ] Git Repo Initialization \& `.gitignore`.

&nbsp;   \* \[ ] Folder Structure (`Source/Core`, `Content/Localization`, `Content/Audio`).

\* \*\*Core C++ (Data)\*\*

&nbsp;   \* \[ ] `FGridNode` struct implementasyonu.

&nbsp;   \* \[ ] `AQuoridorGridManager` class oluşturulması.

&nbsp;   \* \[ ] `InitGrid()` fonksiyonu (81 Nodes generation).

\* \*\*Localization Setup\*\*

&nbsp;   \* \[ ] \*\*Localization Dashboard\*\* kurulumu.

&nbsp;   \* \[ ] \*\*String Table\*\* yapısının oluşturulması (`ST\_UI`, `ST\_Dialogues`). Hard-coded string kullanımının yasaklanması.

\* \*\*Testing\*\*

&nbsp;   \* \[ ] Output Log kontrolü: Grid başarıyla oluşturuldu mu?



---



\## ♟️ SPRINT 2: Pawn, Movement \& Audio Base

\*\*Goal:\*\* Piyonun temel hareketi ve ses yöneticisinin entegrasyonu.



\* \*\*Core C++ (Player)\*\*

&nbsp;   \* \[ ] `AQuoridorPawn` class oluşturulması.

&nbsp;   \* \[ ] `Turn System` iskeletinin `GameMode` içine kurulması.

\* \*\*Audio System\*\*

&nbsp;   \* \[ ] \*\*Sound Manager:\*\* `USoundClass` (SFX/Music) ve `USoundMix` ayarları.

&nbsp;   \* \[ ] Placeholder seslerin (Click) eklenmesi.

\* \*\*Movement Logic\*\*

&nbsp;   \* \[ ] `ValidateMove` (Mesafe kontrolü).

&nbsp;   \* \[ ] `VInterpTo` ile yumuşak hareket (Smooth movement).

\* \*\*Testing\*\*

&nbsp;   \* \[ ] Piyon hareket ettiğinde ses duyuluyor mu?



---



\## 🧱 SPRINT 3: The Wall \& Graph Logic

\*\*Goal:\*\* Duvar koyma mekaniği ve grid veri manipülasyonu.



\* \*\*Core C++ (Wall)\*\*

&nbsp;   \* \[ ] `AQuoridorWall` actor (Object Pooling altyapısı ile).

\* \*\*Graph Manipulation\*\*

&nbsp;   \* \[ ] `TryPlaceWall` fonksiyonu.

&nbsp;   \* \[ ] Edge Blocking (Duvar konunca node bağlantısının kesilmesi).

\* \*\*Audio Feedback\*\*

&nbsp;   \* \[ ] \*\*SFX Implementation:\*\* Place Wall, Rotate Wall, Invalid Move sesleri.

&nbsp;   \* \[ ] Random Pitch Modulation (Robotik sesi engellemek için).

\* \*\*Testing\*\*

&nbsp;   \* \[ ] Duvar piyonun yolunu fiziksel ve mantıksal olarak kesiyor mu?



---



\## 🧠 SPRINT 4: The Algorithm (Validation \& Jump)

\*\*Goal:\*\* Oyun kurallarının (Pathfinding \& Jump) eksiksiz çalışması.



\* \*\*Pathfinding (Library)\*\*

&nbsp;   \* \[ ] `UQuoridorPathfinder` kütüphanesi.

&nbsp;   \* \[ ] \*\*BFS Algorithm:\*\* "Yol Var mı?" kontrolü.

&nbsp;   \* \[ ] Critical Path Validation: Yolu kapatan duvarın engellenmesi.

\* \*\*Complex Rules\*\*

&nbsp;   \* \[ ] \*\*Jump Rule:\*\* Rakip bitişikteyse üstünden atlama.

&nbsp;   \* \[ ] \*\*Diagonal Jump:\*\* Arkası doluysa çapraz gitme.

\* \*\*Testing\*\*

&nbsp;   \* \[ ] "Prisoner Test": Botu duvarlarla hapsetmeye çalış, sistemin engellemesini gör.



---



\## 🎮 SPRINT 5: AI Intelligence \& Core UI

\*\*Goal:\*\* Botların yazılması ve çok dilli arayüzün bağlanması.



\* \*\*AI Architecture\*\*

&nbsp;   \* \[ ] `AQuoridorAIController` class.

&nbsp;   \* \[ ] \*\*Medium Bot:\*\* A\* (A-Star) algoritması ile en kısa yol takibi.

\* \*\*UI Implementation\*\*

&nbsp;   \* \[ ] \*\*Dynamic Fonts:\*\* Google Noto Sans (CJK \& Arabic support) import edilmesi.

&nbsp;   \* \[ ] \*\*HUD Widget:\*\* Skor, Sıra ve Duvar sayısının String Table'dan çekilmesi.

\* \*\*Testing\*\*

&nbsp;   \* \[ ] Oyun dili Japonca/Arapça yapıldığında HUD bozuluyor mu?



---



\## 💅 SPRINT 6: UX Polish, Input \& Localization Fill

\*\*Goal:\*\* Oyun hissi (Juice) ve içerik tamamlanması.



\* \*\*Input Strategy\*\*

&nbsp;   \* \[ ] `IInputStrategy` interface.

&nbsp;   \* \[ ] \*\*Fluid Mode:\*\* Drag \& Drop, Ghost Wall, Snapping.

&nbsp;   \* \[ ] Haptic Feedback (Titreşim) entegrasyonu.

\* \*\*Content\*\*

&nbsp;   \* \[ ] \*\*Translation Import:\*\* 20 dilin `.csv` çevirilerinin içeri aktarılması.

&nbsp;   \* \[ ] \*\*RTL Support:\*\* Arapça arayüz hizalaması.

&nbsp;   \* \[ ] \*\*Music:\*\* Arka plan müziği entegrasyonu.



---



\## 🚀 SPRINT 7: Monetization, Optimization \& Release

\*\*Goal:\*\* Markete çıkış hazırlığı ve final testler.



\* \*\*Monetization\*\*

&nbsp;   \* \[ ] AdMob Plugin entegrasyonu (Interstitial Ads).

\* \*\*Optimization\*\*

&nbsp;   \* \[ ] Texture \& Audio Compression settings.

&nbsp;   \* \[ ] Code Profiling \& Tick Optimization.

\* \*\*Final QA\*\*

&nbsp;   \* \[ ] \*\*Text Overflow Check:\*\* Almanca gibi dillerde buton taşma kontrolü.

&nbsp;   \* \[ ] \*\*Automation Test:\*\* 1000 maçlık simülasyon testi.

