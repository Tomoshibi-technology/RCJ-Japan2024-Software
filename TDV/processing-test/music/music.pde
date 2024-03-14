import ddf.minim.analysis.*;
import ddf.minim.*;

import processing.serial.*;

Minim minim;
AudioPlayer player;
FFT fft;

Serial myPort;
int available_serialport = 1; // シリアル検索プログラムで調べたシリアルポートの番号に変更
String arduinoPort = Serial.list()[available_serialport]; //

int fftSize;
float radious = 150;
float r = 255;
float g = 0;
float b = 255;

void setup() {
 size(1000, 1000);
  frameRate(60);
 minim = new Minim(this);
 player = minim.loadFile("LostSky-Fearless.mp3");
 player.loop();
 fft = new FFT(player.bufferSize(), player.sampleRate());
 colorMode(HSB, 360, 100, 100);

 myPort = new Serial(this, arduinoPort, 115200); // シリアルポートの設定

}

int count = 0;
void draw() {
	  count++;
	if(count>=60)count-=60;

 translate(width / 2, height / 2);
 background(0);
 strokeWeight(20);
 
 fft.forward(player.mix);
 for (int i=0; i<fft.specSize(); i+= 1) {
   float h = map(i, 0, fft.specSize(), 0, 360); //音楽から取得した周波数スペクトルの幅を0～360に変換
   float angle = +map(i, 0, fft.specSize(), 0, 2*PI) ; //放射状に伸びる線を0～2πの間に描写
   float len = fft.getBand(i)*10;
   stroke(h, 100, 100);
   line(radious * cos(angle), radious * sin(angle), (radious + len) * cos(angle), (radious + len) * sin(angle));
 }
 noStroke();
 fill(0);
 ellipse(0, 0, 280, 280);
 fill(360);
 ellipse(0, 0, 170, 170);
  if(count%3==0){
		//低周波の音量を取得
		float low = fft.getBand(0);
		//0-255の範囲に変換
		int lowValue = int(map(low, 0, 50, 0, 255));
		if(lowValue > 255) lowValue = 255;
		//シリアル通信でArduinoに送信
		myPort.write(lowValue);
		println(fft.getBand(0));

	}
}