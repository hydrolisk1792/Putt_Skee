//Game Start Handler:
void gameStartHandler()
{
  static unsigned long resetMillis = millis();
  
  if(resetState == LOW)
  {
    if(millis() - resetMillis > 250)
    {
      resetScreen();
    }
    resetMillis = millis();
  }
  
  switch(gameStartSwitch)
  {
    case 0:
      gameStartSwitch = 1;
      break;
    case 1:
      matrixWrite(true, 1, 0, " GET", 255, 20, 255);
      matrixWrite(false, 1, 0, "READY", 255, 20, 255);
      delay(1000);
      gameStartSwitch =  2;
      break;
    case 2:
      matrixWrite(true, 1, 0, " GET", 255, 10, 150);
      matrixWrite(false, 1, 0, " SET", 255, 10, 150);
      delay(1000);
      gameStartSwitch =  3;
      break;
    case 3:
      matrixWrite(true, 1, 0, "PUTT!", 255, 255, 255);
      matrixWrite(false, 1, 0, "     ", 0, 0, 0);
      delay(1000);
      points = 0;
      gameStartSwitch =  4;
      break;
    case 4:
      states = gamePlay;
      break;
    default:
      gameStartSwitch = 0;
      return;
  }
}




//Game Play Handler:
void gamePlayHandler()
{
  static unsigned long resetMillis = millis();
  unsigned long timerDuration = tMinus;
  unsigned long remainingTime = timerStartTime + timerDuration - millis();

  //Game Play reset clause:
  if(resetState == LOW)
  {
    if(millis() - resetMillis > 250)
    {
      resetScreen();
    }
    resetMillis = millis();
  }
  //End Reset
  
  switch(gamePlaySwitch)
  {
    case 0:     //Selects which gameplay (Balls Play or Timer Play)
      if(!timerFlag)
      {
        gamePlaySwitch = 1;     //Balls Play
      }
      else if(timerFlag)
      {
        timerStartTime = millis();
        gamePlaySwitch = 2;     //Timer Play
      }
      break;
      
    case 1:     //Balls Game
      matrixWrite(true, 1, 0, "Ball" + String(ballNum), 255, 0, 255);
      matrixWrite(false, 1, 0, " " + String(points), 255, 0, 0);

      //Game Over Trigger:
      if(ballNum <= 0)
      {
        delay(950);     //This needs to be a lower number than the "triggerDebounce" number!!!!!
        gamePlaySwitch = 3;
      }
      //End Game Over Trigger

      scoringHandler(true);
      break;
      
    case 2:     //Timer Game
      matrixWrite(true, 1, 0, "T- " + String(remainingTime / 1000), 255, 0, 255);
      matrixWrite(false, 1, 0, " " + String(points), 255, 0, 0);

      //Game Over Trigger:
      if(millis() - timerStartTime >= timerDuration)
      {
        delay(950);
        gamePlaySwitch = 3;
      }
      //End Game Over Trigger
      
      scoringHandler(false);
      break;
    case 3:
      states = gameOver;
      break;
    default:
      gamePlaySwitch = 0;
      return;
  }
}




//Scoring Handler
void scoringHandler(boolean b)
{
  static unsigned long points50Millis = millis();
  static unsigned long points40Millis = millis();
  static unsigned long points30Millis = millis();
  static unsigned long points20Millis = millis();
  static unsigned long points10Millis = millis();
  static unsigned long zeroTriggerMillis = millis();
  
  if(points50State == LOW)
  {
    if(millis() - points50Millis > triggerDebounce)
    {
      points += 50;
      if(b) {ballNum --;}
    }
    points50Millis = millis();
  }
  if(points40State == LOW)
  {
    if(millis() - points40Millis > triggerDebounce)
    {
      points += 40;
      if(b) {ballNum --;}
    }
    points40Millis = millis();
  }
  if(points30State == LOW)
  {
    if(millis() - points30Millis > triggerDebounce)
    {
      points += 30;
      if(b) {ballNum --;}
    }
    points30Millis = millis();
  }
  if(points20State == LOW)
  {
    if(millis() - points20Millis > triggerDebounce)
    {
      points += 20;
      if(b) {ballNum --;}
    }
    points20Millis = millis();
  }
  if(points10State == LOW)
  {
    if(millis() - points10Millis > triggerDebounce)
    {
      points += 10;
      if(b) {ballNum --;}
    }
    points10Millis = millis();
  }
  if(zeroTriggerState == LOW)
  {
    if(millis() - zeroTriggerMillis > triggerDebounce)
    {
      if(b) {ballNum --;}
    }
    zeroTriggerMillis = millis();
  }
}
//End Scoring Handler




//Game Over:
void gameOverHandler()
{
  switch(gameOverSwitch)
  {
    case 0:
      gameOverSwitch = 1;
      break;
    case 1:
      matrixWrite(true, 1, 0, "GAME", 255, 0, 255);
      matrixWrite(false, 1, 0, " OVER", 255, 0, 255);
      delay(2000);
      gameOverSwitch = 2;
      break;
    case 2:     //High Score Handler:
      if(!timerFlag)
      {
        if(points <= 0)
        {
          gameOverSwitch = 7;
        }
      }
      else if(timerFlag)
      {
        if(points <= 0)
        {
          gameOverSwitch = 7;
        }
      }
      if(points > hiPoints)
      {
        hiPoints = points;
        gameOverSwitch = 6;
      }
      else if(points < hiPoints)
      {
        if(points <= 0)
        {
          gameOverSwitch = 7;
        }
        else
        {
          gameOverSwitch = 3;
        }
      }
      break;     //End High Score Handler
    case 3:
      matrixWrite(true, 1, 0, "YOUR", 255, 0, 255);
      matrixWrite(false, 1, 0, "SCORE", 255, 0, 255);
      delay(1500);
      gameOverSwitch = 4;
      break;
    case 4:
      matrixWrite(true, 1, 0, " IS ", 255, 0, 255);
      matrixWrite(false, 1, 0, "     ", 0, 0, 0);
      delay(1500);
      gameOverSwitch = 5;
      break;
    case 5:
      matrixWrite(true, 1, 0, "SCORE", 255, 0, 255);

      for(int i = 5; i > 0; i--)
      {
        if(i == 1)
        {
          gameOverSwitch = 7;
        }
        matrixWrite(false, 1, 0, " " + String(points), 255, 0, 0);
        delay(750);
        matrixWrite(false, 1, 0, "     ", 0, 0, 0);
        delay(750);
      }
      break;
    case 6:
      switch(hiScoreSwitch)
      {
        case 0:
          hiScoreSwitch = 1;
          break;
        case 1:
          matrixWrite(true, 1, 0, " NEW", 255, 0, 255);
          matrixWrite(false, 1, 0, " HI ", 255, 0, 255);
          delay(1500);
          hiScoreSwitch = 2;
          break;
        case 2:
          matrixWrite(true, 1, 0, "SCORE", 255, 0, 255);
          for(int i = 5; i > 0; i--)
          {
            if(i == 1)
            {
              gameOverSwitch = 7;
            }
            matrixWrite(false, 1, 0, " " + String(points), 255, 0, 0);
            delay(750);
            matrixWrite(false, 1, 0, "     ", 0, 0, 0);
            delay(750);
          }
          break;
      }
      break;
    case 7:
      states = clearAll;
      return;
      break;
    default:
      gameOverSwitch = 0;
      return;
  }
}
//End Game Over
