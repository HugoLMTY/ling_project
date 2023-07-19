/*
   ? AXIS / DIMENSIONS
   *           Y
   *           0
   *          |
   * 0 -------------- 128
   *  X       |         X
   *          64
   *            Y
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

const int outlineRadius = (height / 2) - 4;

void markop_setup() {
    Serial.begin(9600);

    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // initialize OLED display with address 0x3C for 128x64
        Serial.println("SSD1306 allocation failed");
    }

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0, 10);
    oled.println("sup bro");
    oled.display();

    delay(100);
    oled.clearDisplay();

    drawCrosshair();
}

void markop_loop() {
}

void drawCrosshair() {
    drawGrid();
    drawOutline();
    drawCenter();
    drawDistanceStuff();
}

void drawGrid() {
    for (float i = 0; i < 1; i += 0.05) {
        oled.drawLine(width / 2, height / 2, width / 2 + ((width / 2) * i), height / 2, WHITE);
        oled.drawLine(width / 2, height / 2, width / 2 - ((width / 2) * i), height / 2, WHITE);

        oled.drawLine(width / 2, height / 2, width / 2, height / 2 + ((height / 2) * i), WHITE);
        oled.drawLine(width / 2, height / 2, width / 2, height / 2 - ((height / 2) * i), WHITE);

        oled.display();
    }
}

void drawOutline() {
    int radius;
    bool dotted = true;
    for (float i = 0; i < 1; i += 0.05) {

        int currentRadius = outlineRadius * i;

        oled.drawCircle(width / 2, height / 2, currentRadius, WHITE);

        if (!dotted) {
            oled.drawCircle(width / 2, height / 2, radius, BLACK);
        }

        radius = currentRadius;

        if (i > (float)0.8) {
            dotted = !dotted;
        } else {
            dotted = false;
        }

        oled.display();
        delay(25);
    }
}

void drawDistanceStuff() {
    const int start[2] = {width / 2 - outlineRadius, height / 2 - outlineRadius};
    const int center[2] = {start[0] + outlineRadius, start[1] + outlineRadius};
    const int _end[2] = {center[0] + outlineRadius, center[1] + outlineRadius};

    const int marks = 5;
    const int markOffset = 6;
    const int markSize = 2;

    for (int mark = 1; mark < marks; mark++) {
        oled.drawLine(center[0], center[1] + (mark * markOffset), center[0] + ((marks - mark) * markSize), center[1] + (mark * markOffset), WHITE);
        oled.drawLine(center[0], center[1] + (mark * markOffset), center[0] - ((marks - mark) * markSize), center[1] + (mark * markOffset), WHITE);

        oled.display();
        delay(25);
    }
}

void drawCenter() {
    const int start = width / 2 - outlineRadius;
    const int __end = start + (outlineRadius * 2);

    for (float i = 0; i < 0.5; i += 0.05) {

        oled.drawLine(__end, height / 2, __end - outlineRadius * i, height / 2, WHITE);
        oled.drawLine(start, height / 2, start + outlineRadius * i, height / 2, WHITE);

        oled.display();
        delay(50);
    }

    oled.fillCircle(width / 2, height / 2, 1, WHITE);
    oled.display();
    // delay(10);

    // oled.drawCircle(width / 2, height / 2, 1, BLACK);
    // oled.drawCircle(width / 2, height / 2, 2, WHITE);
    // oled.display();
}
