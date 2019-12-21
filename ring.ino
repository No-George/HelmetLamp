void Light(bool flag){
  ring.ClearTo(black);
  
  //Calculate pulse status 
  if (flag==1){
    int timer = (millis() % (Pulse*2));
    Brightness = 80+40*(abs(timer-Pulse))/Pulse;
  }else{
    Brightness = 100;
  }
  R = Brightness;
  G = Brightness* 0.6;
  B = Brightness* 0.6;
  

  for (int i = 0; i<=38; i++){
  ring.SetPixelColor(i, RgbColor(R,G,B));
  }
 
  
  //remove requirement for 0% if statement
    ring.Show();
}


void Portion(int lights){
  lights = lights/3;
    ring.ClearTo(black);
    Serial.println(lights);
    for (int i = 0; i<=lights; i++){
  ring.SetPixelColor(i, RgbColor(50,50,50));
      ring.Show();
  }
}
