#ifndef BNO055_HPP_
#define BNO055_HPP_
#include "math.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"

typedef struct{
	float x,y,z;
}EULAR;

typedef struct{
	float x,y,z,w;
}QUATERNION;


class BNO055{
private:
	unsigned char device_address_;
	I2C_HandleTypeDef i2c_module_;

public:
//	uint8_t ready = 0;
	BNO055(I2C_HandleTypeDef,uint8_t);
	void init();
	QUATERNION get_quaternion();
	EULAR get_eular();
	EULAR get_accel();
	EULAR get_gyro();

};

#endif //BNO055_HPP_
