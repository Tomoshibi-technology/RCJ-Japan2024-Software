import controlP5.*;
import processing.serial.*;

import ddf.minim.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
import ddf.minim.signals.*;
import ddf.minim.spi.*;
import ddf.minim.ugens.*;

AudioPlayer player;
Minim minim;

String title2 = "Faint_Dream.mp3";
String title = "LostSky-Fearless.mp3";
float effect = 0; //波形にエフェクトをかける
float line = 0;
float left;


ControlP5 cp5; 
int slider1;
boolean toggle1 = false;

Serial myPort;
int available_serialport = 1; // シリアル検索プログラムで調べたシリアルポートの番号に変更
String arduinoPort = Serial.list()[available_serialport]; //


void setup() {
	printArray(Serial.list());

	size(960,540);
  frameRate(66);

	myPort = new Serial(this, arduinoPort, 115200); // シリアルポートの設定
	cp5 = new ControlP5(this);

	cp5.addSlider("slider1")
		.setPosition(50,150)
		.setSize(860,150)
		.setRange(1,240)
		.setValue(127)
		// .setRange(0,59)
		// .setValue(30)
	;
	cp5.addToggle("toggle1")
		.setPosition(80,80)
		.setSize(50,20)
		.setValue(false)
		;

	minim = new Minim(this);
  player = minim.loadFile(title,1024);
  // player.play();
}

int count = 0;
long time_count = 0;
boolean startflg = false;
void draw() {
	count++; 
	if(count >1800) count = 0;

	colorMode( HSB ); 
	background(slider1,200,250); // 背景色をスライダーの値に変更

	if(toggle1 && !(startflg)){
		player.play();
  }
	 
	if(toggle1){

    fill(color(255));
		if(count%9 == 0){
			time_count++;
			print("time_count: ");
			println(time_count);
			if((time_count+7)%8==0)println("---------------------------------------------");
		}
  }else{
		fill(color(128));
	}
  rect(180,80,20,20);
	
	// print("count: ");
	// print(count);	

	if(count%3 == 0){ //20FPS
		myPort.write(250);
		myPort.write(30); //1         
		myPort.write(byte(time_count/240 + 5)); //2
		myPort.write(byte(time_count%240 + 5)); //3
		myPort.write(slider1); //4
	}
	if(myPort.available() > 0){
		println(myPort.read());
	}

	for(int i = 0; i < player.left.size()-1; i++){
    line = abs(player.left.get(i));
    left = player.left.get(i);
    // line(i+effect, 100 + left*amp, i, 100 + player.left.get(i+1)*amp);    // Lチャンネルの波形
  }
  stroke(line*400,line*1000,500); //線の色
  strokeWeight(line*4); //線の太さ
}

