#!/bin/bash

#xset led 3
#setleds -L +caps < /dev/tty7
#tleds

function do_beep
{
	if [ "$(which beep)" != "" ];
	then
		beep -r 3 -f 30 -d 1;
	fi

	for (( i=0; i < 10; i++ ))
	{
		./bin/ledctl ~num &> /dev/null;
		sleep 0.05;
	}
}

do_beep;
