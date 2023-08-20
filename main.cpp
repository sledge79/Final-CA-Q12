#include "mbed.h"
#include "LSM6DSLSensor.h"
#include "LSM6DSL_acc_gyro_driver.h"

#define PI 3.141592654

static DevI2C devI2c(PB_11, PB_10);
static LSM6DSLSensor acc_gyro(&devI2c, 0xD4, D4, D5); 

DigitalOut redLed(PE_3);
DigitalOut greenLed(PA_5);
DigitalOut blueLed(PC_9);

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

    while (1) {
        acc_gyro.get_x_axes(axes);
        res = compute(axes[0], axes[1], axes[2], 1, 0, 0);  

        

        acc_gyro.get_x_axes(axes);
        res = compute(axes[0], axes[1], axes[2], 0, 1, 0);  

        greenLed = (res < 45.0);

        acc_gyro.get_x_axes(axes);
        res = compute(axes[0], axes[1], axes[2], 0, 0, 1); 

        blueLed = (res < 45.0);

        wait_us(2000);
    }
}