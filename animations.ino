//idle animations
void idleAnimations(byte r, byte g, byte b)
{
  //static unsigned long startMillis = millis();
  static int scroll = matrixWidth;
  static unsigned long scrollMillis = millis();
  switch (idleAni)
  {
    case 0:
      idleAni = 1;
      break;
    case 1:
      scroll = matrixWidth;
      matrixWrite(true, 1, 0, "COME", r, g, b);
      matrixWrite(false, 1, 0, " PLAY", b, r, g);
      break;
    case 2:
      matrixWrite(true, 1, 0, "PUTT", r, g, b);
      matrixWrite(false, 1, 0, " SKEE", b, r, g);
      break;
    case 3:
      matrixWrite(true, 1, 0, "PRESS", r, b, g);
      matrixWrite(false, 1, 0, "START", b, r, g);
      break;
    case 4:
      matrixWrite(true, 4, 0, " TO ", g, r, b);
      matrixWrite(false, 4, 0, "PLAY", b, g, r);
      break;
    case 5:
      if(hiPoints > 0)
      {
        while(scroll >- 50)
        {
          startState = digitalRead(startPin);
          clockState = digitalRead(clockPin);
          static unsigned long clockMillis = millis();
          static unsigned long startMillis = millis();
          if(startState == LOW)
          {
            if(millis() - startMillis > 250)
            {
              scroll = matrixWidth;
              return;
            }
          }
          if(clockState == LOW)
          {
            if(millis() - clockMillis > 250)
            {
              scroll = matrixWidth;
              return;
            }
          }
          matrixWrite(false, 1, 0, " " + String(hiPoints), 255, 0 ,0);
          topMatrix.setTextColor(topMatrix.Color(255, 0, 255));
          topMatrix.fillScreen(0);
          topMatrix.setCursor(scroll, 0);
          topMatrix.print(F("HI SCORE"));
          topMatrix.show();
          if(millis() - scrollMillis > 50)
          {
            scroll--;
            scrollMillis = millis();
            if(scroll <- 50)
            {
              scroll = matrixWidth;
            }
          }
        }
      }
      break;
    default:
      idleAni = 0;
      return;
  }
}
//End Idle Animations
