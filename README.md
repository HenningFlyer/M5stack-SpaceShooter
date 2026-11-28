# M5Stack Fire SpaceShooter

Arcade Space Shooter for **M5Stack Fire** (classic ESP32 / ESP32-D0WDQ6).
Source: Tyler Edwards (Hackerboxes #0020), M5Stack port by macsbug, Fire/M5Unified adaptation.

## Known-working baseline

The initial source under [SpaceShooter/SpaceShooter.ino](SpaceShooter/SpaceShooter.ino) is the version previously confirmed to run without crashes on the physical M5Stack Fire. GitHub Actions now builds the physical FIRE and a separate generic ESP32 simulation firmware.

Controls: **A = left**, **B = right**, **C = start/fire** (as implemented in source).

## Cloud compilation

Open **Actions > ESP32 firmware build**, select the latest run and download the `SpaceShooter-M5Stack-Fire` artifact. This contains the firmware application `.bin` and symbols `.elf` when compilation succeeds.

Build settings:
- Espressif ESP32 board core **3.3.4**
- Board **esp32:esp32:m5stack-fire**
- **PSRAM=disabled**
- **M5Unified 0.2.11**
- **M5GFX 0.2.17**

You can also launch the build with **Actions > ESP32 firmware build > Run workflow**. The uploaded application `.bin` is intended for an application-aware loader such as M5Launcher, **not** for flashing at address zero as a complete ESP32 flash image.

## Play in the Wokwi browser

1. Open https://wokwi.com/projects/new/esp32 (Arduino ESP32).
2. Replace the editor sketch with the contents of [SpaceShooter.ino](SpaceShooter/SpaceShooter.ino).
3. Replace the editor's diagram.json with [diagram.json](diagram.json).
4. Add the libraries listed in [libraries.txt](libraries.txt) using Library Manager (or paste the file if available).
5. Start simulation, click the simulation area to focus it, then press C to start. A moves left, B moves right, C fires. The on-screen buttons can also be held with the mouse.
6. Stop and restart to reset the game. Save the project to your Wokwi account to obtain a shareable project link.

The browser uses the generic ESP32 Arduino board (ARDUINO_ESP32_DEV), not the physical FIRE target. The TFT part is rotated 90 degrees for landscape presentation; the firmware retains its existing logical 320x240 drawing coordinates.

For the precompiled version, download the SpaceShooter-Wokwi-interactive artifact from the latest successful [Actions run](https://github.com/HenningFlyer/M5stack-SpaceShooter/actions), extract it and open the folder with Wokwi for VS Code. The configuration points to the separate 4 MB merged firmware, not the FIRE binary.

The previous CI run 46120813686 verified boot, active-low GPIO buttons and different non-uniform screenshots. The new browser layout must still be checked visually. The screenshot capture may retain native panel orientation even when the browser part is rotated. RGB LEDs, speaker and other FIRE hardware are not simulated.

## Warnings

- Repository visibility on creation: **public**. Change to private in Settings if desired.
- This code preserves the existing gameplay and debug messages.
- Do not overwrite M5Launcher with a raw firmware image unintentionally.
