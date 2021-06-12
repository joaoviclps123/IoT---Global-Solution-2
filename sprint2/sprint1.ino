#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT);

int Vo;
float R1 = 10000;
float logR2, R2, L2, T, Tc, Tf, temp, light;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; 
bool toggle = true;

int ldr = A0;
int tmp = A1;
int btnA = 2;

void setup(){
	Serial.begin(9600);
	pinMode(tmp, INPUT);
	pinMode(btnA, INPUT_PULLUP);
	oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void loop() {
	
	if (digitalRead(btnA) == LOW){
		toggle = !toggle;
	}
	if (toggle == true){
		temp = readTemp(tmp);
 		printTempOled(temp);
		printTempSerial(temp);
	}
	else{
		light = readLight(ldr);
		printLightOled(light);
		printLightSerial(light);
	}
	delay(250);	
}


float readTemp(int tmp){
	Vo = analogRead(tmp);
	R2 = R1 * (1023.0 / (float)Vo - 1.0);
	logR2 = log(R2);
	T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
	Tc = T - 273.15; 
	return Tc;
}


void printTempSerial(float Tc){
	Serial.print("Temp.: ");
	Serial.print(Tc);
	Serial.println(" C");
}

void printTempOled(float Tc){
	oled.clearDisplay();
	oled.setTextSize(1);
	oled.setTextColor(SSD1306_WHITE);
	oled.setCursor(0, 0); 
	oled.print("Temp.: ");
	oled.print(Tc);
	oled.print(" C");
	oled.display();
}

/* 
  Função de aplicação da fórmula LDR
*/
float readLight(int ldr) {
	int adc = analogRead(ldr);
	float rDark = 100000;
	float light, RLDR, Vout;
	float alfa = 1/0.8582;
	
	Vout = (adc * 0.0048828125);
	RLDR = (10000.0 * (5 - Vout))/Vout; 
	light = pow((rDark/RLDR), alfa);
	return light;
}

/* 
  Função que mostra no display a luminosidade
*/
void printLightOled(float light){
	oled.clearDisplay();
 	oled.setTextSize(1);
	oled.setTextColor(SSD1306_WHITE);
	oled.setCursor(0, 0); 
	oled.print("Lumin.: ");
	oled.print(light);
	oled.print(" lux");
	oled.display();
}

/* 
  Função que mostra no serial a luminosidade
*/
void printLightSerial(float light){
	Serial.print("Lumin.: ");
	Serial.print(light);
	Serial.println(" lux");
}
