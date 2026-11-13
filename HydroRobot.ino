#include "BluetoothSerial.h"
#include <FastLED.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

const byte NUM_LEDS = 8;  
const byte LED_PIN = 2;
const byte BRIGHTNESS = 255;

CRGB leds[NUM_LEDS];

const byte M1CW = 22; 
const byte M1CCW = 23;
const byte M2CW = 19; 
const byte M2CCW = 18;
const byte ENA = 13;
const byte ENB = 12;

#define MAX_VEL 140
#define MIN_VEL 100

int vel = MIN_VEL;  // Variável para armazenar a velocidade

char comando = '\0'; // Inicializa com valor nulo

void setup() {
  Serial.begin(921600);
  SerialBT.begin("Super hydro"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with Bluetooth!");

  pinMode(M1CW, OUTPUT);
  pinMode(M1CCW, OUTPUT);
  pinMode(M2CW, OUTPUT);
  pinMode(M2CCW, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  // Inicializa os LEDs na cor roxa
  set_cores(255, 255, 255);

  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
}

void loop() {
  if (SerialBT.available()) {
    comando = SerialBT.read();
    Serial.println(comando);
  }

  analogWrite(ENA, vel);
  analogWrite(ENB, vel);

  switch (comando) {
    case 'F':
      analogWrite(M1CW, vel);
      analogWrite(M1CCW, 0);
      analogWrite(M2CW, vel);
      analogWrite(M2CCW, 0);
      Serial.println("Frente");
      set_cores(255, 0, 0); // Vermelho
      break;

    case 'R':
      analogWrite(M1CW, 0);
      analogWrite(M1CCW, vel);
      analogWrite(M2CW, vel);
      analogWrite(M2CCW, 0);
      Serial.println("Direita");
      set_cores(0, 0, 255); // Azul
      break;

    case 'L':
      analogWrite(M1CW, vel);
      analogWrite(M1CCW, 0);
      analogWrite(M2CW, 0);
      analogWrite(M2CCW, vel);
      Serial.println("Esquerda");
      set_cores(0, 255, 0); // Verde
      break;

    case 'B':
      analogWrite(M1CW, 0);
      analogWrite(M1CCW, vel);
      analogWrite(M2CW, 0);
      analogWrite(M2CCW, vel);
      Serial.println("Trás");
      set_cores(255, 255, 0); // Amarelo
      break;

    case '0':
      analogWrite(M1CW, 0);
      analogWrite(M1CCW, 0);
      analogWrite(M2CW, 0);
      analogWrite(M2CCW, 0);
      Serial.println("Parado");
      set_cores(255, 255, 255); // Branco
      break;

    case 'T':  // Aumenta a velocidade
      vel = min(vel + 20, MAX_VEL);
      Serial.print("Velocidade aumentada para: ");
      Serial.println(vel);
      set_cores(255, 0, 255); // Roxo
      break;

    case 'X':  // Diminui a velocidade
      vel = max(vel - 20, MIN_VEL);
      Serial.print("Velocidade reduzida para: ");
      Serial.println(vel);
      set_cores(0, 255, 255); // Amarelo
      break;
  }
}

// Define a cor de todos os LEDs
void set_cores(byte R, byte G, byte B) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(R, G, B);
  }
  FastLED.show();
}
