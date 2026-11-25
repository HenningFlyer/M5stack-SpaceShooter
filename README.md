# M5Stack Fire SpaceShooter

Arcade Space Shooter for **M5Stack Fire** (classic ESP32 / ESP32-D0WDQ6).
Source: Tyler Edwards (Hackerboxes #0020), M5Stack port by macsbug, Fire/M5Unified adaptation.

## Known-working baseline

The initial source under [SpaceShooter/SpaceShooter.ino](SpaceShooter/SpaceShooter.ino) is the version previously confirmed to run without crashes on the physical M5Stack Fire. It is **not yet verified** in GitHub Actions or a hardware simulator.

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

## Simulation status

The M5Stack Fire is **not** fully simulated yet. Wokwi can emulate generic ESP32 firmware and supported ILI9341 display parts, but M5Unified's board autodetection, display pins, internal power controller and button mapping must all be validated. A successful compile is **not** proof that the game will run in a virtual Fire or on physical hardware.

### Next checks

1. Verify GitHub Actions build completes and inspect BIN/ELF output.
2. Verify ESP32/M5Stack Fire hardware and SPI display pins before adding a Wokwi diagram.
3. Add a minimal boot/display test and an explicit simulation adaptation if necessary.
4. Add repeatable gameplay checks (buttons, shots, collision, Game Over, reset).

## Warnings

- Repository visibility on creation: **public**. Change to private in Settings if desired.
- This code preserves the existing gameplay and debug messages.
- Do not overwrite M5Launcher with a raw firmware image unintentionally.
