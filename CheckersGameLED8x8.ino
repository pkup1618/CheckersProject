#include <FastLED.h>
#define NUM_LEDS 64
#define DOUT_PIN 9

#define EMPTY 0
#define RED_FIGURE 1
#define BLUE_FIGURE 2
#define SIGHN 3
#define UNPLAYABLE 4
#define RED_QUEEN 5
#define BLUE_QUEEN 6

#define KEY_RIGHT 2
#define KEY_LEFT 4
#define KEY_DOWN 3
#define KEY_UP 5
#define KEY_OK 6

byte board[8][8];
byte sighns[8][8];
CRGB leds[NUM_LEDS];

byte cursorX = 0;
byte cursorY = 0;

bool choosenFigureMarker = false;
byte choosenFigureX = 255;
byte choosenFigureY = 255;
byte choosenFigureType = 255;


void setup() {
    FastLED.addLeds<NEOPIXEL, DOUT_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(1);

    pinMode(KEY_LEFT, INPUT_PULLUP);
    pinMode(KEY_RIGHT, INPUT_PULLUP);
    pinMode(KEY_DOWN, INPUT_PULLUP);
    pinMode(KEY_UP, INPUT_PULLUP);
    pinMode(KEY_OK, INPUT_PULLUP);

    initBoard();
    showBoard();
}

void loop() {
    for (byte i = 2; i < 7; i++) {
        if (!digitalRead(i)) {
            switch(i) {
                case KEY_RIGHT: {
                    moveRight();
                    break;
                }
                case KEY_LEFT: {
                    moveLeft();
                    break;
                }
                case KEY_DOWN: {
                    moveDown();
                    break;
                }
                case KEY_UP: {
                    moveUp();
                    break;
                }
                case KEY_OK: {
                    keyOK();
                    break;
                }
                default: {                  
                    break;
                }
            }
            delay(200);
        }
    }
    showBoard();
}

void initBoard() {
    for (byte y = 0; y < 8; y++) {
        if (y % 2 == 1) {
            for (byte x = 0; x < 8; x+=2) {
                board[x][y] = UNPLAYABLE;
            }
        } else {
            for (byte x = 1; x < 8; x+=2) {
                board[x][y] = UNPLAYABLE;
            }
        }
    }

    for (byte y = 0; y < 3; y++) {
        if (y % 2 == 1) {
            for (byte x = 1; x < 8; x+=2) {
                board[x][y] = RED_FIGURE;
            }
        } else {
            for (byte x = 0; x < 8; x+=2) {
                board[x][y] = RED_FIGURE;
            }
        }
    }

    for (byte y = 5; y < 8; y++) {
        if (y % 2 == 1) {
            for (byte x = 1; x < 8; x+=2) {
                board[x][y] = BLUE_FIGURE;
            }
        } else {
            for (byte x = 0; x < 8; x+=2) {
                board[x][y] = BLUE_FIGURE;
            }
        }
    }

    board[3][3] = BLUE_FIGURE;
}

void showBoard() {
    for (byte i = 0; i < 8; i++) {
        for (byte j = 0; j < 8; j++) {

            CRGB color;

            switch(board[i][j]) {
                case EMPTY: {
                    color = CRGB::Black;
                    break;
                }
                case RED_FIGURE: {
                    color = CRGB::Red;
                    break;
                }
                case BLUE_FIGURE: {
                    color = CRGB::Blue;
                    break;
                }
                case UNPLAYABLE: {
                    color = CRGB::White;
                    break;
                }
                case RED_QUEEN: {
                    color = CRGB::Orange;
                    break;
                }
                case BLUE_QUEEN: {
                    color = CRGB::Purple;
                    break;
                }
                default: {
                    break;
                }
            }

            switch(sighns[i][j]) {
                case EMPTY: {
                    break;
                }
                case SIGHN: {
                    color = CRGB::Yellow;
                    break;
                }
            }

            processCellLight(i, j, color);

        }

        if (choosenFigureMarker) {
              processCellLight(choosenFigureX, choosenFigureY, CRGB::Purple);
        }

        processCellLight(cursorX, cursorY, CRGB::Green);

    }

    FastLED.show();
}

void processCellLight(byte i, byte j, CRGB color) {
    if (j % 2 == 0) {
        leds[(j * 8) + i] = color;
    } else {
        leds[(j * 8) + 7 - i] = color;
    }    
}


void moveRight() {
    if (cursorX < 7) {
        cursorX++;
    }
}

void moveLeft() {

    if (cursorX > 0) {
        cursorX--;
    }
}

void moveUp() {
    if (cursorY < 7) {
        cursorY++;
    }
}

void moveDown() {

    if (cursorY > 0) {
        cursorY--;
    }
}

void keyOK() {
    byte cell = board[cursorX][cursorY];

    if (cell == EMPTY) {
        if (sighns[cursorX][cursorY] == SIGHN) {
            makeMovement(cursorX, cursorY);
        } 
    }

    if (cell != EMPTY && cell != UNPLAYABLE) {
        chooseFigure(cursorX, cursorY);
    }
}

void chooseFigure(byte x, byte y) {
    if (choosenFigureX == x && choosenFigureY == y) {
        choosenFigureMarker = false;
        hideSighns();
        choosenFigureX = 255;
        choosenFigureY = 255;
    } else {
      choosenFigureMarker = true;
      choosenFigureX = x;
      choosenFigureY = y;
      choosenFigureType = board[x][y];
      hideSighns();
      showSighns();
    }
}

void showSighns() {
    switch (choosenFigureType) {
        case RED_FIGURE: {
            switch (board[choosenFigureX + 1][choosenFigureY + 1]) {
                case EMPTY: {
                    sighns[choosenFigureX + 1][choosenFigureY + 1] = SIGHN;
                    break;
                }
                case BLUE_FIGURE:
                case BLUE_QUEEN: {
                    if (board[choosenFigureX + 2][choosenFigureY + 2] == EMPTY) {
                        sighns[choosenFigureX + 2][choosenFigureY + 2] = SIGHN;
                    }
                    break;
                }
            } 
            switch (board[choosenFigureX - 1][choosenFigureY + 1]) {
                case EMPTY: {
                    sighns[choosenFigureX - 1][choosenFigureY + 1] = SIGHN;
                    break;
                }
                case BLUE_FIGURE:
                case BLUE_QUEEN: {
                    if (board[choosenFigureX - 2][choosenFigureY + 2] == EMPTY) {
                        sighns[choosenFigureX - 2][choosenFigureY + 2] = SIGHN;
                    }
                    break;
                }
            } 
            break;
        }
        case BLUE_FIGURE: {
            switch (board[choosenFigureX - 1][choosenFigureY - 1]) {
                case EMPTY: {
                    sighns[choosenFigureX - 1][choosenFigureY - 1] = SIGHN;
                    break;
                }
                case RED_FIGURE:
                case RED_QUEEN: {
                    if (board[choosenFigureX - 2][choosenFigureY - 2] == EMPTY) {
                        sighns[choosenFigureX - 2][choosenFigureY - 2] = SIGHN;                    
                    }
                    break;
                }
            } 
            switch (board[choosenFigureX + 1][choosenFigureY - 1]) {
                case EMPTY: {
                    sighns[choosenFigureX + 1][choosenFigureY - 1] = SIGHN;
                    break;
                }
                case RED_FIGURE:
                case RED_QUEEN: {
                    if (board[choosenFigureX + 2][choosenFigureY - 2] == EMPTY) {
                        sighns[choosenFigureX + 2][choosenFigureY - 2] = SIGHN;
                    }
                    break;
                }
            } 
            break;
        }
        default: {
            break;
        }
    }
}

void hideSighns() {
    for (byte i = 0; i < 8; i++) {
        for (byte j = 0; j < 8; j++) {
            sighns[i][j] = EMPTY;
        }
    }
}

void makeMovement(byte x, byte y) {
    byte x_step;
    byte y_step;

    switch (choosenFigureType) {
        case RED_FIGURE:
        case RED_QUEEN:

            if (x > choosenFigureX) {
                x_step = 1;
            }
            if (x < choosenFigureX) {
                x_step = -1;
            }
            if (y > choosenFigureY) {
                y_step = 1;
            }
            if (y < choosenFigureY) {
                y_step = -1;
            }

        for (byte i = choosenFigureX; i != x; i += x_step) {
            for (byte j = choosenFigureY; j != y; j += y_step) {
                if (board[i][j] == BLUE_FIGURE || board[i][j] == BLUE_QUEEN) {
                    board[i][j] = EMPTY;
                }
            }
        }
        break;
        case BLUE_FIGURE:
        case BLUE_QUEEN:

            if (x > choosenFigureX) {
                x_step = 1;
            }
            if (x < choosenFigureX) {
                x_step = -1;
            }
            if (y > choosenFigureY) {
                y_step = 1;
            }
            if (y < choosenFigureY) {
                y_step = -1;
            }

        for (byte i = choosenFigureX; i != x; i += x_step) {
            for (byte j = choosenFigureY; j != y; j += y_step) {
                if (board[i][j] == RED_FIGURE || board[i][j] == RED_QUEEN) {
                    board[i][j] = EMPTY;
                }
            }
        }
        break;
    }

    board[choosenFigureX][choosenFigureY] = EMPTY;
    board[x][y] = choosenFigureType;
    chooseFigure(x,y);
}
