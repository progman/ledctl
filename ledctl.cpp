//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// 0.0.1
// Alexey Potehin <gnuplanet@gmail.com>, http://www.gnuplanet.ru/doc/cv
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define KEYBOARDDEVICE  "/dev/console"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// get or set or reset or invert keyboard leds
int ledctl(int led_name, bool flag_enable, bool flag_set, bool flag_invert)
{
	int keyboardDevice = open(KEYBOARDDEVICE, O_RDONLY);
	if (keyboardDevice == -1)
	{
		perror("ERROR[open()]");
		return -1;
	}


	unsigned int led_val = 0;
	if (ioctl(keyboardDevice, KDGETLED, &led_val))
	{
		perror("ERROR[ioctl(KDGETLED)]");
		return -1;
	}


	if (flag_set != false)
	{
		if (flag_invert == false)
		{
			if (flag_enable == false)
			{
				led_val &= ~led_name;
			}
			else
			{
				led_val |= led_name;
			}
		}
		else
		{
			if (led_val & led_name)
			{
				led_val &= ~led_name;
			}
			else
			{
				led_val |= led_name;
			}
		}

		if (ioctl(keyboardDevice, KDSETLED, led_val))
		{
			perror("ERROR[ioctl(KDSETLED)]");
			return -1;
		}
	}


	if (led_val & led_name)
	{
		printf("1\n");
	}
	else
	{
		printf("0\n");
	}


	close(keyboardDevice);


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// view help
void help(const char *name)
{
	printf ("example: %s [num|-num|+num|~num|cap|-cap|+cap|~cap|scr|-scr|+scr|~scr]\n", name);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// general function
int main(int argc, char *argv[])
{
	int rc;


	if (argc == 1)
	{
		help(argv[0]);
		return 1;
	}


	struct
	{
		const char *scr_arg;
		int int_arg;
		bool flag_enable;
		bool flag_set;
		bool flag_invert;
	}
	args[] =
	{
		{ "num",  LED_NUM, false, false, false },
		{ "-num", LED_NUM, false, true,  false },
		{ "+num", LED_NUM, true,  true,  false },
		{ "~num", LED_NUM, true,  true,  true  },

		{ "cap",  LED_CAP, false, false, false },
		{ "-cap", LED_CAP, false, true,  false },
		{ "+cap", LED_CAP, true,  true,  false },
		{ "~cap", LED_CAP, true,  true,  true  },

		{ "scr",  LED_SCR, false, false, false },
		{ "-scr", LED_SCR, false, true,  false },
		{ "+scr", LED_SCR, true,  true,  false },
		{ "~scr", LED_SCR, true,  true,  true  }
	};


	bool flag_found = false;
	for (size_t i=0; i < (sizeof(args)/sizeof(args[0])); i++)
	{
		if (strcmp(argv[1], args[i].scr_arg) == 0)
		{
			rc = ledctl(args[i].int_arg, args[i].flag_enable, args[i].flag_set, args[i].flag_invert);
			if (rc == -1) return 1;
			flag_found = true;
			break;
		}
	}
	if (flag_found == false)
	{
		help(argv[0]);
		return 1;
	}


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
