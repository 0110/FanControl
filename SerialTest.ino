
#define FAN2_PWM_OFFSET 2000

int OUTPWM_PIN1=5;
int TACHO_PIN1=4;
int OUTPWM_PIN2=14;
int TACHO_PIN2=12;

unsigned long tacho_value1;
int pwm_value1;
unsigned long tacho_value2;
int pwm_value2;

int x;

/**
 * @param duration high cycle in microseconds
 */
float convertDelay2rpm(unsigned long duration) {
  /* Found at: http://www.energyscienceforum.com/showthread.php?t=905 */
  if(duration > 5000) // duration of 5000 micro seconds limits speed to 3000 RPM. This stops erroneous data caused by transient spikes
  {
   return (15000000.0/duration); // (1000000uS * 1/4 * 60s) / duration = 15000000.0 / duration keeps calculation simple
  }
  else
  {
    return 0.0f;
  }
}

void statusPrintln() {
  Serial.print("Tacho1 on GPIO");
  Serial.println(TACHO_PIN1);
  Serial.print("Tacho2 on GPIO");
  Serial.println(TACHO_PIN2);
  Serial.print("PWM1 on GPIO");
  Serial.println(OUTPWM_PIN1);
  Serial.print("PWM2 on GPIO");
  Serial.println(OUTPWM_PIN2);
  Serial.println("\nCommands\n 1-1024: Sets PWM1 (1: slow, 1023: full speed, 1024: off)\n 2000-3023 Sets PWM2 (2000: off, 2023: fullspeed)\n");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogWriteFreq(25000); // Set frequency to 25KHz
  Serial.write("PWM Test Up and running\n");
  statusPrintln();
  
  Serial.setTimeout(3000);
  pinMode(TACHO_PIN1, INPUT);
  pinMode(TACHO_PIN2, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  
  tacho_value1 = pulseIn(TACHO_PIN1, HIGH);
  tacho_value2 = pulseIn(TACHO_PIN2, HIGH);
  
    if(Serial.available() > 0)
    {
        x = Serial.parseInt();
        if (x == 0) {
          statusPrintln();
        } else if (x < FAN2_PWM_OFFSET) {
          // FAN 1 
          if (x > 0 && x <= 1024) {
            if (x == 1024) {
               x=0;
               //TODO PWM is disabled with zero -> set output to zero?
               //digitalWrite(outpwm_pin, LOW);
            }
            
            pwm_value1=x;
            analogWrite(OUTPWM_PIN1, pwm_value1);
            Serial.print("Updated pwm1 to : ");
            Serial.println(x, DEC);  // print as an ASCII-encoded decimal
          }
        } else {
            // FAN 2
            x = x - FAN2_PWM_OFFSET;
            if (x >= 0 && x <= 1024) {
            pwm_value2=x;
            analogWrite(OUTPWM_PIN2, pwm_value2);
            Serial.print("Updated pwm2 to : ");
            Serial.println(x, DEC);  // print as an ASCII-encoded decimal
          }
        }

    }

    Serial.print("Input pwm1: ");
    Serial.print(tacho_value1, DEC);
    Serial.print(" ");
    Serial.print(convertDelay2rpm(tacho_value1));
    Serial.print("rpm  Input pwm2: ");
    Serial.print(tacho_value2, DEC);
    Serial.print(" ");
    Serial.print(convertDelay2rpm(tacho_value2));
    Serial.print("rpm  Output pwm1: ");
    Serial.print(pwm_value1, DEC);
    Serial.print("  Output pwm2: ");
    Serial.print(pwm_value2, DEC);
    Serial.println("  ");
    
    // delay 50 milliseconds before the next reading:
    delay(1000);
}

