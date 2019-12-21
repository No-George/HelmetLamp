int button1_capture(){
    for (int i=0; i<(debounce-1);i++){
    Button1[i]=Button1[i+1];
  }
    Button1[debounce-1] = (touchRead(T0));
    float current = MaxArray(Button1);
    float Button1_smooth = Button1_total/smoothing;

//  float current = AveArray(Button1);
  Serial.print(Button1_smooth);Serial.print("   ");Serial.print(current);Serial.print("    ");Serial.println(Button1[debounce-1]);
  if (current < (0.85*Button1_smooth)){
    Button1_total = 0; //works as a debounce
    return 1;
  }else{
    Button1_total = current +Button1_total -Button1_smooth;
  }
  return 0;
}

float MaxArray(float MaxMe[]){
  float mxm = MaxMe[0];
  float mnm = MaxMe[0];
  for (int i=0; i<debounce; i++) {
    if (MaxMe[i]>mxm) {
      mxm = MaxMe[i];
    }
  }
return mxm;
}

float AveArray(float AveMe[]){
  float total = 0;
  float ave = 0;
  for (int i=0; i<debounce; i++) {
      total+=AveMe[i];
  }
  ave = total/debounce;
return ave;
}
