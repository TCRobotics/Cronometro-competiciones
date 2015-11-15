#include <TVout.h> // https://code.google.com/p/arduino-tvout/wiki/Welcome?tm=6
#include <fontALL.h>
#include "BricoLabsLogo.h"
#include "StopWatch.h" // http://playground.arduino.cc/Code/StopWatchClass

TVout TV;
StopWatch sw_millis(StopWatch::MILLIS);

int playButtonIN   = 2;
int pauseButtonIN  = 3;
int stopButtonIN   = 4;
int triggerLoIN    = 5;
int triggerHiIN    = 6;
//videoOUT           7
int player1LOUT    = 8;
//syncOUT            9
int player2IN      = 10;
//audioOUT           11
int player1IN      = 12;
int player2LOUT    = 13;

int state = 0;
int modo = 0;
int prevState = 0;
boolean firstTime = true;
boolean player1Stop = false;
boolean player2Stop = false;
int triggerCount = 0;

long minutes = 0;
long seconds = 0;
long miliseconds = 0;

long milisecondsStart = 0;
long milisecondsValue = 0;

#define INTRO     0
#define READY     1
#define COUNTING  2
#define PAUSED    3
#define STOPPED   4

#define MODONORMAL  0
#define MODOPARTIDO 1
#define MODOSENSOR  2

void setup() {
  TV.begin(PAL);
  pinMode(playButtonIN, INPUT);
  pinMode(pauseButtonIN, INPUT);
  pinMode(stopButtonIN, INPUT);
  
  pinMode(triggerHiIN, INPUT);
  pinMode(triggerLoIN, INPUT);
  
  pinMode(player1IN, INPUT);
  pinMode(player2IN, INPUT);
  
  pinMode(player1LOUT, OUTPUT);
  pinMode(player2LOUT, OUTPUT);
  
  TV.select_font(font8x8);
  
  TV.tone(800);
  TV.delay(200);
  TV.tone(900);
  TV.delay(200);
  TV.tone(1100);
  TV.delay(200);
  TV.noTone();
  
}

void loop() {
   switch(state)
  {
   case INTRO:
     if(firstTime)
     {
       intro();
       //TV.println("BricoLabs Chrono v1.0");

       //TV.print(0,33,"________");
       TV.println("");
       TV.println("Modo:");
       TV.println("");
       TV.println(" *Normal");
       TV.println(" *Partido");
       TV.println(" *Sensor");
       TV.tone(800);
       TV.delay(200);
       TV.noTone();
       firstTime = false;
     }

     if(digitalRead(playButtonIN))  
     {
       TV.clear_screen();
       TV.tone(800);
       TV.delay(200);
       TV.noTone();
       TV.delay(1000);
       modo = MODONORMAL;
       firstTime = true;
       prevState = state;
       state = READY;
       break;
     } 
     if(digitalRead(pauseButtonIN))
     {
       TV.clear_screen();
       TV.tone(900);
       TV.delay(200);
       TV.noTone();
       TV.delay(1000);
       modo = MODOPARTIDO;
       firstTime = true;
       prevState = state;
       state = READY;
       break;
     }
     if(digitalRead(stopButtonIN))
     {
       TV.clear_screen();
       TV.tone(1100);
       TV.delay(200);
       TV.noTone();
       TV.delay(1000);
       modo = MODOSENSOR;
       firstTime = true;
       prevState = state;
       state = READY;
       break;
     }
     break;
   case READY:
     if(firstTime)
     {
       TV.clear_screen();
       intro();
       TV.clear_screen();
       if(modo == MODONORMAL)
       {
         TV.print(0,20,"PREPARADOS,");
         
         TV.print(0,30,"LISTOS...");
         TV.print(6,60,"Pulsa Play");
         TV.print(6,70,"para comenzar!");
         TV.draw_rect(0,55,119,30,WHITE);
       }
       else if(modo == MODOPARTIDO)
       {
         TV.print(0,30,"JUGADOR 1");
         TV.print(6,60,"Pulsa");
         TV.print(6,70,"para comenzar!");
         TV.draw_rect(0,55,119,30,WHITE);
         
         while(!digitalRead(player1IN))
         {
           digitalWrite(player1LOUT, LOW);
           TV.delay(50);
           digitalWrite(player1LOUT, HIGH);
           TV.delay(50);
         }
         TV.tone(300);
         TV.delay(200);
         TV.tone(600);
         TV.delay(300);
         TV.tone(1200);
         TV.delay(1000);
         TV.noTone();
         TV.print(0,30,"JUGADOR 2");

         while(!digitalRead(player2IN))
         {
           digitalWrite(player2LOUT, LOW);
           TV.delay(50);
           digitalWrite(player2LOUT, HIGH);
           TV.delay(50);
         }
         TV.tone(300);
         TV.delay(200);
         TV.tone(600);
         TV.delay(300);
         TV.tone(1200);
         TV.delay(1000);
         TV.noTone();
         TV.clear_screen();
         TV.print(0,30,"ESPERA JUEZ");
         TV.print(6,60,"Pulsa");
         TV.print(6,70,"para comenzar!");
         TV.draw_rect(0,55,119,30,WHITE);
         while(!digitalRead(playButtonIN)){}
         firstTime = true;
         prevState = state;
         state = COUNTING;
         break;
       }
       
       if(modo == MODOSENSOR)
       {
         TV.print(42,0,"Sensor");
         TV.print(0,20,"PREPARADOS,");
         TV.print(0,30,"LISTOS...");
         TV.print(6,60,"Esperando");
         TV.print(6,70,"sensor...");
         TV.draw_rect(0,55,119,30,WHITE);
         TV.delay(1000);
       }
       firstTime = false;
       if(prevState == STOPPED && modo!=MODOSENSOR)
       {
         while(digitalRead(playButtonIN)){}
       } 
     }
 
     if (!digitalRead(triggerLoIN))triggerCount++;
     else triggerCount = 0;
     if(digitalRead(playButtonIN) && modo == MODONORMAL || triggerCount >= 20 )
     {
       firstTime = true;
       triggerCount = 0;
       prevState = state;
       state = COUNTING;
     }
     break;
   case COUNTING:
     if(firstTime)
     {
       TV.tone(800);
       TV.clear_screen();
       if(modo == MODOPARTIDO)
       {
         digitalWrite(player1LOUT, HIGH);
         digitalWrite(player2LOUT, HIGH);
       }
       TV.print(6,20,"TIEMPO:");
       firstTime = false;
       if(prevState != PAUSED)sw_millis.reset();
       sw_millis.start();
     }
     
     miliseconds = (sw_millis.elapsed()) % 1000;
     seconds = ((sw_millis.elapsed()) / 1000) % 60;
     minutes = ((sw_millis.elapsed()) / 1000) / 60;
     if(miliseconds>=500)TV.noTone();

     if(minutes<10){TV.print(6,40,"0");TV.print(14,40,minutes);}
     else TV.print(6,40,minutes);
     TV.print(22,40,":");
     
     if(seconds<10){TV.print(30,40,"0");TV.print(38,40,seconds);}
     else TV.print(30,40,seconds);
     TV.print(46,40,":");
     
     if(miliseconds<10){TV.print(54,40,"00");TV.print(70,40,miliseconds);}
     else
     {
       if(miliseconds<100) {TV.print(54,40,"0");TV.print(62,40,miliseconds);}
       else TV.print(54,40,miliseconds); 
     }
     
     if(digitalRead(pauseButtonIN))
     {
       sw_millis.stop();
       firstTime = true;
       prevState = state;
       state = PAUSED;
     }
     if (digitalRead(player1IN) && modo == MODOPARTIDO)
     {
       sw_millis.stop();
       firstTime = true;
       prevState = state;
       player1Stop = true;
       state = PAUSED;       
     }
     if (digitalRead(player2IN) && modo == MODOPARTIDO)
     {
       sw_millis.stop();
       firstTime = true;
       prevState = state;
       player2Stop = true;
       state = PAUSED;       
     }  
     if (!digitalRead(triggerLoIN) && seconds >= 3)triggerCount++;
     else triggerCount = 0;
     if(triggerCount >= 20 && modo == MODOSENSOR )
     {
       sw_millis.stop();
       firstTime = true;
       triggerCount = 0;
       prevState = state;
       state = PAUSED;
     }
     if(digitalRead(stopButtonIN))
     {
       sw_millis.stop();
       firstTime = true;
       prevState = state;
       state = STOPPED;
     }
     break;
   case PAUSED:
     if(firstTime)
     {
       if(modo == MODOSENSOR)
       {
         TV.tone(300);
         TV.delay(200);
         TV.tone(600);
         TV.delay(300);
         TV.tone(1200);
         TV.delay(1000);
         TV.noTone();
       }
       else
       {
         TV.tone(800);
         TV.delay(500);
         TV.noTone();
       }
       TV.print(6,20,"PAUSA  ");
       TV.print(6,60,"Pulsa Play");
       TV.print(6,70,"para continuar");
       if(player1Stop)
       {
         TV.tone(1100);
         TV.delay(200);
         TV.tone(900);
         TV.delay(200);
         TV.tone(200);
         TV.delay(2000);
         TV.noTone();
         TV.print(6,60,"Jugador 1  ");
         TV.print(6,70,"abandona!     ");
         digitalWrite(player1LOUT, LOW);
         TV.delay(100);
         digitalWrite(player1LOUT, HIGH);
         TV.delay(100);
         digitalWrite(player2LOUT, LOW);
       }
       if(player2Stop)
       {
         TV.tone(1100);
         TV.delay(200);
         TV.tone(900);
         TV.delay(200);
         TV.tone(200);
         TV.delay(2000);
         TV.noTone();
         TV.print(6,60,"Jugador 2  ");
         TV.print(6,70,"abandona!     ");
         digitalWrite(player2LOUT, LOW);
         TV.delay(100);
         digitalWrite(player2LOUT, HIGH);
         TV.delay(100);
         digitalWrite(player1LOUT, LOW);
       }
       TV.draw_rect(0,55,119,30,WHITE);
       firstTime = false;
     }
     if(player1Stop)
       {
         digitalWrite(player1LOUT, LOW);
         TV.delay(100);
         digitalWrite(player1LOUT, HIGH);
         TV.delay(100);
         digitalWrite(player2LOUT, LOW);
       }
       if(player2Stop)
       {
         digitalWrite(player2LOUT, LOW);
         TV.delay(100);
         digitalWrite(player2LOUT, HIGH);
         TV.delay(100);
         digitalWrite(player1LOUT, LOW);
       }
     if(!player1Stop & !player2Stop)
     {
       digitalWrite(player1LOUT, LOW);
       digitalWrite(player2LOUT, LOW);  
     }
     if(digitalRead(playButtonIN))
     {
       firstTime = true;
       prevState = state;
       player1Stop = false;
       player2Stop = false;
       state = COUNTING;
     }
     if(digitalRead(stopButtonIN))
     {
       firstTime = true;
       prevState = state;
       player1Stop = false;
       player2Stop = false;
       state = STOPPED;
     }
     break;
   case STOPPED:
     if(firstTime)
     {
       TV.tone(300);
       TV.delay(200);
       TV.tone(600);
       TV.delay(300);
       TV.tone(1200);
       TV.delay(1000);
       TV.noTone();
       digitalWrite(player1LOUT, LOW);
       digitalWrite(player2LOUT, LOW); 
       TV.print(6,20,"TIEMPO TOTAL:");
       TV.draw_rect(0,55,119,30,BLACK);
       TV.print(6,60,"Pulsa Play");
       TV.print(6,70,"para empezar  ");
       TV.print(6,80,"de nuevo");
       TV.draw_rect(0,55,119,40,WHITE);
       firstTime = false;
     }
     if(digitalRead(playButtonIN))
     {
       firstTime = true;
       prevState = state;
       state = READY;
     }
     break;
  }  
}

void intro() {
unsigned char w,l,wb;
  int index;
  w = 90;
  l = 90;
  if (w&7)
    wb = w/8 + 1;
  else
    wb = w/8;
  index = wb*(l-1) + 2;
  for ( unsigned char i = 1; i < l; i++ ) {
    TV.bitmap((TV.hres() - w-3)/2,0,BricoLabsLogo,index,w,i);
    index-= wb;
    //TV.delay(10);
  }
  for (unsigned char i = 0; i < (TV.vres() - l)/2; i++) {
    TV.bitmap((TV.hres() - w-3)/2,i,BricoLabsLogo);
    //TV.delay(10);
  }
  TV.delay(1000);
  TV.clear_screen();
}



