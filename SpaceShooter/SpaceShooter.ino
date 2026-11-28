//======================== intro =======================================
//      Space Shooter, basically knock-off Space Invaders
//             and also maybe a bit of Galaga
//   Written by Tyler Edwards for the Hackerbox #0020 badge
//  But should work on any ESP32 and Adafruit ILI9341 display
//  Sorry for the messy code, you'll just have to live with it
//      Tyler on GitHub: https://github.com/HailTheBDFL/
//          Hackerboxes: http://hackerboxes.com/
//=========================== setup ===================================
// Space Shooter with M5STACK : 2018.01.12 Transplant by macsbug
// Controller   : Buttons A = LEFT, B = RIGHT, C = START or SHOOTING
// Github:https://macsbug.wordpress.com/2018/01/12/esp32-spaceshooter-with-m5stack/
// Anpassung an M5Stack FIRE 22.11.2026 Henning Stöcklein
//===================================================================
// The simulated ESP32 uses an independent Adafruit display driver.
#if defined(ARDUINO_ESP32_DEV)
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
Adafruit_ILI9341 simDisplay(14, 27, 33);
#define GAME_LCD simDisplay
#define BLACK ILI9341_BLACK
#else
#include <M5Unified.h>
#define GAME_LCD M5.Lcd
#endif

#define BUTTON_A_PIN 39
#define BUTTON_B_PIN 38
#define BUTTON_C_PIN 37

//============================= game variables =========================
unsigned long offsetM = 0;
unsigned long offsetT = 0;
unsigned long offsetF = 0;
unsigned long offsetB = 0;
unsigned long offsetA = 0;
unsigned long offsetAF = 0;
unsigned long offsetAB = 0;
unsigned long offsetS = 0;
int threshold = 40;

boolean startPrinted = false;
boolean beginGame = false;
boolean beginGame2 = true;
boolean play = false;

int score = 0;
int scoreInc = 10;
int level = 1;

//---------------------Player---------------------------------------
int shipX = 147;
int shipY = 190;
int oldShipX = 0;
int oldShipY = 0;
int changeShipX = 0;
int changeShipY = 0;
int shipSpeed = 50;
boolean doSplode = false;
boolean fire = false;
int fFireX[5] = {0, 0, 0, 0, 0};
int fFireY[5] = {0, 0, 0, 0, 0};
int fFireAge[5] = {0, 0, 0, 0, 0};

//--------------------------Aliens----------------------------------
boolean alienLive[18];
int alienLiveCount = 18;
int alienX = 7;
int alienY = 25;
int oldAlienX = 7;
int oldAlienY = 25;
int changeAlienX = 6;
int changeAlienY = 0;
int alienSpeed = 200;
int oldAlienSpeed;
int aFireX[5];
int aFireY[5];
boolean aFireAge[5];
int chanceOfFire = 2;

//================================ bitmaps ========================
//your starship
const int shipImgW = 14;
const int shipImgH = 16;
char shipImg[] = "ZZZZZZWWZZZZZZZZZZZYWWYZZZZZZZZZZWWWWZZZZZZZZZZ"
 "WWWWZZZZZZZZZWWWWWWZZZZZZZZWWWWWWZZZZZYZZWWWWWWZZYZZYZZWWWWWWZZ"
 "YZWWZZWWBBWWZZWWWWZZWBBBBWZZWWWWZWWBBBBWWZWWWWZWWWWWWWWZWWWWWWW"
 "WWWWWWWWWWRWWWWWWWWWWRWZZWWWWWWWWWWZZZZZWRRWWRRWZZZ";

//flames
const int flamesImgW = 12;
const int flamesImgH = 6;
char flamesImg[] = "RZZZZZZZZZZRRZRYYRRYYRZRRZRYYRRYYRZRZZRYRZZRY"
 "RZZZZZRZZZZRZZZZZZRZZZZRZZZ";

//alien
const int alienImgW = 14;
const int alienImgH = 11;
char alienImg[] = "GGGZZZZZZZZGGGZZZGZZZZZZGZZZZZGGGGGGGGGGZZZGGG"
 "GGGGGGGGGZGGGZGGGGGGZGGGGGGZZGGGGZZGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
 "GGGGZZZGGZZGGZZZGZGGZGGZZGGZGGZZZZZZGZZGZZZZZ";

//ship 'sploded
const int splodedImgW = 14;
const int splodedImgH = 16;
char splodedImg[] = "ZZZZZZWWZZZZZZZZZZRYWWYRZZZYZZZRRWWRRRRZRWYZ"
 "RRRRRYYRRRZWYZRYRYYRYYRRRZWWRYYYRYYYYYRZWWRYYRYRYYYYRRWWRYYYRWR"
 "YBRRZRRRYRRWWWRYRWZZRYYRRBBWRYRWWZZRYYBBBRRYBWWRZZRYYYRRYYZZWZR"
 "RWRYYRBYRZZWZZRYBRYYYYYRRZZRWWYYYWWRRRZZZZWRRWWRRRWZZZ";

//=============================== setup and loop ==================
void setup() {
  Serial.begin(115200) ;
  delay (300) ;

Serial.println("DEBUG 1: vor memset");

  memset(alienLive, 1, sizeof(alienLive));
  memset(aFireX, 0, sizeof(aFireX));
  memset(aFireY, 0, sizeof(aFireY));
  memset(aFireAge, 0, sizeof(aFireAge));

Serial.println("DEBUG 2: vor m5.begin");

 #if defined(ARDUINO_ESP32_DEV)
  SPI.begin(18, 19, 23, 14);
  simDisplay.begin(27000000);
  pinMode(32, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
#else
  M5.begin();
#endif
  GAME_LCD.setRotation(1);
  GAME_LCD.fillScreen(ILI9341_BLACK);
  GAME_LCD.setTextColor(0x5E85);
  GAME_LCD.setTextSize(4);

Serial.println("DEBUG 3: vor randomseed");
  randomSeed(esp_random());  // GPIO6 ist mit dem SPI-Flash verbunden
//  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
//  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
//  pinMode(BUTTON_C_PIN, INPUT_PULLUP);
}

//==================================================================
void loop() 
{
Serial.println("DEBUG 4: vor m5.update");

#if !defined(ARDUINO_ESP32_DEV)
   M5.update();
#endif

#if defined(ARDUINO_ESP32_DEV)
  if (!digitalRead(32)) { left(); }
  if (!digitalRead(25)) { right(); }
  if (!digitalRead(26)) { select(); }
#else
  if(M5.BtnA.isPressed()) { left  () ;}
  if(M5.BtnB.isPressed()) { right () ;}
  if(M5.BtnC.isPressed()) { select() ;}
#endif

  //-------------Start Screen--------------
  if (millis() - offsetS >= 900 and !beginGame) {
    if (!startPrinted) {
      GAME_LCD.setCursor(77, 105);
      GAME_LCD.print(">START<");
      startPrinted = true;
      offsetS = millis();
    }
    else {
      GAME_LCD.fillRect(77, 105, 244, 32, ILI9341_BLACK);
      startPrinted = false;
      offsetS = millis();
    }
  }
  if (beginGame and beginGame2) {
    GAME_LCD.fillRect(77, 105, 244, 32, ILI9341_BLACK);
    beginGame2 = false;
    play = true;
  }

  //-------------Player-----------------------------------------------
  if (millis() - offsetM >= shipSpeed and play) {
    moveShip();
    offsetM = millis();
  }

  if (oldShipX != shipX or oldShipY != shipY) {
    GAME_LCD.fillRect(oldShipX, oldShipY, 28, 44, ILI9341_BLACK);
    oldShipX = shipX;
    oldShipY = shipY;
    drawBitmap(shipImg, shipImgW, shipImgH, shipX, shipY, 2);
  }
  if (fire and play) { fireDaLazer();}
  if (millis() - offsetB >= 50) {
   for (int i = 0; i < 5; i++) {
    if (fFireAge[i] < 20 and fFireAge[i] > 0){keepFirinDaLazer(i);}
    if (fFireAge[i] == 20) { stopFirinDaLazer(i);}
   }
   offsetB = millis();
  }  
  if (millis() - offsetT > 50) {
    changeShipX = 0;
    changeShipY = 0;
  }

  //---------------Aliens--------------------------------------------
  if (millis() - offsetA >= alienSpeed and play) {
    moveAliens(); offsetA = millis();
  }
  if (findAlienX(5) >= 294){changeAlienX = -3;changeAlienY = 7;}
  if (alienX <= 6){changeAlienX = 3; changeAlienY = 7;}

  alienLiveCount = 0;
  for (int i = 0; i < 18; i++) {
   if (alienLive[i]) {
    alienLiveCount += 1;
     if (alienShot(i)) {
      GAME_LCD.fillRect(findOldAlienX(i),findOldAlienY(i),28,22,BLACK);
      alienLiveCount -= 1;
      alienLive[i] = false;
      score += scoreInc;
     }
     if (onPlayer(i) or exceedBoundary(i)) {
      gameOver();
    }
   }
  }
  if (alienLiveCount == 1) {
    oldAlienSpeed = alienSpeed;
    if (alienSpeed > 50) {
      alienSpeed -= 10;
    }
    else {
      alienSpeed = 20;
    }
  }
  if (alienLiveCount == 0) {
    levelUp();
  }
#if !defined(ARDUINO_ESP32_DEV)
  M5.update();
#endif
  delay(1);
}

// functions =======================================================
void gameOver() {
  play = false;
  if (doSplode) {
    drawBitmap(splodedImg,splodedImgW,splodedImgH,shipX,shipY,2);
  }
  GAME_LCD.fillScreen(ILI9341_BLACK);
  drawScore(false);
  delay(1000);
  GAME_LCD.setCursor(17, 168);
  GAME_LCD.setTextSize(2);
  GAME_LCD.print("(Reset device to replay)");
  while (1) { delay(100); }
}

//==================================================================
void drawScore(boolean win) {
  GAME_LCD.setCursor(53, 40);
  GAME_LCD.setTextColor(ILI9341_WHITE);
  GAME_LCD.setTextSize(4);
  if (win) {
    GAME_LCD.print("LEVEL UP!");
  }
  else {
    GAME_LCD.print("GAME OVER");
  }
  delay(1000);
  GAME_LCD.setCursor(59, 89);
  GAME_LCD.setTextSize(3);
  GAME_LCD.print("Score: "); GAME_LCD.print(score);
  offsetM = millis();
  delay(1000);
  GAME_LCD.setCursor(71, 128);
  GAME_LCD.print("Level: "); GAME_LCD.print(level);
}

//==================================================================
void levelUp() {
  play = false;
  memset(alienLive, 1, sizeof(alienLive));
  memset(aFireX, 0, sizeof(aFireX));
  memset(aFireY, 0, sizeof(aFireY));
  memset(aFireAge, 0, sizeof(aFireAge));
  alienX = 7;
  alienY = 25;
  oldAlienX = 7;
  oldAlienY = 25;
  alienSpeed = oldAlienSpeed;
  if (alienSpeed > 100) {
    alienSpeed -= 10; chanceOfFire -= 10;
  }
  else if (alienSpeed > 50) {
    alienSpeed -= 10; chanceOfFire -=5;
  }
  else if (alienSpeed > 25) {
    alienSpeed -= 5; chanceOfFire -=1;
  }
  score += 50;     scoreInc += 5;
  changeShipX = 0; changeShipY = 0; 

  for (unsigned long i = millis(); millis() - i <= 1600;) {
    if (millis() - offsetM >= 20) {
      GAME_LCD.fillRect(oldShipX, oldShipY, 28, 44, ILI9341_BLACK);
      drawBitmap(shipImg,shipImgW,shipImgH,shipX,shipY,2);
      drawBitmap(flamesImg,flamesImgW,flamesImgH,shipX + 1,
                 shipY + 32,2);
      oldShipX = shipX; oldShipY = shipY;
      shipY -= 6;
      offsetM = millis();
    }
    delay(1);
  }
  drawScore(true);
  level += 1;
  shipX = 147;
  shipY = 190;
  delay(4000);
  GAME_LCD.fillScreen(ILI9341_BLACK);
  offsetM = millis();
  play = true;
}

//==================================================================
boolean alienShot(int num) {
  for (int i = 0; i < 5; i++) {
    if (fFireAge[i] < 20 and fFireAge[i] > 0) {
      if (fFireX[i] > findAlienX(num) - 4 and fFireX[i] < 
         findAlienX(num) + 28 and fFireY[i] < findAlienY(num) + 
         22 and fFireY[i] > findAlienY(num) + 4) {
        fFireAge[i] = 20;
        return true;
      }
    }
  }
  return false;
}

//==================================================================
boolean onPlayer(int num) {
  if (findAlienX(num) - shipX < 24 and findAlienX(num) - 
      shipX > -28 and findAlienY(num) - shipY < 32 and 
      findAlienY(num) - shipY > -22) {
    doSplode = true;
    return true;
  } else { return false;}
}

//==================================================================
boolean exceedBoundary(int num) {
  if (findAlienY(num) > 218) { return true;
  } else { return false;
  }
}

//==================================================================
void moveAliens() {
  for (int i = 0; i < 18; i++) {
   if (alienLive[i]) {
    GAME_LCD.fillRect(findOldAlienX(i),findOldAlienY(i),28,22,BLACK);
    drawBitmap(alienImg,alienImgW,alienImgH,findAlienX(i),
               findAlienY(i),2);
   }
  }
  oldAlienX = alienX; oldAlienY = alienY;
  alienX += changeAlienX; alienY += changeAlienY;
  if (changeAlienY != 0) { changeAlienY = 0; }
}

//==================================================================
int findAlienX   (int num) { return alienX + 42*(num % 6); }
//==================================================================
int findAlienY   (int num) { return alienY + 33*(num / 6); }
//==================================================================
int findOldAlienX(int num) { return oldAlienX + 42*(num % 6); }
//==================================================================
int findOldAlienY(int num) { return oldAlienY + 33*(num / 6); }

//---------------------------Player---------------------------------
void fireDaLazer() {
  int bulletNo = -1;
  for (int i = 0; i < 4; i++) {
    if (fFireAge[i] == 0) { bulletNo = i;}
  }
  if (bulletNo != -1) {
   fFireAge[bulletNo] = 1;
   fFireX[bulletNo] = shipX + 13;
   fFireY[bulletNo] = shipY - 4;
   GAME_LCD.fillRect(fFireX[bulletNo],fFireY[bulletNo],4,3,MAGENTA);
  }
  fire = false;
}

//==================================================================
void keepFirinDaLazer(int bulletNo) {
  GAME_LCD.fillRect(fFireX[bulletNo],fFireY[bulletNo],4,4,BLACK);
  fFireY[bulletNo] -= 8;
  GAME_LCD.fillRect(fFireX[bulletNo],fFireY[bulletNo],4,4,MAGENTA);
  fFireAge[bulletNo] += 1;
}
//==================================================================
void stopFirinDaLazer(int bulletNo) {
  GAME_LCD.fillRect(fFireX[bulletNo],fFireY[bulletNo],4,4,BLACK);
  fFireAge[bulletNo] = 0;
}

//==================================================================
void moveShip() {
  if (shipX + changeShipX < 288 and shipX + changeShipX > 
      6 and changeShipX != 0){
    shipX += changeShipX;
  }
  if (shipY + changeShipY > 24 and shipY + changeShipY < 
      192 and changeShipY != 0){
    shipY += changeShipY;
  }
  if (oldShipX != shipX or oldShipY != shipY) {
    GAME_LCD.fillRect(oldShipX, oldShipY, 28, 44, ILI9341_BLACK);
    oldShipX = shipX; oldShipY = shipY;
    drawBitmap(shipImg, shipImgW, shipImgH, shipX, shipY, 2);
  }
}

//==================================================================
void drawBitmap(char img[],int imgW,int imgH,int x,int y,int scale){
  uint16_t cellColor;
  char curPix;
  for (int i = 0; i < imgW*imgH; i++) {
    curPix = img[i];
    if (curPix == 'W') {      cellColor = ILI9341_WHITE; }
    else if (curPix == 'Y') { cellColor = ILI9341_YELLOW; }
    else if (curPix == 'B') { cellColor = ILI9341_BLUE; }
    else if (curPix == 'R') { cellColor = ILI9341_RED; }
    else if (curPix == 'G') { cellColor = 0x5E85; }
    else { continue; } // Transparent or unsupported pixel: do not draw.
    if (curPix != 'Z' and scale == 1) {
      GAME_LCD.drawPixel(x + i % imgW, y + i / imgW, cellColor);
    }
    else if (curPix != 'Z' and scale > 1) {
      GAME_LCD.fillRect(x + scale*(i%imgW),y + 
         scale*(i/imgW),scale,scale,cellColor);
    }
  }
}

//=========================== button functions =====================
void up() {
  if (millis() - offsetT >= 50 and play) {
    changeShipX = 0; changeShipY = -6; offsetT = millis();
  }
}
//==================================================================
void down() {
  if (millis() - offsetT >= 50 and play) {
    changeShipX = 0; changeShipY = 6; offsetT = millis();
  }
}
//==================================================================
void left() {
  if (millis() - offsetT >= 50 and play) {
    changeShipX = -6; changeShipY = 0; offsetT = millis();
  }
}
//==================================================================
void right() {
  if (millis() - offsetT >= 50 and play) {
    changeShipX = 6; changeShipY = 0; offsetT = millis();
  }
}
//==================================================================
void select() {
  if (millis() - offsetF >= 500 and play) {
    fire = true; offsetF = millis();
  }
  if (!beginGame) { beginGame = true;}
}
//==================================================================