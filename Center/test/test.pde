import controlP5.*;
import processing.serial.*;

import ddf.minim.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
import ddf.minim.signals.*;
import ddf.minim.spi.*;
import ddf.minim.ugens.*;

AudioPlayer performance;
AudioPlayer ready;
AudioPlayer pon;
Minim minim;
FFT fft;

String bgm_performance = "LostSky-Fearless.mp3";
String bgm_ready = "Faint_Dream.mp3";
String bgm_pon = "button.mp3";


ControlP5 cp5; 
int slider1;
boolean toggle1 = false;

Serial myPort;


void setup() {
	printArray(Serial.list());

	// size(1920,1080);
	fullScreen();
  frameRate(66);

	int available_serialport = 1; // シリアル検索プログラムで調べたシリアルポートの番号に変更s
	String arduinoPort = Serial.list()[available_serialport];
	myPort = new Serial(this, arduinoPort, 115200); // シリアルポートの設定
	
	cp5 = new ControlP5(this);

	cp5.addSlider("slider1")
		.setPosition(80,30)
		.setSize(1350,50)
		.setRange(1,254)
		.setValue(127)
    .setColorForeground(color(20,10,100))   // バー色
    .setColorActive(color(10,20,230))   // マウス選択色
  ;

	cp5.addToggle("toggle1")
		.setPosition(80,100)
		.setSize(500,50)
		.setValue(false)
	;

	minim = new Minim(this);
  performance = minim.loadFile(bgm_performance,1024);
	ready = minim.loadFile(bgm_ready,1024);
	pon = minim.loadFile(bgm_pon,1024);

	fft = new FFT(performance.bufferSize(), performance.sampleRate());
}

boolean startflg = false;

int start_millis;
int beat_millis = 17; // 1000/66
long pre_millis = 0;

int raw_count = 0;
int beat_count = 0;

void draw() {


	// colorMode( HSB ); 
	// background(slider1,200,250); // 背景色をスライダーの値に変更


	//　時間経過
	if(toggle1 && !(startflg)){
		performance.play();
		start_millis = millis();
		startflg = true;
  }
	if(startflg){
    //fill(color(255));
		int now_millis = millis() - start_millis;
		if(pre_millis + beat_millis < now_millis){
			if(raw_count%32 == 0){
				beat_count++;
				// pon.play();
				// pon.rewind();
				// print("__________________");
				// println(beat_count);
			}
			pre_millis += beat_millis;
			raw_count++;
		}
  }else{
		//fill(color(128));
	}
  //rect(670,100,20,20);

	//Mode選ぶ
	int mode;
	int myHue = 0;
	if(beat_count == 0){
		mode = 0;
		myHue = slider1;
	}else if(beat_count <= 32){
		mode = 1;
		myHue = (beat_count*5 + 20)%255;
	}else if(beat_count <= 80){
		mode = 2;
		myHue = 115;
	}else if(beat_count <= 96){
		mode = 3;
		myHue = 134;
	}else if(beat_count <= 161){
		mode = 4;
		myHue = 180;
	}else if(beat_count <= 192){ // 一つ目のみんな同時
		mode = 5;
		myHue = 20;
	}else if(beat_count <= 257){
		mode = 6;
		myHue = 160;
	}else if(beat_count <= 321){ //全部
		mode = 7;
		myHue = beat_count%255;
	}else if(beat_count <= 354){
		mode = 8;
		myHue = 140;
	}else{
		mode = 9;
		myHue = 140;
	}

	// 色の調整
	if(myHue == 250){
		myHue = 251;
	}

	// 通信
	if(raw_count%3 == 0){ //20FPS
		myPort.write(250);
		myPort.write(mode+5); //1         
		myPort.write(byte(raw_count/240 + 5)); //2
		myPort.write(byte(raw_count%240 + 5)); //3
		myPort.write(myHue); //4
	}



	if(myPort.available() > 0){
		// print(myPort.read());
		// print("______");
	}
	if(raw_count%20 == 0){
		// print(mode);
		// print("___");
		// print(raw_count);
		// print("___");
		// print(myHue);
		// println("___");
	}

	//お絵描き

	colorMode( HSB ); 
	background(myHue,200,250); // 背景色をスライダーの値に変更

	if(mode != 0){
		// rect(0,0,1920,1080);
		// colorMode(RGB, 255);
		// background(0);
		// fft.forward(in.mix);
		
		// noStroke();
		// colorMode(HSB, 360, 100, 100, 255);
		// for(int i = 0; i < fft.specSize(); i++){
		// 	float hue = map(i, 0, fft.specSize(), 0, 360);
		// 	fill(hue, 100, 100, 10);
		// 	float radious = fft.getBand(i) * 5;
		// 	ellipse(width/2, height/2, radious * 2, radious * 2);
		// }
	}
}


