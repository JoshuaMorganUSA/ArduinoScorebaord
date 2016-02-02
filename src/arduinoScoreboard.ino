
//PWM FET Pins
int led_red = 3;
int led_green = 5;
int led_blue = 6;


//595 Pins
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int latch2 = 10;
int clock2 = 9;
int data2 = 7;


//color stuff
int currentred = 0;
int currentgreen = 0;
int currentblue = 0;
int firstrun = 1;



//Input stuff
String inputString = "";
boolean stringComplete = false;

//holds the input parameters
String digit1 = "";
String digit2 = "";
String incolor_red = "";
String incolor_green = "";
String incolor_blue = "";


int leds[] = {led_red, led_green, led_blue};

void parseInput(String instring);
int get7seg(String input);
void writedisplay(int char1, int char2);
void writergb(int red, int green, int blue);


void setup() {
	
	//595 pins
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	digitalWrite(clockPin, LOW);

	digitalWrite(latchPin, HIGH);
	
	pinMode(latch2, OUTPUT);
	pinMode(data2, OUTPUT);
	pinMode(clock2, OUTPUT);
	
	
	
	Serial.begin(9600);
	Serial.println("START");
	
	
	writedisplay(get7seg(""), get7seg(""));
	
  for(int i = 0; i < 3; i++)
  {
	  pinMode(leds[i], OUTPUT);
	  digitalWrite(leds[i], HIGH);
  }
 
}



void loop() {
	
	
	if(stringComplete)
	{	
		
		int redval = incolor_red.toInt();
		int greenval = incolor_green.toInt();
		int blueval = incolor_blue.toInt();
		
		writergb(redval, greenval, blueval);
		writedisplay(get7seg(digit1), get7seg(digit2));
		
		//Serial.println(inputString);
		inputString = "";
		stringComplete = false;
	}
	
	
}




//Runs between "void loop" executions. Special interrupt Arduino function
void serialEvent() {
	while (Serial.available()) {
		// get the new byte:
		char inChar = (char)Serial.read();
		// add it to the inputString:
		inputString += inChar;
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '!') {
			parseInput(inputString);
			stringComplete = true;
		}
	}
}


//Parses the input string for the various parameters
void parseInput(String instring)
{
	//Clear the existing parameters
	digit1 = "";
	digit2 = "";
	incolor_red = "";
	incolor_green = "";
	incolor_blue = "";
	
	int i = 0;
	//Get the first character 
	while(i < instring.length())
	{
		char currentChar = instring[i];
		i++;
		if(currentChar == ',')
			break;
		else
			digit1 += currentChar;
		
	}
	
	//Get the second Character	
	while(i < instring.length())
	{
		char currentChar = instring[i];
		i++;
		if(currentChar == ',')
			break;
		else
			digit2 += currentChar;
	
	}
	
	//Get the red value
	while(i < instring.length())
	{
		char currentChar = instring[i];
		i++;
		if(currentChar == ',')
			break;
		else
			incolor_red += currentChar;

	}
	
	//Get the green value
	while(i < instring.length())
	{
		char currentChar = instring[i];
		i++;
		if(currentChar == ',')
			break;
		else
			incolor_green += currentChar;
	}
	
	//Get the blue value
	while(i < instring.length())
	{
		char currentChar = instring[i];
		i++;
		if(currentChar == ',')
			break;
		else
			incolor_blue += currentChar;
	}
	
}


int get7seg(String input)
{
	if		(input == "0" || input == "O")
		return 0b11111100;
	else if	(input == "1")
		return 0b01100000;
	else if	(input == "2")
		return 0b11011010;
	else if (input == "3")
		return 0b11110010;
	else if (input == "4")
		return 0b01100110;
	else if (input == "5" || input == "S")
		return 0b10110110;
	else if (input == "6" || input == "G")
		return 0b10111110;
	else if (input == "7")
		return 0b1110000;
	else if (input == "8")
		return 0b11111110;
	else if (input == "9")
		return 0b1111011;
	else if (input == "A")
		return 0b1110111;
	else if (input == "C")
		return 0b1001110;	
	else if (input == "F")
		return 0b1000111;
	else if (input == "U")
		return 0b01111100;
	else if (input == "K")
		return 0b00001101;
	else if (input == "T")
		return 0b0001111;
	else if (input == "DEGREE")
		return 0b1100011;
	else if (input == "")
		return 0x00;
}



void writedisplay(int char1, int char2)
{
	int data = (char1 << 8) | (char2);
	
	
	int redvalue = currentred;
	int greenvalue = currentgreen;
	int bluevalue = currentblue;
	delayMicroseconds(500);
	digitalWrite(latchPin,LOW);
	writergb(0,0,0);
//	delay(1);
	delayMicroseconds(500);
	shiftOut(dataPin, clockPin, LSBFIRST, data);
	shiftOut(dataPin, clockPin, LSBFIRST, (data >> 8));	
	delayMicroseconds(500);
	digitalWrite(latchPin, HIGH);
	writergb(redvalue, greenvalue, bluevalue);

}


void writergb(int red, int green, int blue)
{
	
	currentred = red;
	currentgreen = green;
	currentblue = blue;
	analogWrite(led_red,red);
	analogWrite(led_blue, blue);
	analogWrite(led_green, green);
}
