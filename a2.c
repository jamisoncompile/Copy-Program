/*
Names: Jamison Campbell, Jacob Jewel, Shyam Patel
Class: CPS 470 T/Th 3:30-4:45 22362465 
Assignment: a2
Due: January 29, 2019
Credit: 10 points
Problem:
Create a program to copy files from src to dst path specified in arguments
using system call interface, however, it should only copy regular files
from src, should not copy file that already exists at dst, should check if
last arg (dst) is a dir, if errors are encountered the program should
continue but failed calls should printed to stderr 
Soluion:
Have a module to return 0 or 1 for each subtask and store call, then print
error, this will include chkdst(), isdir(), isregular(), then modules to
create path and copy files which will include copyfiles(), cp1file(), and
buildpath() 
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int chkdst(int argc, char **argv);
int isdir(char *path);
int isregular(char *path);
int copyfiles(int argc, char **argv);
int cp1file(char *srcpath, char *dstpath);
char *buildpath(char *src, char *dst);

int main(int argc, char *argv[])
{
  if(chkdst(argc, argv) == 1)
    copyfiles(argc, argv);
  else
    fprintf(stderr, "Invalid destination path\n");
  exit(0);
}

/*
Problem: 
Create a function that will check whether or not there is a
destination path in the command line arguement.
Solution: 
Assign a char pointer to the location in argv where the
destination path is. Then check if argc has the proper amount of
arguents. If not print out a stderr. Finally call isdir method to see
whether or not the directory is valid.
*/
int chkdst(int argc, char **argv)
{
  char *dst;
  dst = argv[argc - 1];

  if(argc <= 2)
  {  
    fprintf(stderr, "Invalid number of arguments\n");
    exit(0); 
  }
  else
    return isdir(dst);
}

/*
Problem: 
Create a function that will check whether or not the directory is
proper, and can be a directory for the copied files
Solution: 
Declare a stat struct and a retval int. Stat will then check
with the flag S_SDIR if the path that is assigned in the stat is a
directory. Retval will return with the success or failure.
*/
int isdir(char *path)
{
  struct stat sbuf;
  int retval;
  stat(path, &sbuf);
  retval = S_ISDIR(sbuf.st_mode);
  return retval;
}

/*
Problem: 
Create a function that will check if a file is regular.
Solution: 
Declare a stat struct and int retval. The stat will hold the
path in sbuf. Using S_ISREG flag we will check if the path is a regular
file. Retval will return with the success or failure.
*/
int isregular(char *path)
{
  struct stat sbuf;
  int retval;
  stat(path, &sbuf);
  retval = S_ISREG(sbuf.st_mode);
  return retval;
}

/*
Problem: 
Create a function that will create a path for a file and copy it
into the proper directory while also catching various returns form calls
that catch usage errors, and then print.
Solution: 
Function will take argc, and **argv as parameters. The char
pointer dst will be initialized to destination path. A for loop will be
ran until in the range of 1 to argc - 1. A char pointer src will be 
initialzed to the file at argv position i. isregular will check if src
is regular or not. If it is then an int variable exists will be initalized
to the return value of cp1file with arguments of src and the return value
of buildpath with arguments of src and dst. If exists == 0 an fprintf will
print to stdout a success message. If it does not then an fprintf will
print to stdout a failure message. If the file is not a regular file then
an fprintf will print to stdout a failure message of the file not being
regular.
*/
int copyfiles(int argc, char **argv)
{
  char *dst, *src;
  int i, exists;
  
  dst = argv[argc - 1];
    
  for(i = 1; i < argc - 1; i++)
  {
    src = argv[i];
    if(isregular(src) == 1)
    {
      exists = cp1file(src, buildpath(src, dst));
      if(exists == 0)
        fprintf(stderr, "%-16s:  exists at destination\n", src);
      else
        fprintf(stderr, "%-16s:  copied\n", src);
    }
    else if(isregular(src) == 0 && isdir(src) == 0)
      fprintf(stderr, "%-16s:  no such file or directory\n", src);
    else
      fprintf(stderr, "%-16s:  not a regular file\n", src);
  }
}

/*
Problem: 
To create a function that will first read the src file. Then
create a new file at the directory destination. Then write the src file
into the new file
Solution: 
An fdin int variable will hold the return value of open() with
the parameters of srcpath, O_RDONLY (read only). Another int variable of
fdout will hold the return value of open() with parameters of dstpath,
O_CREAT (create file) | O_WRONLY (write only) | O_EXCL (return -1 if file
already exists). A while loop will run while a ssize_t variable nread =
read(fdin, buf, 2048) is less than 0. In the loop nwrite will be
initialized to the return value of write(fdout, buf, (ssize_t)nread). If
fdout is greater than 0 retval will equal 1. Else retval will equal 0.
Retal will be returned.
*/
int cp1file(char *srcpath, char *dstpath)
{
  int fdin, fdout, retval;
  char buf[2048];
  ssize_t nread, nwrite;
  
  fdin = open(srcpath, O_RDONLY);
  fdout = open(dstpath, O_CREAT | O_WRONLY | O_EXCL, 0644);

  while( (nread = read(fdin, buf, 2048)) > 0 )
    nwrite = write(fdout, buf, (ssize_t)nread);

  if(fdout > 0)
    retval = 1;
  else
    retval = 0;

  close(fdin);
  close(fdout);

  return retval;
}

/*
Problem:
Create a function since a full path is needed for the destination to
create the file in cp1file() and write to it, the last arg saved as dst is
not sufficent, the last part of the src (filename) will be added to the
dst to create the full path.
Solution:
Create a pointer for the chars after the last occurence of '/'in src which
is then the file name, then copy the destination path to a new array of
chars called path which has memory allocated for the destination parameter
length plus the newly made pointer. Lastly, concatenate the path string
with the conents of the pointer and return the full path to copyfile().
*/
char *buildpath(char *src, char *dst)
{
  char *ptr, *path;
  ptr = strrchr(src, '/');
  path = malloc(strlen(dst) + strlen(ptr) + 1);
  strcpy(path, dst);
  strcat(path, ptr);
  return path;
}

