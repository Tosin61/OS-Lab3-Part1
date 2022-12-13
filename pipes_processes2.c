#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[4];
  int pid;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "Lakers", NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  if (argc<1){
    printf("Please enter grep argument");
  }

  grep_args[1] = argv[1];

  pipe(pipefd);

  pid = fork();

  if (pid == 0)
    {
      pid2 = fork();
      if (pid2==0){
        dup2(pipefd[0], 0);
        close(pipefd[3]);
        close(pipefd[1]);
        close(pipefd[0]);
        execvp("sort", sort_args);
      }
      else if (pid2 < 0){
        printf("error");
      }
      else {
        dup2(pipefd[0], 0);
        dup2(pipefd[3], 1);

        close(pipefd[1]);
        close(pipefd[2]);

        execvp("grep", grep_args);
      }
      // child gets here and handles "grep Villanova"
      // replace standard input with input part of pipe
      // close unused hald of pipe
      // execute grep
    }
  else
    {
      // parent gets here and handles "cat scores"

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);
      close(pipefd[3]);

      // execute cat

      execvp("cat", cat_args);
    }
}