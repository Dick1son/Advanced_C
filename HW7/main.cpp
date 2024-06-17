#include "move.h"
#include "pumpkin.h"
#include "my_time.h"
#include "logic.h"

void generateMakefile(const char* projectName, const char** sourceFiles, const char** headerFiles, int numSources, int numHeaders) {
	FILE* makefile = fopen("Makefile", "w");

	// Write the common makefile content
	fprintf(makefile, "CC = gcc\nCFLAGS = -Wall\n\n");

	// Write project target and dependency rule
	fprintf(makefile, "%s: ", projectName);
	for (int i = 0; i < numSources; i++) {
		fprintf(makefile, "%s.o ", sourceFiles[i]);
	}
	fprintf(makefile, "\n\t$(CC) -o %s ", projectName);
	for (int i = 0; i < numSources; i++) {
		fprintf(makefile, "%s.o ", sourceFiles[i]);
	}
	fprintf(makefile, "\n\n");

	// Write individual compilation rules
	for (int i = 0; i < numSources; i++) {
		fprintf(makefile, "%s.o: %s.cpp ", sourceFiles[i], sourceFiles[i]);
		for (int j = 0; j < numHeaders; j++) {
			fprintf(makefile, "%s.h ", headerFiles[j]);
		}
		fprintf(makefile, "\n\t$(CC) $(CFLAGS) -c %s.cpp\n\n", sourceFiles[i]);
	}

	fclose(makefile);
}

int main() {
	const char* project = "drone";
	const char* sources[] = { "main", "init", "logic", "move", "my_time", "pumpkin"};
	const char* headers[] = { "unicode_chars","init", "logic", "move", "my_time", "pumpkin", "curses"};

	double DELAY = 0.15;
	int key_pressed = 0;

	generateMakefile(project, sources, headers, 6, 7);

	initScreen();
	initMap();

	time(&start_time);
	time(&main_time);
	pumpkin_time = localtime(&start_time);

	srand(time(NULL));

	drone_t* drone = (drone_t*)malloc(sizeof(drone_t));

	initPumpkin();

	initDrone(drone, RED);

	putPumpkin();

	timeout(0);

	while (key_pressed != 'q') {
		printTime();
		printLevel(drone);
		if (pumpkin_time->tm_sec >= 60 - MATURATION_TIME_MAX) {
			pumpkin_time->tm_sec = 0;
			for (uint8_t i = 0; i < PUMPKIN_MAX_QUANTITY; i++) {
				harvest[i].put_time = 0;
			}
		}
		clock_t begin = clock();
		key_pressed = tolower(getch());

		moveDrone(drone);

		checkPickUp(drone);

		changeStatus();
		if (checkDirection(drone, key_pressed))
			changeDirection(drone, key_pressed);

		refresh();
		
		while ((double)(clock() - begin) / CLOCKS_PER_SEC < DELAY) {
			time(&main_time);
			pumpkin_time = localtime(&main_time);
		}

	}

	free(drone);
	for (uint8_t i = 0; i < PUMPKIN_MAX_QUANTITY; i++) {
		free(&harvest[i]);
	}
	free(pumpkin_time);
	endwin();
	return 0;
}
