//Set Timer Handler:
void setTimerHandler()
{
  static unsigned long startMillis = millis();
  static unsigned long resetMillis = millis();
  
  if(startState == LOW)
  {
    if(millis() - startMillis > 250)
    {
      exitFlag = false;
      delay(1);
      states = gameStart;
    }
    startMillis = millis();
  }

  if(resetState == LOW)
  {
    if(millis() - resetMillis > 250)
    {
      exitFlag = false;
      delay(1);
      resetScreen();
    }
    resetMillis = millis();
  }
  
  switch(timerSwitch)
  {
    case 0:
      timerSwitch = 1;
      break;
    case 1:
      matrixWrite(true, 1, 0, " SET", 255, 255, 255);
      matrixWrite(false, 1, 0, "15sec", 255, 255, 255);
      tMinus = 16000;
      if(!timerFlag)
      {
        timerFlag = true;
      }
      break;
    case 2:
      matrixWrite(true, 1, 0, " SET", 255, 255, 255);
      matrixWrite(false, 1, 0, "30sec", 255, 255, 255);
      tMinus = 31000;
      if(!timerFlag)
      {
        timerFlag = true;
      }
      break;
    case 3:
      matrixWrite(true, 1, 0, " SET", 255, 255, 255);
      matrixWrite(false, 1, 0, "45sec", 255, 255, 255);
      botMatrix.show();
      tMinus = 46000;
      if(!timerFlag)
      {
        timerFlag = true;
      }
      break;
    case 4:
      matrixWrite(true, 1, 0, " SET", 255, 255, 255);
      matrixWrite(false, 1, 0, "60sec", 255, 255, 255);
      tMinus = 61000;
      if(!timerFlag)
      {
        timerFlag = true;
      }
      break;
    case 5:
      matrixWrite(true, 1, 0, "SET 9", 255, 255, 255);
      matrixWrite(false, 1, 0, "BALLS", 255, 255, 255);
      tMinus = 0;
      if(timerFlag)
      {
        timerFlag = false;
      }
      break;
    default:
      timerSwitch = 0;
      return;
  }
}
//End Set Timer Handler
