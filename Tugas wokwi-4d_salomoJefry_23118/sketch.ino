#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin sensor PIR
#define PIR_PIN A0

// Pin sensor Ultrasonik
#define TRIG_PIN A1
#define ECHO_PIN A2

// Pin LED
#define LED_MERAH 13
#define LED_KUNING 12
#define LED_HIJAU 11

void setup() {
    Serial.begin(9600);
    
    // Inisialisasi OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("Gagal menginisialisasi OLED");
        for (;;);
    }
    display.clearDisplay();
    
    // Inisialisasi pin sensor
    pinMode(PIR_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    // Inisialisasi LED sebagai output
    pinMode(LED_MERAH, OUTPUT);
    pinMode(LED_KUNING, OUTPUT);
    pinMode(LED_HIJAU, OUTPUT);
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout 30ms
    return duration * 0.034 / 2;
}

void loop() {
    bool motionDetected = digitalRead(PIR_PIN);
    long distance = getDistance();

    // Tentukan kondisi jarak
    String kondisi;
    if (distance > 0 && distance < 50) {
        kondisi = "DEKAT";
        digitalWrite(LED_MERAH, HIGH);
        digitalWrite(LED_KUNING, LOW);
        digitalWrite(LED_HIJAU, LOW);
    } else if (distance >= 50 && distance <= 125) {
        kondisi = "SEDANG";
        digitalWrite(LED_MERAH, LOW);
        digitalWrite(LED_KUNING, HIGH);
        digitalWrite(LED_HIJAU, LOW);
    } else {
        kondisi = "JAUH";
        digitalWrite(LED_MERAH, LOW);
        digitalWrite(LED_KUNING, LOW);
        digitalWrite(LED_HIJAU, HIGH);
    }

    // Tampilkan di Serial Monitor
    Serial.print("PIR: ");
    Serial.print(motionDetected ? "Terdeteksi" : "Tidak Terdeteksi");
    Serial.print(" | Jarak: ");
    Serial.print(distance);
    Serial.print(" cm | Objek: ");
    Serial.println(kondisi);

    // Tampilkan di OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("PIR: ");
    display.println(motionDetected ? "Terdeteksi" : "Tidak Terdeteksi");
    display.print("Jarak: ");
    display.print(distance);
    display.println(" cm");
    display.print("Objek: ");
    display.println(kondisi);
    display.display();

    delay(500);
}
