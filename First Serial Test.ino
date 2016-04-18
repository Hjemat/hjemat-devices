int serial = 1;

void setup() {
	Serial.begin(9600);
}

void loop(){
	Serial.print(0x0);
	digitalWrite(7, HIGH);
}