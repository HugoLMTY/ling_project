// oled.h

#ifndef OLED_H
#define OLED_H

void oled_setup();

const bool DEFAULT_NEWLINE = true;
const bool DEFAULT_RESET = false;
const String DEFAULT_TYPE = "status";

void oline(String text, bool newLine = DEFAULT_NEWLINE, bool DEFAULT_RESET = DEFAULT_RESET);
void setNavMessage(String text, String type);

#endif // OLED_H