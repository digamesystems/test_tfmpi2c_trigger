#include <Wire.h>     // Arduino standard I2C/Two-Wire Library
#include <TFMPI2C.h>  // Include TFMini Plus LIDAR Library v1.5.0
                      //   https://github.com/budryerson/TFMini-Plus

TFMPI2C tfmP;

bool    externalTrigger = true; // Use to select between Self/External triggering 
uint8_t addr = 10;               // I2C address of my LIDAR sensor

//***************************************************************************
void setup() {
//*************************************************************************** 
  
  Serial.begin(115200);
  
  delay(2000);
  Serial.begin(115200);
  
  
  Wire.begin();
  Wire.setClock(400000);  // 400KHz.
  
  if(tfmP.sendCommand(SOFT_RESET, 0, addr)) {  
    Serial.println("TFMP-S initialized.");
  }
  else { 
    Serial.println("ERROR: Trouble initializing TFMP-S.");  
  }

  if (externalTrigger == false) {
    
    if (tfmP.sendCommand(SET_FRAME_RATE, FRAME_50, addr)) {
      Serial.println("TFMP-S set for self-triggered operation.");  
    }else{
      Serial.println("ERROR: Trouble setting self-triggered operation.");  
    }; 
    
  } else {  // Externally triggered
    if (tfmP.sendCommand(SET_FRAME_RATE, FRAME_0, addr)){
       Serial.println("TFMP-S set for externally-triggered operation.");  
    }else {
       Serial.println("ERROR: Trouble setting externally-triggered operation.");  
    }; 
    
    // Trigger the first reading
    if( tfmP.sendCommand(TRIGGER_DETECTION, 0, addr)) {
      Serial.println("First trigger sent.");  
    }else{
      Serial.println("Trouble Triggering LIDAR 1");
    }      
  }
  delay(3000); // Give us time to read the screen
}


//***************************************************************************
void loop() {
//***************************************************************************
  int16_t tfDist, tfFlux, tfTemp;
  unsigned int T1, T2;

  T1 = millis();
  T2 = T1; 

  // Take a measurement and route to serial monitor
  tfmP.getData(tfDist, tfFlux, tfTemp, addr); 
  
  Serial.print(tfmP.status); // Status code 0 = SUCCESS
  Serial.print(" ");
  Serial.print(tfDist);
  Serial.print(" ");
  Serial.print(tfFlux);
  Serial.print(" ");
  Serial.print(tfTemp);
  Serial.print(" ");

  if (externalTrigger){
    if(tfmP.status != TFMP_READY) {   
      //tfmP.sendCommand(SOFT_RESET, 0, addr);
    }
    Serial.print("T");
    Serial.print(" ");
    tfmP.sendCommand(TRIGGER_DETECTION, 0, addr);    
  }

  T2 = millis();
  Serial.print((T2-T1));
  Serial.print(" ");

  // introduce a delay to set the acquisition rate to c.a. 50Hz.
  unsigned long deltaT; 
  deltaT = 21 - (T2-T1); 

  if (deltaT >= 0) delay(deltaT);
  
  T2 = millis();
  Serial.println((T2-T1));
  
}
