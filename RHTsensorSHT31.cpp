#include "RHTsensorSHT31.h"

RHTsensorSHT31::RHTsensorSHT31() {
	measurementDuration = 12.5;// For high repeatitivity, 12.5 ms typical according to sensor documentation. 
	_temp = 0;
	_humidity = 0;
}

void RHTsensorSHT31::updateRHT() {
	/* Connect to SHT31 Sensor via I2C to get RH and T
	*  Display values via Serial and BLE
	*/
	int tempMSB = 0;
	int tempLSB = 0;
	int ChecksumT = 0;
	int humidityMSB = 0;
	int humidityLSB = 0;
	int ChecksumH = 0;
	double oldTemp = _temp;
	double oldHumidity = _humidity;
	double tempMeasure = 0;
	double humidityMeasure = 0;

	//Serial.println("Measurement starting");
	//I2C Initialization 
	Wire.begin();
	//Start condition: device address #= 0 as the write bit   
	Wire.beginTransmission(ADDRESS_SHT31); // Open I2C line in write mode

	//16 bits measurement command   
	Wire.write((byte)CMD_MSB);
	Wire.write((byte)CMD_LSB);
	Wire.endTransmission();
	delay(measurementDuration);//Measurement on-going
	 //Measurements

	Wire.requestFrom(ADDRESS_SHT31, 6); // Open the I2C line in send mode
	//Serial.print("The numbers of bits available for retrieval with read() : ");
	//Serial.println(Wire.available());

	if (6 <= Wire.available()) {
		tempMSB = Wire.read();
		tempLSB = Wire.read();
		ChecksumT = Wire.read();
		humidityMSB = Wire.read();
		humidityLSB = Wire.read();
		ChecksumH = Wire.read();
	}
	Wire.endTransmission();

	//Measurement data MSB + LSB
	tempMeasure = tempLSB + (tempMSB << 8);
	//16 bits value to be converted into a physical scale
	_temp = (175 * tempMeasure / (pow(2, 16) - 1)) - 45;
	if (_temp != oldTemp) {
		oldTemp = _temp;
	}
	//Measurement data MSB + LSB
	humidityMeasure = humidityLSB + (humidityMSB << 8);
	//16 bits value to be converted into a physical scale
	_humidity = 100 * humidityMeasure / (pow(2, 16) - 1);
	if (_humidity != oldHumidity) {
		oldHumidity = _humidity;
	}
}

double RHTsensorSHT31::getHumidity(){
  Serial.print("Sensor : Humidity = ");
  Serial.println(_humidity, 10);
	return (_humidity);
}

double RHTsensorSHT31::getTemp() {
  Serial.print("Sensor : Temperature = ");
  Serial.println(_temp, 10);
	return (_temp);
}

void RHTsensorSHT31::reset(){
  Wire.begin();        // join i2c bus
  //16 bits measurement command   
  Wire.write((byte)S_RST) ;
  Wire.endTransmission();
}

