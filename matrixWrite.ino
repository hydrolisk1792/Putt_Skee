//Applet for writing to the Matrix
void matrixWrite(boolean p, byte c, byte d, String t, byte r, byte g, byte b)
{
  if(p)
  {
    topMatrix.fillScreen(0);
    topMatrix.setTextColor(topMatrix.Color(r, g, b));
    topMatrix.setCursor(c, d);
    topMatrix.print(t);
    topMatrix.show();
  }
  else if(!p)
  {
    botMatrix.fillScreen(0);
    botMatrix.setTextColor(topMatrix.Color(r, g, b));
    botMatrix.setCursor(c, d);
    botMatrix.print(t);
    botMatrix.show();
  }
}
//End Applet




//Reset Screen
void resetScreen()
{
  matrixWrite(true, 1, 0, "GAME ", 255, 0, 255);
  matrixWrite(false, 1, 0, "RESET", 255, 0, 255);
  delay(1500);
  states = clearAll;
  delay(500);
  return;
}
//End Reset Screen
