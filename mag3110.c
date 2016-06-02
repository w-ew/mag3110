#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <i2c/smbus.h>

#define MAG3110_I2C_ADDR	(0x0E)
#define MAG3110_CHIP_ID		(0xC4)

/* register enum for mag3110 registers */
enum {
	MAG3110_DR_STATUS = 0x00,
	MAG3110_OUT_X_MSB,
	MAG3110_OUT_X_LSB,
	MAG3110_OUT_Y_MSB,
	MAG3110_OUT_Y_LSB,
	MAG3110_OUT_Z_MSB,
	MAG3110_OUT_Z_LSB,
	MAG3110_WHO_AM_I,

	MAG3110_OFF_X_MSB,
	MAG3110_OFF_X_LSB,
	MAG3110_OFF_Y_MSB,
	MAG3110_OFF_Y_LSB,
	MAG3110_OFF_Z_MSB,
	MAG3110_OFF_Z_LSB,

	MAG3110_DIE_TEMP,

	MAG3110_CTRL_REG1 = 0x10,
	MAG3110_CTRL_REG2,
};

#define MAKEWORD(h,l) ((((h)&0xFF)<<8)|((l)&0xFF))

int main(int argc, char *argv[]) {

	int i2c_device;
	int r;

	struct {
		__s16 x;
		__s16 y;
		__s16 z;
	} mag3110_data;
	__u8 i2c_bytes[6];

	i2c_device = open("/dev/i2c-0", O_RDWR);
	if(i2c_device < 0) return -1;

	r = ioctl(i2c_device, I2C_SLAVE_FORCE, MAG3110_I2C_ADDR);


	r = i2c_smbus_read_byte_data(i2c_device, MAG3110_WHO_AM_I);
	if(r != MAG3110_CHIP_ID) return -1;

	r = i2c_smbus_write_byte_data(i2c_device, MAG3110_CTRL_REG2, 0x80);
	r = i2c_smbus_write_byte_data(i2c_device, MAG3110_CTRL_REG1, 0x01);


	while(1) {
		r = i2c_smbus_read_i2c_block_data(
				i2c_device,
				MAG3110_OUT_X_MSB,
				sizeof(mag3110_data),
				i2c_bytes);

		mag3110_data.x = MAKEWORD(i2c_bytes[0],i2c_bytes[1]);
		mag3110_data.y = MAKEWORD(i2c_bytes[2],i2c_bytes[3]);
		mag3110_data.z = MAKEWORD(i2c_bytes[4],i2c_bytes[5]);

		fprintf(stdout,
				"{\"x\":%d,\"y\":%d,\"z\":%d}\n",
				mag3110_data.x,
				mag3110_data.y,
				mag3110_data.z);
		fflush(stdout);

		usleep(50*1000);
	};

	close(i2c_device);


	return 0;
}
