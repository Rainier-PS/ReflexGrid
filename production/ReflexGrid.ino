const int BUTTON_UP_PIN    = 26;
const int BUTTON_DOWN_PIN  = 28;
const int BUTTON_LEFT_PIN  = 4;
const int BUTTON_RIGHT_PIN = 3;

const int LED_PINS[2][2] = {
  {29, 27},
  {7,  6}
};

int playerX = 0;
int playerY = 0;
int dangerX = 1;
int dangerY = 1;

bool dangerActive = false;
bool gameOver = false;

unsigned long dangerShownTime = 0;
unsigned long lastDangerSpawnTime = 0;
unsigned long reactionTime = 2000;

unsigned long lastMoveTime = 0;
unsigned long lastBlink = 0;
bool dangerLEDState = false;

int score = 0;

const unsigned long moveCooldown = 150;
const unsigned long blinkInterval = 200;
const unsigned long dangerLockDuration = 200;

void setup() {
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      pinMode(LED_PINS[y][x], OUTPUT);
      digitalWrite(LED_PINS[y][x], LOW);
    }
  }

  randomSeed(analogRead(0));
  drawPlayer();
  delay(1000);
}

void loop() {
  if (gameOver) {
    showPlayerCaught();
    flashAll();
    showFinalScore();
    resetGame();
    return;
  }

  handleInput();
  drawPlayer();

  if (!dangerActive) {
    spawnDanger();
    dangerShownTime = millis();
    lastDangerSpawnTime = millis();
    dangerLEDState = false;
    lastBlink = millis();
    dangerActive = true;
  }

  blinkDangerLED();

  if (dangerActive && millis() - dangerShownTime > reactionTime) {
    if (playerX == dangerX && playerY == dangerY) {
      gameOver = true;
    } else {
      dangerActive = false;
      reactionTime = max(reactionTime - 100, 500UL);
      score++;
      showSafeDodge();
    }
  }

  delay(10);
}

void handleInput() {
  if (millis() - lastMoveTime < moveCooldown) return;
  if (millis() - lastDangerSpawnTime < dangerLockDuration) return;

  if (digitalRead(BUTTON_UP_PIN) == LOW && playerY > 0) {
    playerY--; lastMoveTime = millis();
  } else if (digitalRead(BUTTON_DOWN_PIN) == LOW && playerY < 1) {
    playerY++; lastMoveTime = millis();
  } else if (digitalRead(BUTTON_LEFT_PIN) == LOW && playerX > 0) {
    playerX--; lastMoveTime = millis();
  } else if (digitalRead(BUTTON_RIGHT_PIN) == LOW && playerX < 1) {
    playerX++; lastMoveTime = millis();
  }
}

void drawPlayer() {
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      digitalWrite(LED_PINS[y][x], LOW);
    }
  }

  digitalWrite(LED_PINS[playerY][playerX], HIGH);
}

void spawnDanger() {
  do {
    dangerX = random(0, 2);
    dangerY = random(0, 2);
  } while (dangerX == playerX && dangerY == playerY);
}

void blinkDangerLED() {
  if (!dangerActive) return;

  if (millis() - lastBlink > blinkInterval) {
    dangerLEDState = !dangerLEDState;
    lastBlink = millis();
    digitalWrite(LED_PINS[dangerY][dangerX], dangerLEDState ? HIGH : LOW);
  }
}

void showSafeDodge() {
  digitalWrite(LED_PINS[playerY][playerX], LOW);
  delay(100);
  digitalWrite(LED_PINS[playerY][playerX], HIGH);
}

void showPlayerCaught() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PINS[playerY][playerX], HIGH);
    delay(100);
    digitalWrite(LED_PINS[playerY][playerX], LOW);
    delay(100);
  }
}

void flashAll() {
  for (int i = 0; i < 4; i++) {
    setAllLEDs(HIGH);
    delay(200);
    setAllLEDs(LOW);
    delay(200);
  }
}

void setAllLEDs(bool state) {
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      digitalWrite(LED_PINS[y][x], state);
    }
  }
}

void showFinalScore() {
  delay(500);

  int blinkCount = min(score, 10);
  for (int i = 0; i < blinkCount; i++) {
    setAllLEDs(HIGH);
    delay(150);
    setAllLEDs(LOW);
    delay(150);
  }
}

void resetGame() {
  gameOver = false;
  dangerActive = false;
  playerX = 0;
  playerY = 0;
  reactionTime = 2000;
  score = 0;
  drawPlayer();
  delay(1000);
}
