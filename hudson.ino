
#include "LiquidCrystal.h"

#define litrosMin		1
#define litrosMax		999
#define matLitros		425
#define casas			3

#define pinLitrosUp		A1
#define pinLitrosDown	A2
#define pinLitrosMens	A3
#define pinInterrupt	2
#define pinLitrosVal	13



LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int
	contaPulso = 0,
	contaLitro = 0,
	litros = litrosMin;

unsigned long 
	timerSelect = 0,
	timerUpdate = 0;

bool
	commandSelect,
	commandMensure,
	keyUp,
	keyDown,
	keyMens;

void setup(void) {

	lcd.begin(16, 2);

	pinMode(pinLitrosUp,	INPUT_PULLUP);
	pinMode(pinLitrosDown,	INPUT_PULLUP);
	pinMode(pinLitrosMens,	INPUT_PULLUP);

	pinMode(pinInterrupt,	INPUT);

	pinMode(pinLitrosVal,	OUTPUT);
	digitalWrite(pinLitrosVal, LOW);

	commandSelect = LOW;
	commandMensure = LOW;
	keyUp = LOW;
	keyDown = LOW;
	keyMens = LOW;

	attachInterrupt(digitalPinToInterrupt(pinInterrupt), incPulso, RISING);
}

void loop(void) {

	keyUp =		!digitalRead(pinLitrosUp	);
	keyDown =	!digitalRead(pinLitrosDown	);
	keyMens =	!digitalRead(pinLitrosMens	);

	digitalWrite(pinLitrosVal, commandSelect);

	if (commandSelect == LOW && commandMensure == LOW) {
		if (timerSelect < millis()){
			readCommand();
		}
	}
	else {
		if (keyUp == LOW && keyDown == LOW && keyMens == LOW) {
			timerSelect = millis() + 200;
			commandSelect = LOW;
		}
	}

	if (commandMensure == HIGH) {
		readLitros();
	}

	if (timerUpdate < millis()) {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Litros =");
		lcd.setCursor(9, 0);
		lcd.print(arrangeLitros(litros));
		lcd.setCursor(0, 1);
		lcd.print("Medido =");
		lcd.setCursor(9, 1);
		lcd.print(arrangeLitros(contaLitro));
		if (commandMensure == HIGH) {
			lcd.setCursor(15, 0);
			lcd.print("M");
		}
		timerUpdate = millis() + 100;
	}
}

void incPulso(void) {
	contaPulso++;
}

void readLitros(void) {

	if (contaPulso >= matLitros) {
		contaPulso = 0;
		contaLitro ++;
	}

	if (contaLitro >= litros) {
		commandMensure = LOW;
	}
}

void readCommand(void) {

	if (keyUp == HIGH) {
		litros = setLitrosUp(litros);
		commandSelect = HIGH;
	}

	if (keyDown == HIGH) {
		litros = setLitrosDonw(litros);
		commandSelect = HIGH;
	}
	
	if (keyMens == HIGH) {
		contaPulso = 0;
		contaLitro = 0;
		commandMensure = HIGH;
		commandSelect = HIGH;
	}
}

int setLitrosUp(int value) {

	if (value < litrosMax) {
		value++;
	}
	
	return value;
}

int setLitrosDonw(int value) {

	if (value > litrosMin) {
		value--;
	}

	return value;
}

String arrangeLitros(int value) {
	
	String arrange = String(value);

	for (int i = 0; i < (casas + 1)  - arrange.length(); i++) {
		arrange = "0" + arrange;
	}
	
	return arrange;
}