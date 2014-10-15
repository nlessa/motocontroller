#include <LiquidCrystal.h>

//Variaveis Receita
const int N_RAMPAS = 2;
const int T_AGUA_INI = 55;
const int T_AGUA_TROCA_PANELA = 78;
const int T_AGUA_TROCA_FERMENTADOR = 50;
const int T_FIM_WIRL_PULL = 50;
const long TEMPO_RAMPAS[]   = {5000,5000};//1320000;
const int TEMPERATURA_RAMPAS[]   = {65,72};
const long TEMPO_FERVURA = 10000;
const long TEMPO_LUPULOS[]   = {9000,5000};
const long TEMPO_WHIRLFLOC = 5000;

//Portas
const int buttonCenter = A0;
const int buttonLeft   = A2;
const int buttonRight  = A4;
const int buttonUp     = A1;
const int buttonDown   = A3;

const int R1 = 10;
const int R2 = 11;
const int R3 = 12;

const int T1 = 1;
const int T2 = 2;
const int T3 = 3;

const int BOMBA = 13;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//Constantes Estados
const int INIT_STATE            = 0; 
const int YES_NO_STATE          = 1; 
const int AQUECER_AGUA_STATE    = 2; 
const int RECUPERAR_STATE       = 3; 
const int OK_STATE              = 4; 
const int INGREDIENTES_STATE    = 5;
const int STOP_STATE            = 6; 
const int AQUECER_AGUA2_STATE   = 7; 
const int AQUECER_AGUA3_STATE   = 8; 
const int AQUECER_AGUA4_STATE   = 9; 
const int MANTER_TEMPERATURA_STATE = 11; 
const int BRASSAGEM_STATE       = 12; 
const int FIM_STATE       = 13; 
const int SUBIR_TEMPERATURA_STATE       = 14; 
const int TESTE_DE_IODO_STATE       = 15; 
const int BRASSAGEM_IODO_STATE       = 16;
const int PREPARAR_FERVURA_STATE       = 17; 
const int PREPARAR_FERVURA2_STATE       = 18; 
const int PREPARAR_FERVURA3_STATE       = 19; 
const int PREPARAR_FERVURA4_STATE       = 20; 
const int FERVURA_STATE       = 21; 
const int CONTA_TEMPO_STATE       = 22; 
const int WHIRLFLOC_STATE       = 23; 
const int LUPULO_STATE       = 24; 
const int LUPULO2_STATE       = 25;
const int WHIRLFLOC2_STATE    = 26;
const int WHIRLFLOC3_STATE    = 27;
const int FERMENTADOR_STATE    = 28;






//Constantes
const long DEBOUNCE_DELAY   = 50; 
const long TEMPO_IODO = 10000;

//Variáveis
long lastDebounceTime = 0;  
long lastIntervalTime = 0;  
long tempoAlvo;
long lastLCD = 0;;
int state = INIT_STATE;
int lastButtonState = -1;
int lcdSelected = 1;
int temperaturaAlvo;
int simAction;
int noAction;
int rampa_atual = 0;
boolean esquentaLavagem = false;
int lupulo_atual = 0;


void setup() {
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(BOMBA, OUTPUT);
  
  pinMode(buttonCenter, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  
  lcd.begin(20, 4);
  Serial.begin(9600);
}

void loop() {
  if (esquentaLavagem)
  {
    int temper2 = lerTemperatura(T2);
    if ( temper2 < T_AGUA_TROCA_PANELA )
    {
       digitalWrite(R2, HIGH);
    }
    else
    {
       digitalWrite(R2, LOW);
    }
  }
  switch (state)
  {
      case INIT_STATE:
      {
        lcd.print("Deseja reiniciar?");
        lcd.setCursor(0, 3);
        lcd.print("<SIM>  NAO");
        simAction = INGREDIENTES_STATE;
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
                lcd.setCursor(0, 3);
                lcd.print(" SIM  <NAO>");
            }
            else
            {
                lcd.setCursor(0, 3);
                lcd.print("<SIM>  NAO ");
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
      case OK_STATE:
      {
        int btn = lerBotao();
        if ( btn == buttonCenter)
        {
          state = simAction;
        }
        break;
      }
      case INGREDIENTES_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Add agua/malte");
        lcd.setCursor(0, 3);
        lcd.print("<OK>");
        simAction = AQUECER_AGUA_STATE;
        state = OK_STATE;
        break;
      }
      case AQUECER_AGUA_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Aquecendo ");
        lcd.print(T_AGUA_INI);
        lcd.print("c!");
        digitalWrite(R2, HIGH);
        state = AQUECER_AGUA2_STATE;
        break;
      }
      case AQUECER_AGUA2_STATE:
      {
        lcd.setCursor(0, 1);
        int temper = lerTemperatura(T2);
        lcd.print(temper);
        if ( temper > T_AGUA_INI )
        {
          digitalWrite(R2, LOW);
          simAction = AQUECER_AGUA3_STATE;
          state = OK_STATE;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Abrir V1!");
          lcd.setCursor(0, 3);
          lcd.print("<OK>");
        }
        break;
      }
      case AQUECER_AGUA3_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Esvaziou PA?");
        lcd.setCursor(0, 3);
        lcd.print("<OK>");
        simAction = AQUECER_AGUA4_STATE;
        state = OK_STATE;
        break;
      }
      case AQUECER_AGUA4_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Abrir V2 e V4");
        lcd.setCursor(0, 1);
        lcd.print("Fechar V1 ");
        lcd.setCursor(0, 2);
        lcd.print("Encher PA");
        lcd.setCursor(0, 3);
        lcd.print("<OK>");
        simAction = BRASSAGEM_STATE;
        state = OK_STATE;
        break;
      }
      case BRASSAGEM_STATE:
      {
        esquentaLavagem = true;
        if ( rampa_atual < N_RAMPAS )
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("RAMPA");
          lcd.print(rampa_atual+1);
          lcd.print(" T=");
          lcd.print(TEMPERATURA_RAMPAS[rampa_atual]);
          lcd.print("C");
          digitalWrite(R1, HIGH);
          digitalWrite(BOMBA, HIGH);
          temperaturaAlvo = TEMPERATURA_RAMPAS[rampa_atual];
          tempoAlvo = TEMPO_RAMPAS[rampa_atual];
          simAction = BRASSAGEM_STATE;
          state = SUBIR_TEMPERATURA_STATE;
          rampa_atual++;
        }
        else
        {
          state = TESTE_DE_IODO_STATE;
        }
        break;
      }
      case BRASSAGEM_IODO_STATE:
      {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("RAMPA");
          lcd.print(N_RAMPAS);
          lcd.print(" T=");
          lcd.print(TEMPERATURA_RAMPAS[N_RAMPAS-1]);
          lcd.print("C");
          digitalWrite(R1, HIGH);
          digitalWrite(BOMBA, HIGH);
          temperaturaAlvo = TEMPERATURA_RAMPAS[N_RAMPAS-1];
          tempoAlvo = TEMPO_IODO;
          lastIntervalTime = millis();
          simAction = TESTE_DE_IODO_STATE;
          state = MANTER_TEMPERATURA_STATE;
        break;
      }
      case TESTE_DE_IODO_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Passou no teste");
        lcd.setCursor(0, 1);
        lcd.print("de Iodo?");
        lcd.setCursor(0, 3);
        lcd.print("<SIM>  NAO");
        simAction = PREPARAR_FERVURA_STATE;
        noAction = BRASSAGEM_IODO_STATE;
        state = YES_NO_STATE;
        lcdSelected = 0;
        break;
      }      
      case PREPARAR_FERVURA_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Subindo Temp = ");
        lcd.print(T_AGUA_TROCA_PANELA);
        lcd.print("C!");
        digitalWrite(R1, HIGH);
        state = PREPARAR_FERVURA2_STATE;
        break;
      }      
      case PREPARAR_FERVURA2_STATE:
      {
        int temper = lerTemperatura(T1);
        if ( temper < T_AGUA_TROCA_PANELA )
        {
           digitalWrite(R1, HIGH);
        }
        else
        {
           digitalWrite(R1, LOW);
        }
        int temper2 = lerTemperatura(T2);
        if ( millis() - lastLCD > 1000 )
        {
          lcd.setCursor(0, 1);
          lcd.print("PA - ");
          lcd.print(temper2);
          lcd.print("C    "); 
          lcd.setCursor(0, 2);
          lcd.print("PB - ");
          lcd.print(temper);
          lcd.print("C    ");          
        }
        if ( temper >= T_AGUA_TROCA_PANELA && temper2 >= T_AGUA_TROCA_PANELA )
        {
          digitalWrite(R1, LOW);
          digitalWrite(R2, LOW);
          esquentaLavagem = false;
          digitalWrite(BOMBA, LOW);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fechar V4");
          lcd.setCursor(0, 1);
          lcd.print("Abrir V6 e V1 ");
          lcd.setCursor(0, 3);
          lcd.print("<OK>");
          simAction = PREPARAR_FERVURA3_STATE;
          state = OK_STATE;
        }
        break;
      }
      case PREPARAR_FERVURA3_STATE:
      {
        digitalWrite(BOMBA, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Esvaziou PB?");        
        lcd.setCursor(0, 3);
        lcd.print("<OK>");
        simAction = PREPARAR_FERVURA4_STATE;
        state = OK_STATE;
        break;
      }
      case PREPARAR_FERVURA4_STATE:
      {
        digitalWrite(BOMBA, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Fechar V1,V2 e V6");        
        lcd.setCursor(0, 3);
        lcd.print("<OK>");
        simAction = FERVURA_STATE;
        state = OK_STATE;
        break;
      }
      case FERVURA_STATE:
      {
        digitalWrite(R3, HIGH);
        int temper = lerTemperatura(T3);
                  
        if ( millis() - lastLCD > 1000 )
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Esquentando!");
          lcd.setCursor(0, 1);
          lcd.print(temper);
          lcd.print("C    ");
          lastLCD = millis();
        }
        if ( temper > 99 )
        {
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print("Fervendo!");
           state = CONTA_TEMPO_STATE;
           lastIntervalTime = millis();
           tempoAlvo = TEMPO_FERVURA;
           simAction = LUPULO_STATE;
        }    
        break;
      }
      case LUPULO_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Adicionar Lupulo!");
        lcd.setCursor(0, 3);
        lcd.print("<OK>");
        simAction = LUPULO2_STATE;
        state = OK_STATE;
        break;
      }
      case LUPULO2_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Fervendo2!");
        state = CONTA_TEMPO_STATE;
        lastIntervalTime = millis();
        tempoAlvo = TEMPO_FERVURA;
        simAction = WHIRLFLOC_STATE;
        break;
      }
      case WHIRLFLOC_STATE:
      {
        digitalWrite(R3, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Coloque o Whirlfloc!");
        lcd.setCursor(0, 1);
        lcd.print("Abrir V3 e V6!");
        lcd.setCursor(0, 3);
        lcd.print("<OK>");
        simAction = WHIRLFLOC2_STATE;
        state = OK_STATE;
        break;
      }
      case WHIRLFLOC2_STATE:
      {
        digitalWrite(BOMBA, HIGH);        
        int temper = lerTemperatura(T3);
        if ( millis() - lastLCD > 1000 )
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Whirlpull   ");
          lcd.print(T_AGUA_TROCA_FERMENTADOR);
          lcd.print("C");
          lcd.setCursor(0, 1);
          lcd.print(temper);
          lcd.print("C    ");
          lastLCD = millis();          
        }
        if ( temper < T_AGUA_TROCA_FERMENTADOR  )
        {
          state = WHIRLFLOC3_STATE;
          lastIntervalTime = millis();
          
        }
        break;
      }
      case WHIRLFLOC3_STATE:
      {
        digitalWrite(BOMBA, LOW);
        long pass = (millis() - lastIntervalTime);
        long tempoFalta = TEMPO_WHIRLFLOC - pass;
        if ( millis() - lastLCD > 1000 )
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Aguardando...");
          lcd.setCursor(0, 1);
          
          int mim = tempoFalta / 60000;
          int seg = (tempoFalta - mim*60000)/1000;
          lcd.print(mim);
          lcd.print(":");
          if ( seg <10 )
            lcd.print(0);
          lcd.print(seg);
          lastLCD = millis();          
        }                
        if ( tempoFalta <= 0 )  
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Abrir V5");
          lcd.setCursor(0, 1);
          lcd.print("Fechar V6"); 
          lcd.setCursor(0, 3);
          lcd.print("<OK>");
          simAction = FERMENTADOR_STATE;
          state = OK_STATE;
        }
        break;
      }
      case FERMENTADOR_STATE:
      {
        digitalWrite(BOMBA, HIGH); 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Esvaziou PF?");
        lcd.setCursor(0, 3);
        lcd.print("<OK>");
        simAction = FIM_STATE;
        state = OK_STATE;
        break;
      }
      case FIM_STATE:
      {
        digitalWrite(BOMBA, LOW); 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("FIM!");
        state = STOP_STATE;
        break;
      }
      case SUBIR_TEMPERATURA_STATE:
      { 
        int temper = lerTemperatura(T1);
        if ( temper < temperaturaAlvo )
        {
           digitalWrite(R1, HIGH);
        }
        else
        {
           state = MANTER_TEMPERATURA_STATE;
           lastIntervalTime = millis();
        }        
        if ( millis() - lastLCD > 1000 )
        {
          lcd.setCursor(0, 1);
          lcd.print(temper);
          lcd.print("C    ");
          int mim = tempoAlvo / 60000;
          int seg = (tempoAlvo - mim*60000)/1000;
          lcd.print(mim);
          lcd.print(":");
          if ( seg <10 )
            lcd.print(0);
          lcd.print(seg);
          lastLCD = millis();
        }
        break;
      }
      case MANTER_TEMPERATURA_STATE:
      {      
        int temper = lerTemperatura(T1);
        if ( temper < temperaturaAlvo )
        {
           digitalWrite(R1, HIGH);
        }
        else
        {
           digitalWrite(R1, LOW);
        }
        long pass = (millis() - lastIntervalTime);
        long tempoFalta = tempoAlvo - pass;        
        if ( tempoFalta <= 0 )  
        {
          state = simAction;
        }
        if ( millis() - lastLCD > 1000 )
        {
          lcd.setCursor(0, 1);
          lcd.print(temper);
          lcd.print("C    ");
          int mim = tempoFalta / 60000;
          int seg = (tempoFalta - mim*60000)/1000;
          lcd.print(mim);
          lcd.print(":");
          if ( seg <10 )
            lcd.print(0);
          lcd.print(seg);
        }
        break;
      }
      case CONTA_TEMPO_STATE:
      {      
        
        long pass = (millis() - lastIntervalTime);
        long tempoFalta = tempoAlvo - pass;        
        if ( tempoFalta <= 0 )  
        {
          state = simAction;
        }
        if ( millis() - lastLCD > 1000 )
        {
          lcd.setCursor(0, 1);
          int mim = tempoFalta / 60000;
          int seg = (tempoFalta - mim*60000)/1000;
          lcd.print(mim);
          lcd.print(":");
          if ( seg <10 )
            lcd.print(0);
          lcd.print(seg);
        }
        break;
      }
      case RECUPERAR_STATE:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Recuperar!");
        state = STOP_STATE;
        break;
      }
      case STOP_STATE:
      {
        break;
      }
  }
}

int lerTemperatura(int termometro)
{
  int sensorValue = analogRead(A5);           
  return map(sensorValue, 0, 1023, 0, 110);
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
  if (-1 != lastDebounceTime && (millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    lastDebounceTime = -1;
    return reading;
  }
  return -1;
}
