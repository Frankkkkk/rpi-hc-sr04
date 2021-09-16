/*
 *  hc-sr04.c:
 *	Simple test program to test the wiringPi functions
 */

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int TRIG, ECHO;

float ping()
{
	unsigned long ping      = 0;
	unsigned long pong      = 0;
	float distance = 0;
 	long timeout   = micros() + 1000000; // 0.5 sec ~ 171 m
	
	
	// Ensure trigger is low.
	digitalWrite(TRIG, LOW);
	delay(50);
	
	// Trigger the ping.
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10); 
	digitalWrite(TRIG, LOW);
	
	// Wait for ping response, or timeout.
	while (digitalRead(ECHO) == LOW && micros() < timeout) {
	}
	ping = micros();
	
	// Wait for pong response, or timeout.
	while (digitalRead(ECHO) == HIGH && micros() < timeout) {
	}
	pong = micros();
	
	// Convert ping duration to distance.
	distance = (float) (pong - ping) * 0.017150;


	return distance;
	
//	printf("%.2f\n", distance);
	
//	return 1;
}

float median(float *x, int n) {
    float temp;
    int i, j;
    // the following two loops sort the array x in ascending order
    for(i=0; i<n-1; i++) {
        for(j=i+1; j<n; j++) {
            if(x[j] < x[i]) {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    if(n%2==0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        return((x[n/2] + x[n/2 - 1]) / 2.0);
    } else {
        // else return the element in the middle
        return x[n/2];
    }
}

int main (int argc, char *argv[])
{

	TRIG = 4;
	ECHO = 5;
	
	if (wiringPiSetup () == -1) {
		exit(EXIT_FAILURE);
	}

	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);

	int num_tries = 400;
	float *tries = malloc(sizeof(*tries) * num_tries);
	for(int i = 0; i < num_tries; i++) {
		float d = 0;
		while(d < 7) {
			delay(15);
			d = ping();
		}

		tries[i] = d;
		//printf("%.2f\n", tries[i]);
	}

	printf("%.2f\n", median(tries, num_tries));
		
	
	return 0;
}

