/*
 * chronicle.c
 * 
 * Copyright 2018 graf <g@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

/*
 * log utility
 * */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int log_it(char* logthis, char*logpath) {
	//TODO: check for buffer overflow
	
	char write_buffer[4096] = { 0 };
	char time_stamp[32] = {0};
	char i_temp = 0;
	struct tm *time_information;
	time_t timer;
	
	// get date time now, store in buffer
	time(&timer);
	time_information = localtime(&timer);
	strftime(time_stamp, 32, "%Y.%m.%d %H:%M:%S", time_information);
	
	//printf("%s: %s\n", time_stamp, logthis);
	FILE *fp = fopen(logpath, "ab+");
	if (fp < 1) {
		printf("file IO error: %s", logpath);
		i_temp = -1;
		return i_temp;
		}	
	
	return i_temp;
	}
