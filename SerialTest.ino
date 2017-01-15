
int outpwm_pin=5;
int TACHO_PIN=4;

int tacho_value;
int pwm_value;
int x;
String str;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogWriteFreq(25000); // Set frequency to 25KHz
  Serial.write("PWM Test Up and running\n");
  Serial.print("Tacho on GPIO");
  Serial.println(TACHO_PIN);
  Serial.print("PWM on GPIO");
  Serial.println(outpwm_pin);
  
  Serial.setTimeout(3000);
  pinMode(TACHO_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  tacho_value = pulseIn(TACHO_PIN, HIGH);
  
    if(Serial.available() > 0)
    {
        x = Serial.parseInt();

        if (x > 0 && x <= 1024) {
          if (x == 1024) {
             x=0;
             //TODO PWM is disabled with zero -> set output to zero?
             //digitalWrite(outpwm_pin, LOW);
          }
          pwm_value=x;
          analogWrite(outpwm_pin, pwm_value);
          Serial.print("Updated pwm to : ");
          Serial.println(x, DEC);  // print as an ASCII-encoded decimal
        }
    }

    Serial.print("Input pwm: ");
    Serial.print(tacho_value, DEC);
    Serial.print("  Output pwm: ");
    Serial.print(pwm_value, DEC);
    Serial.println("  ");
    
    // delay 50 milliseconds before the next reading:
    delay(1000);
}

