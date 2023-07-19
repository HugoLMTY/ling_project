/*
   ? AXIS / DIMENSIONS
   *           Y
   *          0
   *          |
   * 0 -------------- 128
   *  X       |         X
   *          64
   *            Y
   *
   *
   ? GRID (screen usage)
   * /----------------\
   * |  {0,0} | {0,1} |
   * |----------------|
   * |  {1,0} | {1,1} |
   * \----------------/
   *
   ? TEMPLATES
  oled.drawLine(startX, startY, endX, endY, WHITE);
  oled.drawRect(startX, startY, width, height, WHITE);
  oled.fillRect(startX, startY, width, height, BLACK);
  oled.drawCircle(centerX, centerY, radius, WHITE);
  oled.drawTriangle(firstX , firstY, secondX, secondY, thirdX, thirdY, WHITE);
  oled.drawBitmap(startX, startY, bitmap, bitmap_height, bitmap_width, WHITE);
   ?
*/

//? --- CONFIG

//* DISPLAY
const bool showNerdStuff = false;
const bool showGrid = false;
const bool showDeepGrid = false;

const int transitionFrames = 10;   //? number of frames of position clearing
const int transitionDuration = 10; //? ms, duration of position clearing

const int textSize = 1;
const int textMargin = 2;
const int fontSizes[] = {0, 7, 16, 24}; //? pixels, size of a char

//* MODE / AMMOS
int ammos = 5;
const int maxAmmos = 250;
const bool autoReload = true;

//* GYRO
const int gyroPrecision = 45;  //? degrees, precision of the gyro
const int gyroAxisLength = 20; //? pixels, length of the gyro axis
const int gyroRadius = 10;     //? pixels, radius of the gyro circle

//? --- MODULES

//* SETTINGS
const bool showModules = true;
const int moduleWidth = width / 2;
const int moduleHeight = height / 2;
const int moduleMargin = 2;

//* MODULE TEMPLATE
const int modulePos[2] = {0, 0};    //? position on the GRID
const int moduleSize[2] = {25, 25}; //? pixels, w / h
const int moduleOffset[2] = {5, 5}; //? percentage, x / y | from top left 0,0

//* SELECTOR
const int selectorPos[2] = {0, 0};
const int selectorSize[2] = {25, 25};
const int selectorOffset[2] = {5, 5};

//* MAGAZINE
const int magazinePos[2] = {1, 0};
const int magazineSize[2] = {25, 25};
const int magazineOffset[2] = {5, 7};

//* GYRO
const int gyroPos[2] = {0, 1};
const int gyroSize[2] = {25, 25};
const int gyroOffset[2] = {0, 0};

//* TIMER
const int timerPos[2] = {1, 1};
const int timerSize[2] = {fontSizes[textSize] * 5, 5};
const int timerOffset[2] = {0, 0};

void oled_setup() {
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // initialize OLED display with address 0x3C for 128x64
        Serial.println(F("SSD1306 allocation failed"));
    }

    if (showNerdStuff) {
        Serial.print("OLED Width: ");
        Serial.print(width);
        Serial.print(" - OLED Height: ");
        Serial.println(height);
    }

    oled.clearDisplay();
    oled.setTextSize(textSize);
    oled.setTextColor(WHITE);
    oled.setCursor(0, 10);
    oled.println("sup bro");
    oled.display();
    delay(500);
    oled.clearDisplay();

    for (int i = 0; i <= moduleWidth; i += moduleWidth / transitionFrames) {
        //? X
        oled.drawLine(moduleWidth, moduleHeight, moduleWidth + i, moduleHeight, WHITE);
        oled.drawLine(moduleWidth - i, moduleHeight, moduleWidth, moduleHeight, WHITE);
        //? Y
        oled.drawLine(moduleWidth, moduleHeight, moduleWidth, moduleHeight + i, WHITE);
        oled.drawLine(moduleWidth, moduleHeight - i, moduleWidth, moduleHeight, WHITE);

        oled.display();
        delay(50);
    }
    delay(100);

    if (!showGrid) {
        for (float i = 0; i <= 1; i += 0.1) {
            oled.fillCircle(moduleWidth, moduleHeight, moduleWidth * i * 1.1, BLACK);
            oled.display();
            delay(25);
        }
        delay(500);
        oled.fillRect(0, 0, width, height, BLACK);
        oled.display();
    }

    if (showDeepGrid) {
        //? 1/4
        oled.drawLine(0, height / 4, width, height / 4, WHITE);
        oled.drawLine(width / 4, 0, width / 4, height, WHITE);
        //? 3/4
        oled.drawLine(0, (height / 4) * 3, width, (height / 4) * 3, WHITE);
        oled.drawLine((width / 4) * 3, 0, (width / 4) * 3, height, WHITE);

        oled.display();
    }

    drawSelector();
    drawMagazine();
    drawGyro();
    drawTimer();

    selectModule(0, 0);
    selectModule(1, 0);
    selectModule(0, 1);
    selectModule(1, 1);

    // oled.drawCircle(CenterX, CenterY, Radius, WHITE);
}

void oled_loop() {
    drawSelector();
    delay(transitionDuration * 2);
    drawMagazine();

    if (ammos == 0) {
        selectModule(1, 0);
        delay(2000);
        unselectModule(1, 0);
    }

    if (showModules) {
        switch (mode) {
        case 1:
            shoot();
            delay(500);

            shoot();
            delay(750);

            shoot();
            delay(1000);
            break;

        case 2:
            for (int b = 0; b < 3; b++) {
                shoot();
            }

            delay(500);

            for (int b = 0; b < 3; b++) {
                shoot();
            }
            break;

        case 3:
            for (int b = 0; b < 10; b++) {
                shoot();
            }
            break;
        }

        delay(2000);
        if (mode >= 3) {
            mode = 0;
        } else {
            mode++;
        }
        clearPos(0, 0);

        delay(250);
    }
    delay(500);
}

void selectModule(int posX, int posY) {
    const int startX = moduleWidth * posX + moduleMargin;
    const int startY = moduleHeight * posY + moduleMargin;

    const int mWidth = moduleWidth - (moduleMargin * 2);
    const int mHeight = moduleHeight - (moduleMargin * 2);

    oled.drawRoundRect(startX, startY, mWidth, mHeight, 3, WHITE);
    oled.display();
}
void unselectModule(int posX, int posY) {
    oled.drawRect(moduleWidth * posX, moduleHeight * posY, moduleWidth * posX, moduleHeight - (moduleHeight * posY), BLACK);
    oled.display();
}

void clearPos(int posX, int posY) {
    for (float i = 0; i <= 1; i += 0.1) {
        int startX = (moduleWidth * posX) + moduleMargin + 1;
        int startY = (moduleHeight * posY) + moduleMargin + 1;

        int mWidth = moduleWidth - (moduleMargin * 4);
        int mHeight = (moduleHeight - (moduleMargin * 2)) * i;

        oled.fillRect(startX, startY, mWidth, mHeight, BLACK);
        oled.display();
        delay(transitionDuration / transitionFrames);
    }
    delay(transitionDuration);
}

void oled_shoot() {
    //? Borders to show the shot
    oled.drawRect(0, 0, width, height, WHITE);
    oled.display();

    if (autoReload && ammos < 1)
    {
        ammos = maxAmmos;
    }

    //? Removes one ammo from the magazine and redraws it
    ammos--;
    drawMagazine();

    //? Removes the borders
    delay(10);
    oled.drawRect(0, 0, width, height, BLACK);
    oled.display();
}

void drawTimer() {
    const int start[2] = {
        (width * ((float)timerOffset[0] / (float)100)) + (moduleWidth * timerPos[0]) + moduleWidth / 2,
        (height * ((float)timerOffset[1] / (float)100)) + (moduleHeight * timerPos[1]) + moduleHeight / 2};

    oled.drawRect(start[0] - 20, start[1] - 5, 40, 10, WHITE);

    oled.fillRect(start[0] - 18, start[1] - 3, 27, 6, WHITE);

    oled.display();
    // oled.setTextColor(WHITE);
    // oled.setCursor(start[0], start[1]);

    // int minutes = (millis() / 1000) / 60;
    // int seconds = (millis() / 1000) % 60;

    // if (minutes < 10)
    // {
    //     oled.print("0");
    // }
    // oled.print(minutes);
    // oled.print(":");
    // if (seconds < 10)
    // {
    //     oled.print("0");
    // }
    // oled.println(seconds);

    // oled.display();
}

void drawGyro() {
    const int start[2] = {
        (width * ((float)gyroOffset[0] / (float)100)) + (moduleWidth * gyroPos[0]) + moduleWidth / 2,
        (height * ((float)gyroOffset[1] / (float)100)) + (moduleHeight * gyroPos[1]) + moduleHeight / 2};
    const int r = ((gyro_loop() / 100) / 2) + 100;

    //? Remove the old line
    oled.fillCircle(start[0], start[1], gyroRadius, BLACK);

    //? Draw the new line
    int xStart = start[0] + (cos(radians(r)) * (gyroRadius / 2));
    int yStart = start[1] + (sin(radians(r)) * (gyroRadius / 2));

    int xEnd = start[0] - (cos(radians(r)) * (gyroRadius / 2));
    int yEnd = start[1] - (sin(radians(r)) * (gyroRadius / 2));

    oled.drawLine(xStart, yStart, xEnd, yEnd, WHITE);

    oled.display();
    delay(50);
}

void drawMagazine() {
    const int start[2] = {//? pixels, x / y
                          (width * ((float)magazineOffset[0] / (float)100)) + (moduleWidth * magazinePos[0]),
                          (height * ((float)magazineOffset[1] / (float)100)) + (moduleHeight * magazinePos[1])};

    const int textOffset[2] = {//? pixels, x / y | based on icon position + margin
                               start[0] + magazineSize[0] + textMargin,
                               start[1] + ((magazineSize[1] / 2) - fontSizes[textSize] * 3 / 2)};

    if (showNerdStuff)
    {
        clog("Icon Width", magazineSize[0], true);
        clog("Icon Height", magazineSize[1], true);
        cln();

        clog("Offset X", magazineOffset[0], true);
        clog("Offset Y", magazineOffset[1], true);
        cln();

        clog("Start X", start[0], true);
        clog("Start Y", start[1], true);
        cln();
        // Serial.print("Icon Width: ");
        // Serial.print(magazineSize[0]);
        // Serial.print(" - Icon Height: ");
        // Serial.println(magazineSize[1]);

        // Serial.print("Offset X: ");
        // Serial.print(magazineOffset[0]);
        // Serial.print(" - Offset Y: ");
        // Serial.println(magazineOffset[1]);

        // Serial.print("Start X: ");
        // Serial.print(start[0]);
        // Serial.print(" - Start Y: ");
        // Serial.println(start[1]);

        Serial.print("oled.drawRect(");
        Serial.print(start[0]);
        Serial.print(", ");
        Serial.print(start[1]);
        Serial.print(", ");
        Serial.print(selectorSize[0]);
        Serial.print(", ");
        Serial.print(selectorSize[1]);
        Serial.println(", WHITE);");
    }

    if (ammos != 0)
    {
        oled.fillRect(start[0], start[1], magazineSize[0], magazineSize[1], BLACK);
        oled.drawBitmap(start[0], start[1], magazine, magazineSize[0], magazineSize[1], WHITE);
    }
    else
    {
        oled.drawBitmap(start[0], start[1], magazine_empty, magazineSize[0], magazineSize[1], WHITE);
    }

    //? Clear old text
    oled.fillRect(textOffset[0], textOffset[1], fontSizes[textSize] * 4, fontSizes[textSize], BLACK);

    //? Current ammos
    oled.setCursor(textOffset[0], textOffset[1]);
    oled.println(ammos);

    //? Max ammos
    oled.setCursor(textOffset[0] + fontSizes[textSize] / 2, textOffset[1] + fontSizes[textSize] + textMargin);
    oled.print("/");
    oled.println(maxAmmos);

    oled.display();
}

void drawSelector() {
    const int start[2] = {
        (width * ((float)selectorOffset[0] / (float)100)) + (moduleWidth * selectorPos[0]),
        (height * ((float)selectorOffset[1] / (float)100)) + (moduleHeight * selectorPos[1]) + 1};

    const int textOffset[2] = {
        start[0] + selectorSize[0] + textMargin,
        start[1] + ((selectorSize[1] / 2) - fontSizes[textSize] / 2)};

    if (false && showNerdStuff)
    {
        Serial.print("Icon Width: ");
        Serial.print(selectorSize[0]);
        Serial.print(" - Icon Height: ");
        Serial.println(selectorSize[1]);

        Serial.print("Offset X: ");
        Serial.print(selectorOffset[0]);
        Serial.print(" - Offset Y: ");
        Serial.println(selectorOffset[1]);

        Serial.print("Start X: ");
        Serial.print(start[0]);
        Serial.print(" - Start Y: ");
        Serial.println(start[1]);

        Serial.print("oled.drawRect(");
        Serial.print(start[0]);
        Serial.print(", ");
        Serial.print(start[1]);
        Serial.print(", ");
        Serial.print(selectorSize[0]);
        Serial.print(", ");
        Serial.print(selectorSize[1]);
        Serial.println(", WHITE);");
    }

    oled.drawBitmap(start[0], start[1], bullet_type[mode], selectorSize[0], selectorSize[1], WHITE);
    oled.setCursor(textOffset[0], textOffset[1]);

    //? 0 SAFE
    //? 1 SEMI
    //? 2 BURST
    //? 3 AUTO
    // very useful documentation 11/10
    switch (mode) {
    case 0:
        oled.println("SAFE");
        break;
    case 1:
        oled.println("SEMI");
        break;
    case 2:
        oled.println("BUR.");
        break;
    case 3:
        oled.println("AUTO");
        break;
    }

    oled.display();
};
