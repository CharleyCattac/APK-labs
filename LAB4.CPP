






#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define base  1193180
#define MAX_RANDOM  3
#define SHORT_MAX  65535

enum Note {
	a2 = 110,
	ab2 = 117,
	b2 = 123,
	c3 = 131,
	cd3 = 139,
	d3 = 147,
	de3 = 156,
	e3 = 165,
	f3 = 175,
	fg3 = 185,
	g3 = 196,
	ga3 = 208,
	a3 = 220,
	ab3 = 233,
	b3 = 247,
	c4 = 261,
	d4 = 294,
	e4 = 329,
	f4 = 349,
	g4 = 391,
	ga4 = 415,
	a4 = 440,
	ab4 = 455,
	b4 = 466,
	c5 = 523,
	cd5 = 554,
	d5 = 587,
	de5 = 622,
	e5 = 659,
	f5 = 698,
	fg5 = 740,
	g5 = 784,
	ga5 = 830,
	a5 = 880
};

enum Duration{
	eighth = 88,
	quater = 175,
	quaterPlus = 263,
	half = 350,
	whole = 700
};

void printByte(char byte){
	char i;
	char byteBuf[8];
	for (i = 7; i >= 0; i--) {
		byteBuf[i] = (byte % 2) ? '1' : '0';
		byte /= 2;
	}
	for (i = 0; i < 8; i++) {
		printf("%c", byteBuf[i]);
	}
}


void printChannelStatus(char channel) {
	char controlByte;
	char channelPort;
	switch (channel) {
	case 0: {
		controlByte = 0xE2;	//11_10_00_1_0
		channelPort = 0x40;
		break;
	}
	case 1: {
		controlByte = 0xE4;	//11_10_01_0_0
		channelPort = 0x41;
		break;
	}
	case 2: {
		controlByte = 0xE8;	//11_10_10_0_0
		channelPort = 0x42;
		break;
	}
	default: {
		printf("ERROR: Illegal printChannelStatus usage (ivalid channel number %d)\n", channel);
		return;
	}
	}
	outp(0x43, controlByte);
	char state = inp(channelPort);
	printf("The %d-th channel state: ", channel);
	printByte(state);
	printf("\n");
}

void printChannelDiv(short channel) {
	char controlByte;
	char channelPort;
	switch (channel) {
	case 0: {
		controlByte = 0x00;		//11_01_001_0
		channelPort = 0x40;
		break;
	}
	case 1: {
		controlByte = 0x40;		//01_00_000_0 | 11_0_1_010_0
		channelPort = 0x41;
		break;
	}
	case 2: {
		controlByte = 0x80;		//10_00_000_0	| 11_0_1_100_0
		channelPort = 0x42;
		break;
	}
	default: {
		printf("ERROR: Illegal printChannelDiv usage (ivalid chanel number %d)\n", channel);
		return;
	}
	}
	unsigned short int div = 0, divHigh = 0, divLow = 0, divMax = 0, i;

	for (i = 0; i < SHORT_MAX; i++) {
		outp(0x43, controlByte);
		divLow = inp(channelPort);
		divHigh = inp(channelPort);
		div = divHigh * 256 + divLow;
		if (div > divMax) {
			divMax = div;
		}
	}
	printf("The %d-th channel divider: \t%hx\n", channel, divMax);
}


void random(long maxNumber) {
	long final = 0, divLow = 0, divHigh = 0;
	long prevState = inp(0x61);

	outp(0x43, 0xB6); // 10_11_010_0
	outp(0x42, maxNumber % 256);
	outp(0x42, maxNumber / 256);

	outp(0x61, inp(0x61) & 0xFC | 0x01);	// disable dynamic, enable tick

	int i = MAX_RANDOM;
	printf("Random numbers from 0 to %ld just for you!\n\n", maxNumber);
	while(i) {
		delay(i);
		divLow = inp(0x42);
		divHigh = inp(0x42);
		final = divHigh * 256 + divLow;
		if (final <= maxNumber) {
			printf("%d\n", final);
			--i;
			}
	}
	outp(0x61, prevState);
}

void playSound(Note note, Duration duration) {
	int kd;
	outp(0x43, 0xB6);  //10_11_011_0

	kd = base / note;

	outp(0x42, kd % 256);
	kd /= 256;
	outp(0x42, kd);

	outp(0x61, inp(0x61) | 0x03);
	delay(duration);

	outp(0x61, inp(0x61) & 0xFC);
	delay(duration / 3);
}

void playStarWars() {
	playSound(d3, eighth);
	playSound(d3, eighth);
	playSound(d3, eighth);
	playSound(g3, half);

	delay(half);

	playSound(d4, quater);
	delay(eighth);
	playSound(c4, eighth);
	playSound(b3, eighth);
	playSound(a3, eighth);
	playSound(g4, half);

	delay(half);

	playSound(d4, quater);
	delay(eighth);
	playSound(c3, eighth);
	playSound(b3, eighth);
	playSound(a3, eighth);
	playSound(g4, half);

	delay(half);

	playSound(d4, quater);
	delay(eighth);
	playSound(c3, eighth);
	playSound(b3, eighth);
	playSound(c3, eighth);
	playSound(a3, whole);

}

void playJB() {
	playSound(e3, quater);
	playSound(e3, quater);
	playSound(e3, half);

	playSound(e3, quater);
	playSound(e3, quater);
	playSound(e3, half);

	playSound(e3, quater);
	playSound(g3, quater);
	playSound(c3, quaterPlus);
	playSound(d3, eighth);
	playSound(e3, whole);
}

void playMusic() {
	char i;
	printf("\nPlaylist:\n");
	printf("1 - Star Wars;\n");
	printf("2 - Jingle Bells;\n");
	printf("\nChoose music to play:\n");

	while(1){
		rewind(stdin);
		i = getch();
		if (i <= '0' || i >= '9') break;
		switch(i){
		case '1': { playStarWars(); break;}
		case '2': { playJB(); break; }
		default: { printf("Wrong input!\n"); break;}
		}
	}
}

void showOptions() {
	printf("Select an option:\n\n");
	printf("\t1 - show timers's channes' states;\n");
	printf("\t2 - show timers's channels' 16-based dividors;\n");
	printf("\t3 - play some music;\n");
	printf("\t4 - generate random number;\n");
	printf("\nOr press any char to exit...\n\n");
}

int main() {
	char key;
	long maxRandom;
	while(1) {
		clrscr();
		rewind(stdin);
		showOptions();
		key = getch();
		switch(key) {
		case '1': {
			printChannelStatus(0);
			printChannelStatus(1);
			printChannelStatus(2);
			break;
			}
		case '2': {
			printChannelDiv(0);
			printChannelDiv(1);
			printChannelDiv(2);
			break;
			}
		case '3': {
			playMusic();
			break;
			}
		case '4': {
			printf("Enter max random number: ");
			if(!scanf("%ld", &maxRandom)) {
				printf("Wrong input! Operation cannot be performed!");
				}
			else {
				random(maxRandom);
				}
			break;
			}
		default: {
			if (key >= '0' && key <= '9') {
				printf("No option with this number! Try again!");
				break;
				}
			return 0;
			}
		}
		printf("\n\nPress any key to continue...");
		getch();
	}

	return 0;
}