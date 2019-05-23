/*
Autor: lezzzca
version: 2.1
descripcion: interruptor por aplausos , en esta version se implemento la libreria LOWPOWER para bajar el consumo
energetico, durmiendo el Arduino mientras no se escucha ningun estimulo del sensor de Sonido
*/

#include <LowPower.h>
#define SENSOR_SONIDO 2 // lo asigno a la pata 2 de la interrupcion 0
#define RELE 5
#define VCC_1 6
#define VCC_2 7
#define LED_WHILE 9
#define LED_RELE 10

#define TIME_LIMIT 1000

bool valor = false ;
volatile bool trigerState;
void wakeUp(){
	// solo un handler para la interrupcion
	delay(150); //evita el eco del aplauso
	trigerState= true;
	//Serial.println("se activo la interrupcion!!!");
}

void aplauso(long starTime) {
	
	bool clap2 = false;
	
	if (trigerState) { // si hubo un aplauso q desperto la interrupcion prende el led y continua la funcion
		digitalWrite(LED_WHILE, HIGH);
	}else { exit; } //si no hubo aplauso q desperto la interrupcion sale de la funcion y vuelve al loop
	
	while (millis() - starTime < TIME_LIMIT) {
		valor = digitalRead(SENSOR_SONIDO);
		if (valor) {
		//	 Serial.println("entro al clap2");
			clap2 = true;
			delay(250);
		}
	}
	digitalWrite(LED_WHILE, LOW);

	if (trigerState && clap2 ) {
		digitalWrite(LED_RELE, HIGH);
		digitalWrite(RELE, !digitalRead(RELE));
		delay(250);
		digitalWrite(LED_RELE, LOW);
		delay(1000); // evita rebote del sensor
	}
	else {
		//Serial.println("no hubo 2 aplausos");
	}
}

void setup()
{
	//Serial.begin(9600);
	pinMode(SENSOR_SONIDO, INPUT);
	pinMode(RELE, OUTPUT);
	pinMode(VCC_1, OUTPUT);
	pinMode(VCC_2, OUTPUT);
	pinMode(LED_WHILE, OUTPUT);
	pinMode(LED_RELE, OUTPUT);
	

	digitalWrite(VCC_1, HIGH);
	digitalWrite(VCC_2, HIGH);
	digitalWrite(RELE, LOW);
	digitalWrite(LED_WHILE, LOW);
	digitalWrite(LED_RELE, LOW);
}

void loop()
{
  
	long startTime = millis();
	trigerState = false;
	attachInterrupt(0, wakeUp, RISING);
	LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
	detachInterrupt(0);
	
	aplauso(startTime);
	
}// fin del loop
