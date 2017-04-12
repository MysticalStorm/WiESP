// Dispatcher.h

#ifndef _DISPATCHER_h
#define _DISPATCHER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <vector>

struct Orientation
{
	float pitch;
	float yaw;
	float roll;
	float height;
	float pressure;
	float temperature;
};

class DispatcherClass
{
public:

	typedef void(*CallBack) (Orientation);

	void init();
	void update();
	void write();

	void didNewData(CallBack fn);
	CallBack _callBack;

 protected:
	 char *dataBufer;
	 std::vector<char> callBackRawData;
	 int dataSize = 0;
	 bool hasData = false;

	 void read(int size);
	 bool collectData(char *data, int element);
	 void validateResponse(char *data, int size);
	 void parseResponse();

	 void SerialPrint(String message, int argument);
	 void SerialPrint(String message, char argument);
	 void SerialPrint(String message, std::vector<char> argument);
};

#endif

