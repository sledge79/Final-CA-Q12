#include "mbed.h"
#include "LSM6DSLSensor.h"
#include "LSM6DSL_acc_gyro_driver.h"

#define PI 3.141592654

static DevI2C devI2c(PB_11, PB_10);
static LSM6DSLSensor acc_gyro(&devI2c, 0xD4, D4, D5); 

PwmOut redLed(PE_3);
PwmOut greenLed(PB_14);
PwmOut blueLed(PC_9);

float compute(int x, int y, int z, int x_ref, int y_ref, int z_ref) {
    float dot_product = x * x_ref + y * y_ref + z * z_ref;
    float magnitude1 = sqrt(x * x + y * y + z * z);
    float magnitude2 = sqrt(x_ref * x_ref + y_ref * y_ref + z_ref * z_ref);
    float angle_radians = acos(dot_product / (magnitude1 * magnitude2));
    float angle_degrees = angle_radians * 180.0 / PI;

    return angle_degrees;
}

int main() {
    uint8_t id;
    int32_t axes[3];
    float res = 0;
    acc_gyro.init(NULL);

    acc_gyro.enable_x();
    acc_gyro.enable_g();

    redLed.period_ms(1);
    greenLed.period_ms(1);
    blueLed.period_ms(1);

    

    while (1) {

        acc_gyro.get_x_axes(axes);
        res = compute(axes[0], axes[1], axes[2], 1, 0, 0);  

        redLed = (90.0 - res) / 90.0;  
        acc_gyro.get_x_axes(axes);
        res = compute(axes[0], axes[1], axes[2], 0, 1, 0);  
        
        greenLed = (90.0 - res) / 90.0;  
        acc_gyro.get_x_axes(axes);
        res = compute(axes[0], axes[1], axes[2], 0, 0, 1); 

        
        blueLed = (90.0 - res) / 90.0;  

        
    }
}