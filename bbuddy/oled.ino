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
   ? MODULES GRID (screen usage)
   * /----------------\
   * |  {0,0} | {0,1} |
   * |----------------|
   * |  {1,0} | {1,1} |
   * \----------------/
   *
   ? INTERFACE GRID (screen usage)
   * /--------------------\
   * |                    |
   * |     INFORMATIONS   |
   * |                    |
   * |--------------------|
   * |       NAVBAR       |
   * \--------------------/
   *
   ? TEMPLATES
  oled.setCursor(X, Y);
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

const bool displayMode = 0;
// 0 - Modules (2*2)
// 1 - Navbar (text based)
// 2 - u tell me

const bool skipBootAnimation = true;
const bool showNerdStuff = false;
const bool showGrid = false;
const bool showDeepGrid = false;

const int transitionFrames = 10;   //? number of frames of position clearing
const int transitionDuration = 10; //? ms, duration of position clearing

const int textSize = 1;
const int textMargin = 2;
const int fontSizes[] = {0, 7, 16, 24}; //? pixels, size of a char

//* SETTINGS
const bool showModules = true;
const int moduleWidth = width / 2;
const int moduleHeight = height / 2;
const int moduleMargin = 2;


//* MODULE TEMPLATE
const int modulePos[2] = {0, 0};    //? position on the GRID
const int moduleSize[2] = {25, 25}; //? pixels, w / h
const int moduleOffset[2] = {5, 5}; //? percentage, x / y | from top left 0,


//* INTERFACE 

// const int maxWidth = 90; // %
// const int maxHeight = 90; // %, minus absolute navbar height
// int maxPos[2] = {64 * (maxHeight / 100), 128 * (maxWidth / 100)}; // later (too much maths)
int maxPos[2] = {64, 128};
int cursor[2] = {0, 0};

//* NAVBAR
const int navbarOffset = 5;
const int navbarHeight = fontSizes[textSize] + navbarOffset;
// String navMessage = "Hello there";

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
  oled.println("sup fam");
  oled.display();
  delay(1000);
  clearDisplay();

  if (!skipBootAnimation) {
    bootAnimation();
  }

  switch (displayMode) {
    case 0:
      break;

    case 1:
      navbar_setup();
      break;

    default:
      break;
  }
}

void navbar_setup() {
  for (int i = 0; i <= maxPos[1] / 2; i += maxPos[1] / transitionFrames) {
    oled.drawLine((maxPos[1] / 2), maxPos[0] - navbarHeight, (maxPos[1] / 2) + i, maxPos[0] - navbarHeight, WHITE);
    oled.drawLine((maxPos[1] / 2), maxPos[0] - navbarHeight, (maxPos[1] / 2) - i, maxPos[0] - navbarHeight, WHITE);
    oled.display();
  }

  clearDisplay();
}

void bootAnimation() {
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
    for (float i = 0; i <= 1; i += 0.1)
    {
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
}


void oline(String text, bool newLine, bool reset) {
  // return; 
  if (newLine) {
    Serial.println(text);
  } else {
    Serial.print(text);
  }

  if (cursor[0] + text.length() > (maxPos[0] - navbarHeight) || reset) {
    Serial.println("RESETETET");
    clearDisplay();
    cursor[0] = 0;
    cursor[1] = 0;
  }

  oled.setCursor(cursor[1], cursor[0]);
  oled.print(text);
  
  oled.display();

  if (cursor[0] > maxPos[1]) {
    cursor[0] += fontSizes[textSize] + 3;
    cursor[1] = 0;
  } 
  else 
  if (newLine) {
    cursor[0] += fontSizes[textSize] + 3;
    cursor[1] = 0;
  } 
  else 
  if (!newLine) {
    cursor[1] += (text.length() * fontSizes[textSize]);
  }
}
void clearDisplay() {
  switch (displayMode) {
    default:
    case 0:
      oled.fillRect(0, 0, maxPos[1], maxPos[0], BLACK); // could use oled.clearDisplay() but no
      break;
      
    case 1:
      oled.fillRect(0, 0, maxPos[1], (maxPos[0] - navbarHeight) - 1, BLACK);
      break;
  }

  oled.display();
}


void setNavMessage(String message, String type) {
  const int previousCursor[2] = {cursor[0], cursor[1]}; // Dont know if prev[2] = cursor; works, too much compil time wont find out
  
  // navbarMessage = message;

  int navOffset = navbarHeight - navbarOffset;

  if (type == "network") {
    // Starts from the end (RIGHT)
    // oled.setCursor(maxPos[1] - (message.length() * fontSizes[textSize]), maxPos[0] - navOffset); // Was before string truncate, wip still unsure
    oled.setCursor((maxPos[1] / 2) + 1, maxPos[0] - navOffset);
  }
  else
  if (type == "status") {
    // Starts from the start (LEFT)
    oled.setCursor(0, maxPos[0] - navOffset);
    message = PlayerId + "| " + message;
  }
  else
  if (type == "kill") {
    // Starts from the start (LEFT)
    oled.setCursor(0, maxPos[0] - navOffset);
    message = PlayerId + "| " + killCount + "k " + message;
  }
  else {
    // Whatever i dont use it yet (LEFT default)
    oled.setCursor(0, maxPos[0] - navOffset);
  }


  if (message.length() * fontSizes[textSize] > maxPos[1] / 2) {
    const int maxChars = (maxPos[1] / 2) / fontSizes[textSize] + 2;
    message = message.substring(0, maxChars - 4) + "..";
  }

  oled.println(message);
  oled.display();

  cursor[0] = previousCursor[0];
  cursor[1] = previousCursor[1]; // yea yea i'll fix this later
}
void clearNav(String type) {
  const int startY = maxPos[0] - (navbarHeight - navbarOffset); // Only the chars, not the style (pwease uwu)

  if (type == "network") {
    // Starts from the end (RIGHT)
    oled.fillRect(maxPos[1] / 2, startY, maxPos[1] / 2, navbarHeight, BLACK);
  }
  else
  if (type == "status") {
    // Starts from the start (LEFT)
    oled.fillRect(0, startY, maxPos[1] / 2, navbarHeight, BLACK);
  }
  else
  if (type == "kill") {
    // Starts from the start (LEFT)
    oled.fillRect(0, startY, maxPos[1] / 2, navbarHeight, BLACK);
  }
  else {
    // Whatever i dont use it yet (LEFT default)
    oled.fillRect(0, startY, maxPos[1] / 2, navbarHeight, BLACK);
  }

  oled.display();
}