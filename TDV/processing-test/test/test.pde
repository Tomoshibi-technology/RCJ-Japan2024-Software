import controlP5.*;
ControlP5 cp5;
int slider1;

void setup() {
	size(400,200);
	cp5 = new ControlP5(this);

	cp5.addSlider("slider1")
		.setPosition(50,100)
		.setSize(200,20)
		.setRange(0,255)
		.setValue(127)
	;
}

void draw() {
	background(100);

	fill(slider1);
	rect(320,100,30,20);
}
