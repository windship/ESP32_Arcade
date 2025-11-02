#include "LGFX_ESP32_S3_ST7789.hpp"
#include <SD.h>
#include <SPI.h>
#include <vector>
#include <algorithm>

// 오디오 관련
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"

LGFX tft;

#define SD_CS    4
#define SD_MOSI  3
#define SD_MISO  1
#define SD_CLK   2
#define BUTTON_PIN 45
#define DEBOUNCE_MS 200

SPIClass spiSD(HSPI);

std::vector<String> movieFolders;
int currentFolderIndex = 0;
int volumePercent = 100;

String videoPath, audioPath;
String folderPath;

const int frameWidth = 240;
const int frameHeight = 240;
const int bytesPerFrame = frameWidth * frameHeight * 2;

File videoFile;
uint8_t* frameBuffer[2];
int currentIndex = 0;

AudioGeneratorWAV *wav = nullptr;
AudioFileSourceSD *file = nullptr;
AudioOutputI2S *out = nullptr;

unsigned long lastButtonPress = 0;

void showError(const String& line1, const String& line2) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(middle_center);
  int cx = tft.width() / 2;
  int cy = tft.height() / 2;
  tft.drawString(line1, cx, cy - 16);
  tft.drawString(line2, cx, cy + 16);
  while (true);
}

bool isButtonPressed() {
  static bool lastState = HIGH;
  bool currentState = digitalRead(BUTTON_PIN);
  if (lastState == HIGH && currentState == LOW) {
    lastState = currentState;
    unsigned long now = millis();
    if (now - lastButtonPress > DEBOUNCE_MS) {
      lastButtonPress = now;
      return true;
    }
  } else if (currentState == HIGH) {
    lastState = HIGH;
  }
  return false;
}

void readConfig() {
  File cfgFile = SD.open("/cfg/config.cfg");
  if (!cfgFile) {
    Serial.println("⚠️ 설정 파일 없음. 기본값 사용");
    return;
  }

  while (cfgFile.available()) {
    String line = cfgFile.readStringUntil('\n');
    line.trim();
    if (line.startsWith("volume")) {
      int eq = line.indexOf('=');
      if (eq > 0) {
        String val = line.substring(eq + 1);
        val.trim();
        int v = val.toInt();
        if (v >= 0 && v <= 100) {
          volumePercent = v;
        }
      }
    }
  }
  cfgFile.close();
}

void scanMovieFolders() {
  File root = SD.open("/movie");
  if (!root || !root.isDirectory()) {
    showError("No /movie", "folder found");
  }

  movieFolders.clear();
  File entry;
  while ((entry = root.openNextFile())) {
    if (entry.isDirectory()) {
      movieFolders.push_back(String(entry.name()));
    }
    entry.close();
  }

  if (movieFolders.empty()) {
    showError("No movie folders", "found");
  }

  std::sort(movieFolders.begin(), movieFolders.end());
}

void setupPaths() {
  String movieFolder = movieFolders[currentFolderIndex];
  folderPath = "/movie/" + movieFolder;
  videoPath = folderPath + "/" + movieFolder + ".raw";
  audioPath = folderPath + "/" + movieFolder + ".wav";

  if (!SD.exists(videoPath)) {
    showError("No movie file", movieFolder);
  }
}

void startPlayback() {
  setupPaths();

  if (videoFile) videoFile.close();
  videoFile = SD.open(videoPath);
  if (!videoFile) {
    showError("Failed to open", "video file");
  }

  if (wav) { wav->stop(); delete wav; wav = nullptr; }
  if (file) { delete file; file = nullptr; }
  if (out) { delete out; out = nullptr; }

  file = new AudioFileSourceSD(audioPath.c_str());
  out = new AudioOutputI2S();
  out->SetPinout(8, 9, 7);  // MAX98357A: BCLK, LRC, DIN
  out->SetGain(volumePercent / 100.0f);
  out->SetRate(22050);
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);

  currentIndex = 0;
  size_t bytesRead = videoFile.read(frameBuffer[currentIndex], bytesPerFrame);
  if (bytesRead != bytesPerFrame) {
    showError("Failed to read", "first frame");
  }

  for (int j = 0; j < bytesPerFrame; j += 2) {
    uint8_t tmp = frameBuffer[currentIndex][j];
    frameBuffer[currentIndex][j] = frameBuffer[currentIndex][j + 1];
    frameBuffer[currentIndex][j + 1] = tmp;
  }

  Serial.printf("🎬 재생 시작: %s\n", movieFolders[currentFolderIndex].c_str());
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  tft.init();
  tft.setRotation(2);
  tft.setPivot(0, 0);
  tft.fillScreen(TFT_BLACK);

  spiSD.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spiSD, 40000000)) {
    showError("No SD card", "detected");
  }

  readConfig();
  scanMovieFolders();

  for (int i = 0; i < 2; i++) {
    frameBuffer[i] = (uint8_t*)malloc(bytesPerFrame);
    if (!frameBuffer[i]) {
      Serial.printf("❌ 프레임 버퍼[%d] 할당 실패\n", i);
      while (true);
    }
  }

  startPlayback();
}

void loop() {
  if (wav && wav->isRunning()) {
    wav->loop();
  }

  if (isButtonPressed()) {
    currentFolderIndex = (currentFolderIndex + 1) % movieFolders.size();
    startPlayback();
    return;
  }

  int nextIndex = 1 - currentIndex;
  size_t bytesRead = videoFile.read(frameBuffer[nextIndex], bytesPerFrame);
  if (bytesRead != bytesPerFrame) {
    videoFile.seek(0);
    tft.fillScreen(TFT_BLACK);

    if (wav) { wav->stop(); delete wav; wav = nullptr; }
    if (file) { delete file; file = nullptr; }
    if (out) { delete out; out = nullptr; }

    file = new AudioFileSourceSD(audioPath.c_str());
    out = new AudioOutputI2S();
    out->SetPinout(8, 9, 7);  // MAX98357A: BCLK, LRC, DIN
    out->SetGain(volumePercent / 100.0f);
    out->SetRate(22050);
    wav = new AudioGeneratorWAV();
    wav->begin(file, out);

    bytesRead = videoFile.read(frameBuffer[nextIndex], bytesPerFrame);
    if (bytesRead != bytesPerFrame) {
      showError("Failed to read", "frame after loop");
    }
  }

  for (int j = 0; j < bytesPerFrame; j += 2) {
    uint8_t tmp = frameBuffer[nextIndex][j];
    frameBuffer[nextIndex][j] = frameBuffer[nextIndex][j + 1];
    frameBuffer[nextIndex][j + 1] = tmp;
  }

  tft.pushImage(0, 94, frameWidth, frameHeight, (uint16_t*)frameBuffer[currentIndex]);
  currentIndex = nextIndex;
}
