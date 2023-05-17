/* 
   Name: Darius Kianersi
   UID: 118799810
   Username: radius
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "command.h"
#include "executor.h"

/* static void print_tree(struct tree *t); */

/*
   execute_aux is a helper method for execute, which 
   passes the input and output file descriptors
   along with the tree itself.
*/
int execute_aux(struct tree *t, int infd, int outfd) {
   if (t != NULL) {
      int fd1, fd2;

      /* Overwrite file descriptors with the given ones
      from the tree, if they exist. Otherwise, use the 
      helper method parameters. */
      fd1 = t->input != NULL ? open(t->input, O_RDONLY) : infd;
      fd2 = t->output != NULL ? open(t->output, O_WRONLY | O_CREAT, 0644) : outfd;

      if (t->conjunction == NONE) {
         if (!strcmp(t->argv[0], "cd")) {
            int err;

            /* If no argument is provided to cd,
            change directory to home. */
            if (t->argv[1] == NULL) {
               err = chdir(getenv("HOME"));
            } else {
               err = chdir(t->argv[1]);
            }

            if (err == -1) {
               perror(t->argv[1]);
               return EXIT_FAILURE;
            }
         } else if (!strcmp(t->argv[0], "exit")) {
            printf("See you.\n");
            exit(EXIT_SUCCESS);
         } else {
            /* Fork a child to run the command. */
            int pid = fork();
            
            if (pid < 0) {
               perror("fork");
               exit(EXIT_FAILURE);
            } else if (pid == 0) {
               /* Check if the file descriptor isn't
               stdin or stdout before dup2'ing. */
               if (fd1 != STDIN_FILENO) {
                  if (dup2(fd1, STDIN_FILENO) < 0) {
                     perror("dup2");
                     exit(EXIT_FAILURE);
                  }
                  close(fd1);
               }

               if (fd2 != STDOUT_FILENO) {
                  if (dup2(fd2, STDOUT_FILENO) < 0) {
                     perror("dup2");
                     exit(EXIT_FAILURE);
                  }
                  close(fd2);
               }

               /* Execute command */
               execvp(t->argv[0], t->argv);
               fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
               exit(EXIT_FAILURE);
            } else {
               int status;

               wait(&status);
               /* Ensure that child exited successfully. */
               if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
                  return EXIT_SUCCESS;
               else 
                  return EXIT_FAILURE;
            }
         }
      } else if (t->conjunction == AND) {
         int result = execute_aux(t->left, fd1, fd2);

         if (result == EXIT_SUCCESS) {
            execute_aux(t->right, fd1, fd2);
         } 
         else return EXIT_FAILURE;
      } else if (t->conjunction == PIPE) {
         if (t->left->output != NULL) {
            printf("Ambiguous output redirect.\n");
         } else if (t->right->input != NULL) {
            printf("Ambiguous input redirect.\n");
         } else {
            int pipe_fd[2];
            int pid_left, pid_right;

            if (pipe(pipe_fd) < 0) {
               perror("pipe");
               exit(EXIT_FAILURE);
            }

            /* Fork a child for the left
            side of the pipe. */
            pid_left = fork();
            if (pid_left < 0) {
               perror("fork");
               exit(EXIT_FAILURE);
            } else if (pid_left == 0) {
               close(pipe_fd[0]);
               if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
                  perror("dup2");
                  exit(EXIT_FAILURE);
               }
               close(pipe_fd[1]);
               if (execute_aux(t->left, fd1, fd2) != EXIT_SUCCESS) {
                  exit(EXIT_FAILURE);
               } else {
                  exit(EXIT_SUCCESS);
               }
            } else {
               /* Fork a child for the right side
               of the pipe. */
               pid_right = fork();
               if (pid_right < 0) {
                  perror("fork");
                  exit(EXIT_FAILURE);
               } else if (pid_right == 0) {
                  close(pipe_fd[1]);
                  if (dup2(pipe_fd[0], STDIN_FILENO) < 0) {
                     perror("dup2");
                     exit(EXIT_FAILURE);
                  }
                  close(pipe_fd[0]);
                  if (execute_aux(t->right, fd1, fd2) != EXIT_SUCCESS) {
                     exit(EXIT_FAILURE);
                  } else {
                     exit(EXIT_SUCCESS);
                  }
               } else {
                  /* Parent should wait for
                  children to finish. */
                  int status1, status2;
                  close(pipe_fd[0]);
                  close(pipe_fd[1]);
                  wait(&status1);
                  wait(&status2);
                  if (WIFEXITED(status1) & WIFEXITED(status2)) {
                     if (WEXITSTATUS(status1) == EXIT_SUCCESS &&
                         WEXITSTATUS(status2) == EXIT_SUCCESS) {
                        return EXIT_SUCCESS;
                     }
                  } else {
                     return EXIT_FAILURE;
                  }
               }
            }
         }
         
      } else if (t->conjunction == SUBSHELL) {
         /* Fork a shell to run commands in. */
         int pid;
         if ((pid = fork()) < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
         } else if (pid == 0) {
            int result = execute_aux(t->left, fd1, fd2);
            if (result == EXIT_FAILURE) {
               exit(EXIT_FAILURE);
            } else {
               exit(EXIT_SUCCESS);
            }
         } else {
            /* Reap child */
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
               return EXIT_SUCCESS;
            else 
               return EXIT_FAILURE;
         }
      }

      return EXIT_SUCCESS;
   }
   return EXIT_FAILURE;
}

int execute(struct tree *t) {
   return execute_aux(t, STDIN_FILENO, STDOUT_FILENO);
}

/*
static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}
*/
