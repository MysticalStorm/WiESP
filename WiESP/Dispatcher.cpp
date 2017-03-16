#include "Dispatcher.h"
#include "SoftwareSerial.h"

SoftwareSerial ArduinoSerial(5, 4, false, 256);

void DispatcherClass::init() {
	ArduinoSerial.begin(115200);

	dataBufer = new char[254];
}

void DispatcherClass::update() {
	int size = ArduinoSerial.available();
	if (size > 0) {
		SerialPrint("Arduino send", size);
		read(size);
	}

	/*
	int size = ArduinoSerial.available();
	if (size > 0) {

		char *data = new char[size];
		for (byte i = 0; i < size; i++) {
			int value = ArduinoSerial.read();
			data[i] = value;
			
			Serial.print("This is byte - ");
			Serial.print(value);
			Serial.println("");
		}
		Serial.print("Size -- "); Serial.print(size); Serial.println("");
		
		for (byte i = 0; i < 10; i++) {
			Serial.println(data[i]);
		}

		delete data;
	}
	*/

	/*
	char a[5] = "1234";
	ArduinoSerial.write(a);
	*/
}

void DispatcherClass::didNewData(CallBack fn) {
	_callBack = fn;
}

void DispatcherClass::read(int size) {
	char *data = new char[size];
	for (byte i = 0; i < size; i++) {
		int value = ArduinoSerial.read();
		data[i] = value;
		//SerialPrint("Byte ", value);
		SerialPrint("Char ", data[i]);
	}
	delay(500);
	validateResponse(data, size);

	delete data;
}

void DispatcherClass::validateResponse(char *data, int size) {
	if (!hasData) {
		int writingIndex = 0;
		for (int i = 0; i < size; i++) {
			hasData = true;
			if (collectData(data, i)) {
				parseResponse();
				dataSize = -1;
				hasData = false;
				SerialPrint("End found in first message", 0);
			}
			dataSize++;
		}
	} else {
		for (int i = dataSize; i < dataSize + size; i++) {
			hasData = true;
			if (collectData(data, i)) {
				parseResponse();
				dataSize = -1;
				hasData = false;
				SerialPrint("End found in last message", 0);
			}
			dataSize++;
		}
	}
	SerialPrint("Total data", dataSize);
}

bool DispatcherClass::collectData(char *data, int element) {
	dataBufer[dataSize] = data[element];
	if (data[element] == ';') {
		return true;
	}
	return false;
}

Orientation DispatcherClass::parseResponse() {

	SerialPrint("Data collected", dataSize);

	callBackRawData.clear();
	for (int i = 0; i < dataSize; i++) {
		callBackRawData.push_back(dataBufer[i]);
		//SerialPrint("Parse tick", i);
		//SerialPrint("Parse char", dataBufer[i]);
	}
	//Serial.println(callBackRawData);
	SerialPrint("Result", callBackRawData);
	//_callBack();
}

void DispatcherClass::write() {

}

void DispatcherClass::SerialPrint(String message, int argument) {
	Serial.print(message);
	Serial.print(" - ");
	Serial.print(argument);
	Serial.println();
}

void DispatcherClass::SerialPrint(String message, char argument) {
	Serial.print(message);
	Serial.print(" - ");
	Serial.print(argument);
	Serial.println();
}

void DispatcherClass::SerialPrint(String message, std::vector<char> argument) {
	Serial.print(message);
	Serial.print(" - ");

	int length = argument.size();
	//SerialPrint("String len", length);
	for (int i = 0; i < length; i++) {
		Serial.print(argument[i]);
	}
	Serial.println();
}

