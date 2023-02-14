unsigned long selectorDebounce[2] = {50, 0}; //? Debounce time, last time

void handleSelector()
{
    if (selectorDebounce[1] > millis())
    {
        return;
    }

    switch (mode)
    {
    //* TO SAFE
    case 3:
        mode = 0;
        tone(buzzerPin, melody[mode + 4], 50);
        break;

    //* TO SEMI
    case 0:
        mode++;
        tone(buzzerPin, melody[mode], 100);
        break;

    //* TO BURST
    case 1:
        mode++;
        tone(buzzerPin, melody[mode], 100);
        delay(100);
        tone(buzzerPin, melody[mode], 100);
        break;

    //* TO AUTO
    case 2:
        mode++;
        tone(buzzerPin, melody[mode], 100);
        delay(100);
        tone(buzzerPin, melody[mode], 100);
        delay(100);
        tone(buzzerPin, melody[mode], 100);
        break;
    }

    if (showStats)
    {
        clog("Mode", mode);
        cln();
    }
    triggerReleased = true;

    clearPos(0, 0);
    drawSelector();
    selectorDebounce[1] = millis() + selectorDebounce[0];
}
