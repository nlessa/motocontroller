#include <LiquidCrystal.h>



//Portas
const int buttonCenter = A0;
const int buttonLeft = A1;
const int buttonRight = A2;
const int buttonUp = A3;
const int buttonDown = A4;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//Constantes
const int INIT_STATE = 0; 
const int YES_NO_STATE = 1; 
const int AQUECER_AGUA_STATE = 2; 
const int RECUPERAR_STATE = 3; 

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

//Variáveis
int state = INIT_STATE;
int lastButtonState = -1;
int lcdSelected = 1;
int simAction;
int noAction;

void setup() {
  pinMode(buttonCenter, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  switch (state)
  {
      case INIT_STATE:
      {
        lcd.print("Deseja reiniciar?");
        simAction = AQUECER_AGUA_STATE;
        noAction = RECUPERAR_STATE;
        state = YES_NO_STATE;
        lcdSelected = 0;
        break;
      }
      case YES_NO_STATE:
      {
        int btn = lerBotao();
        if ( btn == buttonLeft || btn == buttonRight) 
        {
            if ( btn == buttonLeft)
              lcdSelected = 0;
            else
              lcdSelected = 1;
            if (lcdSelected )
            {
                
            }
            else
            {
                
            }
        }
        if ( btn == buttonCenter)
        {
            if (lcdSelected )
            {
                state = noAction;
            }
            else
            {
                state = simAction;
            }
        }
        break;
      }
      case AQUECER_AGUA_STATE:
      {
        lcd.print("Aquecendo!");
        break;
      }
      case RECUPERAR_STATE:
      {
        lcd.print("Recuperar!");
        break;
      }
  }
  int btn = lerBotao();
  if (btn != -1)
  Serial.println(btn);
}

int lerBotao()
{
  int reading = -1;
  if ( !digitalRead(buttonCenter) )
    reading = buttonCenter;
  else if ( !digitalRead(buttonLeft) )
    reading = buttonLeft;
  else if ( !digitalRead(buttonRight) )
    reading = buttonRight;
  else if ( !digitalRead(buttonUp) )
    reading = buttonUp;
  else if ( !digitalRead(buttonDown) )
    reading = buttonDown;
    
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }   
  lastButtonState = reading;
  if ((millis() - lastDebounceTime) > debounceDelay) {
    return reading;
  }
  return -1;
}
