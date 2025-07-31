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

unsigned long dangerShownTime = 0;
unsigned long reactionTime = 2000;
bool dangerActive = false;
bool gameOver = false;

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
    flashAll();
    resetGame();
    return;
  }

  handleInput();
  drawPlayer();

  if (!dangerActive) {
    spawnDanger();
    dangerShownTime = millis();
    dangerActive = true;
  }

  if ((millis() / 200) % 2 == 0) {
    digitalWrite(LED_PINS[dangerY][dangerX], HIGH);
  } else {
    if (!(playerX == dangerX && playerY == dangerY))
      digitalWrite(LED_PINS[dangerY][dangerX], LOW);
  }

  if (dangerActive && (millis() - dangerShownTime > reactionTime)) {
    if (playerX == dangerX && playerY == dangerY) {
      gameOver = true;
    } else {
      dangerActive = false;
      reactionTime = (reactionTime - 100 > 500) ? reactionTime - 100 : 500;
    }
  }

  delay(30);
}

void handleInput() {
  if (digitalRead(BUTTON_UP_PIN) == LOW && playerY > 0) {
    playerY--;
    delay(150);
  }
  if (digitalRead(BUTTON_DOWN_PIN) == LOW && playerY < 1) {
    playerY++;
    delay(150);
  }
  if (digitalRead(BUTTON_LEFT_PIN) == LOW && playerX > 0) {
    playerX--;
    delay(150);
  }
  if (digitalRead(BUTTON_RIGHT_PIN) == LOW && playerX < 1) {
    playerX++;
    delay(150);
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

void flashAll() {
  for (int i = 0; i < 4; i++) {
    for (int y = 0; y < 2; y++) {
      for (int x = 0; x < 2; x++) {
        digitalWrite(LED_PINS[y][x], HIGH);
      }
    }
    delay(200);
    for (int y = 0; y < 2; y++) {
      for (int x = 0; x < 2; x++) {
        digitalWrite(LED_PINS[y][x], LOW);
      }
    }
    delay(200);
  }
}

void resetGame() {
  gameOver = false;
  dangerActive = false;
  playerX = 0;
  playerY = 0;
  reactionTime = 2000;
  drawPlayer();
  delay(1000);
} 