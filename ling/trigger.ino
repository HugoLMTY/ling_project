void handleTrigger() {
    switch (mode) {

    //* SAFE
    case 0:
        tone(buzzerPin, melody[mode + 4], 100);
        break;

    //* SEMI
    case 1:
        if (triggerReleased) {
            triggerReleased = false;
            shoot();
        }
        break;

    //* BURST
    case 2:
        //? If the trigger is already pressed and the auto burst is disabled, returns
        if (!triggerReleased && !allowBurstAuto) {
            return;
        }

        if (triggerReleased && !allowBurstAuto) {
            triggerReleased = false;
        }

        for (int i = 0; i < burstLength; i++) {
            shoot();
            delay(interval / 2);
        }
        break;

    //* AUTO
    case 3:
        shoot();
        break;
    }
}

void shoot() {
    //? Shoot action
    digitalWrite(relayPin, HIGH);
    delay(dwell);
    digitalWrite(relayPin, LOW);

    oled_shoot();
}
