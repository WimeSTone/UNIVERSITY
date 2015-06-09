#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <malloc.h>
#include <string.h>

struct list
{
	__ino_t st_ino;
	char *fileName;
	struct list *next;
} *begin;

void scanDirectory(char *dirName);
void add(__ino_t st_ino, char *fileName);
void printList();

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: %s [path]\n", argv[0]);
		return 0;
	}
	begin = NULL;
	scanDirectory(argv[1]);
	printList();
	return 0;
}

void scanDirectory(char *dirName)
{
	DIR *dir;
	struct dirent entry;
	struct dirent *entryPtr;
	int retval;
	dir = opendir(dirName);
	if (dir == NULL)
	{
		printf("Error: can't open directory %s\n", dirName);
		return;
	}
	retval = readdir_r(dir, &entry, &entryPtr);
	while(entryPtr != NULL) {
		struct stat entryInfo;
		if(strncmp(entry.d_name, ".", PATH_MAX) == 0
			|| strncmp(entry.d_name, "..", PATH_MAX) == 0)
		{
			retval = readdir_r(dir, &entry, &entryPtr);
			continue;
		}
		int dirlen = strlen(dirName) + 1;
		int filelen = strlen(entry.d_name) + 1;
		char path[dirlen + filelen];
		strncpy(path, dirName, dirlen);
		strncat(path, "/", 2);
		strncat(path, entry.d_name, filelen);
		if(lstat(path, &entryInfo) == 0)
		{
			if(S_ISDIR(entryInfo.st_mode))
				scanDirectory(path);
			else 
			if(!S_ISLNK(entryInfo.st_mode) && entryInfo.st_nlink > 1)
				add(entryInfo.st_ino, path);
		}
		else
			printf("Error: access denied to %s\n", path);
		retval = readdir_r( dir, &entry, &entryPtr );
	}
}

void add(__ino_t st_ino, char *fileName)
{
	struct list *tmp = (struct list*)malloc(sizeof(struct list));
	tmp->next = NULL;
	int len = strlen(fileName) + 1;
	tmp->fileName = (char*)malloc(len);
	strncpy(tmp->fileName, fileName, len);
	tmp->st_ino = st_ino;
	if(begin == NULL)
		begin = tmp;
	else
	{
		struct list *prev = NULL, *cur = begin;
		while(cur != NULL && cur->st_ino < tmp->st_ino)
		{
			prev = cur;
			cur = cur->next;
		}
		if (prev == NULL)
		{
			tmp->next = begin;
			begin = tmp;
		}
		else if (cur == NULL)
			prev->next = tmp;
		else
		{
			tmp->next = cur;
			prev->next = tmp;
		}
	}
}

void printList()
{
	struct list *p = begin;
	while (p != NULL)
	{
		__ino_t ino = p->st_ino;
		printf("Index node: %lu\n", ino);
		while (p != NULL && ino == p->st_ino)
		{
			printf("\t<- %s\n", p->fileName);
			p = p->next;
		}
	}
}
