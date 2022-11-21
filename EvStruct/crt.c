#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ncurses.h>

#define PERMS 755

char* schedloc = "~/.local/dsep/sched";
//char temp[256] = "";

typedef struct sched {
	int year;
	int month;
	int day;
	int sthour;
	int stmin;
	int endhour;
	int endmin;
	char note[21];
} ssched;

void makeCont(int year) {
	ssched isch;
	isch.year = year;
	isch.month = 1;
	isch.day = 1;
	char buf[12];
	
	chdir(schedloc);
	char cwd[256];
	strcat(cwd, schedloc);
	mkdir(strcat(cwd, "/year"), PERMS);
	chdir(cwd);
	for (; isch.month <= 12; ++isch.month) {
		sprintf(buf ,"%d", isch.month);
		mkdir(buf, PERMS);
		memset(buf, '\0', sizeof(buf));
	}

}

int main(int argc, char *argv[])
{
	DIR* dirp;
	struct dirent* dentry;



	return 0;
}
