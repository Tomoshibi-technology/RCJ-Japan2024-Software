#include "as5600.h"

AS5600::AS5600(I2C_HandleTypeDef* i2c){
    I2C = i2c;
}

long AS5600::get_travel(){
    HAL_I2C_Master_Transmit(I2C, AS5600_ADDR, &ANGLE_ADDR, 1, 1000);
    HAL_I2C_Master_Receive(I2C, AS5600_ADDR, (uint8_t*)rcvBuf, 2, 1000);
    Angle = rcvBuf[0]*256 + rcvBuf[1];

    if(Angle-pAngle>0 && abs(Angle-pAngle)<4000){
        totalAng = totalAng + (Angle-pAngle);
    }else if(Angle-pAngle<0 && abs(Angle-pAngle)<4000){
        totalAng = totalAng + (Angle-pAngle);
    }else if(Angle-pAngle>0 && abs(Angle-pAngle)>4000){
        totalAng = totalAng - ((4095-Angle)+pAngle);
    }else if(Angle-pAngle<0 && abs(Angle-pAngle)>4000){
        totalAng = totalAng + Angle + (4095-pAngle);
    }else{
        totalAng = totalAng;
    }

    ptotalAng = totalAng;
    pAngle = Angle;

    return (totalAng*15.7)/4095;
}