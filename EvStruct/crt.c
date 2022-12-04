#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ncurses.h>
#include <time.h>

#define DPERMS 755
#define FPERMS 644

//char* schedloc = "~/.local/dsep/sched";
char* schedloc = "./sched";
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

char* substr(char* str, int st, int ed)
{
	char* ss = (char *)malloc(sizeof(char) * (ed - st + 2));
	strncpy(ss, str + st, ed - st + 1);
	ss[ed - st + 1] = '\0';
	
	return ss;
}

// 일정 저장 디렉토리 구조 생성
void makeCont(int year) {
	ssched isch;
	isch.year = year;
	isch.month = 1;
	isch.day = 1;
	char buf[12] = { '\0', };
	chdir(schedloc);
	char cwd[256] = { '\0', };
	int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	strcat(cwd, schedloc);
	if (mkdir(strcat(cwd, "/year"), DPERMS) < 0)
		return;
	chdir(cwd);
	for (; isch.month <= 12; ++isch.month) {
		sprintf(buf ,"%d", isch.month);
		mkdir(buf, DPERMS);
		snprintf(buf, sizeof(buf), "%d", isch.month);
		chdir(buf);
		for (int day = 1; day <= days[isch.month - 1]; ++day)
		{
			snprintf(buf, sizeof(buf), "%d", day);
			mkdir(buf, DPERMS);
		}
		memset(buf, '\0', sizeof(buf));
	}

}

void writesch(ssched sch) {
	int fd;
	char* fname = "";
	sprintf(fname, "%d/%d/%d/%d:%d~%d:%d", sch.year, sch.month, sch.day, sch.sthour, sch.stmin, sch.endhour, sch.endmin);
	if ((fd = open(fname, O_RDWR|O_CREAT, FPERMS)) < 0) {
		perror("Error while writing schedule!");
		return;
	}
	write(fd, sch.note, strlen(sch.note));
	close(fd);
}

// 날짜는 01;11; 과 같이 앞에 0을 붙여서 표기
ssched readsch(int yr, int mh, int dd, int sth, int stm, int endh, int endm) {
	int fd;
	size_t rsize;
	ssched res;
	memset(&res, 0, sizeof(res));
	char* schname = "";
	sprintf(schname, "%d/%d/%d/%d:%d~%d:%d", yr, mh, dd, sth, sth, endh, endm);
	if ((fd = open(schname, O_RDWR)) < 0) {
		perror("Error in reading schedule!");
		memset(&res, 0, sizeof(ssched));
		return res;
	}
	char buf[21];
	do {
		memset(buf, 0, sizeof(buf));
		rsize = read(fd, buf, 1024);
	} while(rsize > 0);
	res.year = yr;
	res.month = mh;
	res.day = dd;
	res.sthour = sth;
	res.stmin = stm;
	res.endhour = endh;
	res.endmin = endm;
	strcpy(res.note, buf);

	return res;
}

int printSched(char* date)
{
	char fname[256] = { '\0', };
	char buf[21] = { '\0', };
	strcpy(fname, date);
	DIR* dirp;
	struct dirent* dentry;
	dirp = opendir(date);
	strcat(date, "/");
	while ((dentry = readdir(dirp)) != NULL)
	{
		int fd = open(strcat(fname, dentry->d_name), O_RDONLY);
		read(fd, buf, sizeof(buf));
		close(fd);

		printf("%s: %s\n", fname, buf);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	makeCont(2022);

	return 0;
}
