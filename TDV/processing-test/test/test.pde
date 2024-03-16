import controlP5.*;
import processing.serial.*;

ControlP5 cp5;
int slider1;

Serial myPort;
int available_serialport = 1; // シリアル検索プログラムで調べたシリアルポートの番号に変更
String arduinoPort = Serial.list()[available_serialport]; //

void setup() {
	printArray(Serial.list());

	size(960,540);
  frameRate(60);

	myPort = new Serial(this, arduinoPort, 115200); // シリアルポートの設定
	cp5 = new ControlP5(this);

	cp5.addSlider("slider1")
		.setPosition(50,150)
		.setSize(860,150)
		.setRange(0,255)
		.setValue(127)
		// .setRange(0,59)
		// .setValue(30)
	;
}

int count = 0;
void draw() {
  count++;
	if(count>=60)count-=60;
	colorMode( HSB ); 
	background(slider1,200,250); // 背景色をスライダーの値に変更

	// fill(slider1);
	// rect(320,100,30,20);

	if(count%3 == 0){ //20FPS
		myPort.write(slider1);
		print(slider1);
	}
}
