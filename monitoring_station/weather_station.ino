#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Seeed_BMP280.h>
#include <Wire.h>
#include <ML8511.h>

#include <SimpleTimer.h>
SimpleTimer timer;// variable timer
int TimerId;// ID du timer
uint16_t payloadTemp;

#define ANALOGPIN A0
#define ENABLEPIN 7
#define AnemometrePin 2 // Anèmomètre sur l'entrée numérique D2 
#define GirouettePin A4 // Girouette entrée analogique A4
#define HautPluiePin 3 // Pluviomètre sur entrée numérique D3

static const u1_t PROGMEM APPEUI[8]={ /* Hidden */ };//lsb
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

static const u1_t PROGMEM DEVEUI[8]={ /* Hidden */ };//lsb
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

static const u1_t PROGMEM APPKEY[16] = { /* Hidden */ };//msb
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

static osjob_t sendjob;

const unsigned TX_INTERVAL = 15;

//// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

union floatToSend{ // mémoire partagée pour recupérer les 4 bytes d'un float. Une union est un agrégat regroupant des objets de types différents, mais ne pouvant en stocker qu’un seul à la fois ici 4 bits
  float f;
  unsigned char c[4];
};

union entierToSend{
  int i;
  unsigned char c[2];  
};

// toutes les variables utilisées dans une interruption doit être déclarées "volatile" (instruction pour le compilateur) 
volatile unsigned long Rotations = 0; // Nombre de rotation de l'anémomètre utilisé dans l'interruption
volatile unsigned long impPluie = 0; // Nombre d'impulsion fournie par le pluiviomètre
volatile unsigned int CompteurTimer = 0;// pour définir la durée de comptage des implusions de l'anèmomètre
//volatile bool DureeTerminee; //passe à vrai quand la durée de comptage est écoulée

volatile unsigned long TempsRebond; // intervalle de temps pour éviter le rebond des implusions sur les entrées 

union floatToSend temp1; //valeur qui contiendra la température
union entierToSend vitVent; //valeur qui contiendra la vitesse du vent * 10 (pour avoir un chiffre après la virgule)
union entierToSend dirVent; // direction du vent de 0 à 360 degrés
float gir(0);         //direction moyenne de la girouette sur 1 min (en degrés)
int nbGir = 0;        //nb d'occurence de mesure Anemo

int UVOUT = A0;
int REF_3V3 = 3.3;

BMP280 bmp280;
float pressure;

ML8511 light(ANALOGPIN, ENABLEPIN);

void setup() {
    Serial.begin(9600);
    Serial.println(F("Starting"));
/*
    pinMode(UVOUT, INPUT);
    pinMode(REF_3V3, INPUT);
*/
    Rotations = 0; // initialise à 0 les rotations
    CompteurTimer = 0 ; // utilisé pour choisir la durée de comptage des impulsions de l'anémomètre 
    //DureeTerminee = false; // initiale le comptage
    //TotalPluie = 0;
    vitVent.i = 0;
    pinMode(AnemometrePin, INPUT); 
    attachInterrupt(digitalPinToInterrupt(AnemometrePin),compte_rotations, FALLING); // interruption sur l'entrée front descendant
    attachInterrupt(digitalPinToInterrupt(HautPluiePin),compte_impPluie, FALLING); // interruption pluviomètre
  
    // initialisation Lorawan
    lmicInit();
    TimerId = timer.setInterval(500, DureeTimer); // exécute la fonction DureTimer toutes les 500 ms;
}

///////////////////////////////////////
// pour le mesure de la vitesse du vent : compte 5 * 500 = 2,5 s
void DureeTimer() {
  CompteurTimer++;
  //Serial.println(Rotations);
  //Serial.println("ok int"); 
  if(CompteurTimer == 6) 
  { 
    //DureeTerminee = true; 
    //Serial.println("timer ok !");
    //vitVent.i = 2*3.14*Rotations*0.08*3.6*10 ; // formule la vitesse du vent *10 en km/h
    vitVent.i = 2*3.14*Rotations*0.08*3.6*10;
    //Serial.print("Vitesse du vent : ");
    float vit = vitVent.i/10.0;
    //Serial.println(vit);
    //Serial.println(Rotations);
    //Serial.println(CompteurTimer);
    // Serial.println(vitVent.c[1]);
    Rotations = 0;
    CompteurTimer = 0;
  }
}  

// Fonction qui compte les rotations et qui est appelée par l'interuption (déclenchée sur l'entrée D2)
void compte_rotations() { 
  if((millis() - TempsRebond) > 20 ) { // pour eviter le rebond . 
    Rotations++; 
    TempsRebond = millis(); 
  } 
}

void compte_impPluie(){
  impPluie++;
}

float getGirouetteAngle(int value) {
    float angle = 0;
    if (value > 280 && value < 290) angle = 180;
    if (value > 240 && value < 250) angle = 202.5;
    if (value > 628 && value < 636) angle = 225;
    if (value > 598 && value < 606) angle = 247.5;
    if (value > 940 && value < 950) angle = 270;
    if (value > 824 && value < 832) angle = 292.5;
    if (value > 884 && value < 892) angle = 315;
    if (value > 700 && value < 710) angle = 337.5;
    if (value > 784 && value < 792) angle = 0;
    if (value > 402 && value < 412) angle = 22.5;
    if (value > 458 && value < 468) angle = 45;
    if (value > 78 && value < 85)   angle = 67.5;
    if (value > 88 && value < 98)   angle = 90;
    if (value > 60 && value < 70)   angle = 112.5;
    if (value > 180 && value < 190) angle = 135;
    if (value > 122 && value < 132) angle = 157.5;
    return angle;
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


////////////////
void loop() {
    timer.run();// lance l'interruption Timer toutes les ....
    timer.enable(TimerId);
    os_runloop_once();
}
