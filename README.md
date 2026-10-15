# 🗡️ Pocket Dungeon: Rogue Dice Crawler (ESP32 CYD)

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange?logo=platformio)](https://platformio.org/)
[![Hardware](https://img.shields.io/badge/ESP32-2432S028_(CYD)-blue?logo=espressif)](https://www.espressif.com/)
[![Graphics](https://img.shields.io/badge/Driver-LovyanGFX-green)](https://github.com/lovyan03/LovyanGFX)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Juego táctico de mazmorras y colocación de dados inspirado en el galardonado juego de mesa **One Deck Dungeon**, desarrollado íntegramente en C++ para la placa **ESP32-2432S028 ("Cheap Yellow Display")** con pantalla táctil vertical de 2.8" (240x320) y zumbador piezoeléctrico.

---

## 🎮 Mecánicas y Flujo de Juego

### 1. Elige tu Héroe
- **⚔️ Guerrero (12 HP):** Especialista en combate cuerpo a cuerpo (3 dados de Fuerza 🟡, 1 Agilidad 🟢, 1 Magia 🔵). Su poder *Golpe de Escudo* potencia sus dados amarillos.
- **🏹 Pícara (10 HP):** Ágil y sigilosa (1 Fuerza 🟡, 3 Agilidad 🟢, 1 Magia 🔵). Su poder *Sigilo* transmuta dados verdes en dados comodín (Heroicos ⚫).
- **🔮 Maga (8 HP):** Dominio de las artes arcanas (1 Fuerza 🟡, 1 Agilidad 🟢, 3 Magia 🔵). Su poder *Transmutación* convierte su peor dado en un comodín de valor 6.

### 2. Exploración de Mazmorras (Pisos 1, 2 y Jefe en Piso 3)
- Elige entre dos puertas misteriosas en cada turno.
- Descubre monstruos (Ratas gigantes, Esqueletos, Orcos Berserker, Nigromantes) o trampas letales (Pozos de pinchos, Trampas de flechas, Llamas arcanas).

### 3. Combate por Colocación de Dados
1. Pulsa **`TIRAR DADOS`** para lanzar tu reserva según tus atributos.
2. **Colocación táctica con Stylus:**
   - Toca un dado disponible en tu bandeja (se iluminará en blanco/dorado).
   - Toca una casilla de la carta de monstruo que coincida en color y tenga un valor igual o menor que tu dado.
   - Si te equivocas, ¡toca la casilla para devolver el dado a tu bandeja!
   - Usa el botón **`AUTO`** para una asignación óptima inteligente de un solo toque.
3. Pulsa **`RESOLVER`** para cerrar la ronda:
   - Las casillas que no hayas logrado tapar te infligen daño directo al corazón (💔).

### 4. Botín Multifunción (Looting)
Al derrotar a un enemigo, eliges una de tres recompensas:
- **⚔️ Objeto (+1 Dado permanente):** Suma un dado del atributo del monstruo a tu reserva para futuros combates.
- **🧪 Poción de Salud:** Recupera corazones inmediatamente y rellena tu frasco.
- **⭐ Experiencia (XP):** Sube de nivel, aumentando tu vida máxima y desbloqueando dados heroicos comodines.

### 5. Jefe Final: El Dragón Ancestral
En el Piso 3 te aguarda el temible Dragón Rojo con 5 casillas de alta exigencia. ¡Derrótalo para proclamarte vencedor de la mazmorra!

---

## 🛠️ Conexiones Hardware (ESP32-2432S028 CYD)

| Periférico | Pin ESP32 | Descripción |
|---|---|---|
| **TFT SPI** | GPIO 12 (MISO), 13 (MOSI), 14 (SCK), 15 (CS), 2 (DC), 21 (BL) | Pantalla 240x320 |
| **Touch XPT2046** | GPIO 32 (MOSI), 39 (MISO), 25 (CLK), 33 (CS), 36 (IRQ) | Táctil resistivo |
| **Buzzer** | GPIO 26 | Efectos de dados y combate |
| **LED RGB** | GPIO 4 (R), 16 (G), 17 (B) | Alertas de peligro y éxito |

---

## 🚀 Compilación y Carga por USB

```bash
# 1. Entrar en la carpeta
cd esp32pocketdungeon

# 2. Compilar
pio run

# 3. Flashear a la placa
pio run -t upload --upload-port COM6
```
