import unreal
import json

# Unreal Engine, 'Inputs' (Girdiler) pinini 'unreal.PythonScriptLibrary' üzerinden
# 'script_input' (betik girdisi) adında bir 'dictionary' (sözlük) olarak geçirir.
# Ancak 'wildcard' (genel) pinleri 'marshalling' (veri hazırlama) işlemi 'unreliable'dır (güvenilmez).
# 'Robust' (sağlam) yöntem, 'unreal' modülünü kullanarak doğrudan 'GameState'e erişmektir.

# Bu 'workflow' (iş akışı), 'best-practice' (en iyi uygulama) değildir.
# Bu kod, 'editor-only' (sadece-editör) bir 'debug' (hata ayıklama) aracıdır.

def check_collision(current_loc_str, target_loc_str, wall_array_json):
    try:
        # 1. 'Deserialize' (Seri Çözümle) (Blueprint 'String'lerini Python verisine dönüştür)
        # (Bu, 'IntPoint'lerin de 'String'e (Metin) çevrildiğini varsayar, örn: "4,0")
        cx, cy = map(int, current_loc_str.split(','))
        tx, ty = map(int, target_loc_str.split(','))
        
        # 'JSON String' (JSON Metni) olarak gelen 'PlacedWalls' dizisini 'deserialize' (seri çözümle) et
        placed_walls = json.loads(wall_array_json) 
        
        # 'placed_walls' artık şuna benzer bir Python 'list'idir (liste):
        # [
        #   {"GridLocation": {"X": 4, "Y": 4}, "bIsHorizontal": false, "bIsPositiveOffset": false},
        #   ...
        # ]

        # 2. 'Karnaugh' (Doğruluk Tablosu) Mantığı (Python'da)
        # (Simulink 'MATLAB Function' bloğunuza eşdeğer)
        for wall in placed_walls:
            wx = wall['GridLocation']['X']
            wy = wall['GridLocation']['Y']
            is_h = wall['bIsHorizontal']
            is_pos = wall['bIsPositiveOffset']

            # Senaryo 1: Yukarı Hareket
            if (cx == tx) and (ty == cy + 1) and (is_h == True):
                if ((wx == cx and wy == cy) and (is_pos == True)): return True  # Ayrılma engellendi
                if ((wx == tx and wy == ty) and (is_pos == False)): return True # Girme engellendi

            # Senaryo 2: Aşağı Hareket
            if (cx == tx) and (cy == ty + 1) and (is_h == True):
                if ((wx == cx and wy == cy) and (is_pos == False)): return True # Ayrılma engellendi
                if ((wx == tx and wy == ty) and (is_pos == True)): return True  # Girme engellendi
            
            # Senaryo 3: Sağa Hareket
            if (cy == ty) and (tx == cx + 1) and (is_h == False):
                if ((wx == cx and wy == cy) and (is_pos == True)): return True  # Ayrılma engellendi
                if ((wx == tx and wy == ty) and (is_pos == False)): return True # Girme engellendi

            # Senaryo 4: Sola Hareket
            if (cy == ty) and (cx == tx + 1) and (is_h == False):
                if ((wx == cx and wy == cy) and (is_pos == False)): return True # Ayrılma engellendi
                if ((wx == tx and wy == ty) and (is_pos == True)): return True  # Girme engellendi

        # 3. 'Return' (Döndür)
        return False # Döngü bitti, 'collision' (çarpışma) yok

    except Exception as e:
        # 'String' (Metin) 'parsing' (ayrıştırma) veya 'JSON' (JSON) hatası
        unreal.log_error(f"Python 'IsPathBlockedByWall' hatası: {e}")
        return True # Güvenlik için 'default' (varsayılan) olarak engelle

# 'Execute Python Script' düğümü 'script_output' adında bir değişken bekler.
# 'Inputs' (Girdiler) ve 'Outputs' (Çıktılar) pinlerinin nasıl bağlandığına bağlı olarak
# 'marshalling' (veri hazırlama) işlemi değişir.
# Bu kodun çalışması için 'Inputs' (Girdiler) pinlerini 'String' (Metin) olarak ayarlamak
# ve 'IntPoint'leri `Format Text` ile "X,Y" formatına dönüştürmek gerekir.

# script_output = check_collision(inputs['CurrentLocation'], inputs['TargetLocation'], inputs['WallsJSON'])