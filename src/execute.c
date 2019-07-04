#include "execute.h"

int is_redirection(char **tokens)
{
    for(int i = 0; tokens[i]; i++)
    {
        if((strcmp(tokens[i], ">")==0) || (strcmp(tokens[i], ">>")==0) ||\
            (strcmp(tokens[i], "<")==0))
            return 1;
    }
    return 0;
}

char *tostring(char **tokens)
{
    char *res =(char *)calloc(255, sizeof(char));
    for(int i =0; tokens[i]; i++)
    {
        strcat(res, tokens[i]);
        strcat(res, " ");
    }
    return res;
}

int execute_pipes(struct Node *root)
{
    int p[2];
    pid_t pid;
    int   fd_in = 0;
    int returnval = 0;
    while (root)
    {
        pipe(p);
        if ((pid = fork()) == -1)
        {
            fprintf(stderr, "Pipe fail\n");
            return 127;
        }
        else if (pid == 0)
        {
            dup2(fd_in, 0); //change the input according to the old one 
            if (root->pipe)
                dup2(p[1], 1);
            close(p[0]);
            returnval = execute_tree(root, 0);
        }
        else
        {
            wait(NULL);
            close(p[1]);
            fd_in = p[0]; //save the input for the next command
            root = root->pipe;
        }
    }
    return returnval;
}

int execute_tree (struct Node *root, int returnval)
{
    if(returnval >= 420 || !root)
        return returnval;
    if(root->left)
        returnval =  execute_tree(root->left, returnval);
    if(!is_seperator(*root->command))
    {
        returnval = execute_node(root->command);
    }
    else
    {
        if(strcmp((*root->command), "&&") == 0 && returnval == 0)
            return execute_tree(root->right, returnval);
        else if(strcmp((*root->command), "||") == 0 && returnval != 0)
            return execute_tree(root->right, returnval);
        else if(strcmp((*root->command), ";") == 0)
            return execute_tree(root->right, returnval);
    }
    return returnval;
}

int execute_node(char **tokens)
{
    int check = is_builtin(tokens);
    if(check != 0)
    {
        if(check == 1)
        {
            if(tokens[1] && strcmp(tokens[1], "-n") == 0)
                return builtin_echo(tokens+2, 0);
            else
                return builtin_echo(tokens+1, 1);
        }
        else if (check == 2)
            return builtin_cd(tokens+1);
        else if (check == 3)
            return builtin_exit(tokens+1);
        else
            return builtin_kill(tokens+1);
    }
    else
    {
        pid_t child_pid, tpid;
        int child_status;
        child_pid = fork();
        if(child_pid == 0)
        {
            char *cmd = tokens[0];
            execvp(cmd, tokens);
            fprintf(stderr, "Execution failed\n");
            return 127;
        }
        else
        {
            do
            {
                tpid = wait(&child_status);
            } while(tpid != child_pid);
        }
        return child_status;
    }
}


/*
   if(is_redirection(tokens))
   {
   int kidpid;
   int tpid;
   int child_status;
   int fd = open(tokens[3], O_WRONLY|O_TRUNC|O_CREAT, 0644);
   if (fd < 0) { fprintf (stderr, "Redirection file open\n"); return 1;}
   switch (kidpid = fork()) 
   {
   case -1:
   fprintf(stderr, "fork fail\n");
   return -1;
   case 0:
   if (dup2(fd, 2) < 0) 
   {
   fprintf(stderr, "dup2\n"); 
   return -1;
   }
   close(fd);
   execvp(tokens[0], tokens); 
   fprintf(stderr, "Execvp failed\n");
   return 127;
   default:
   close(fd);
   do
   {
   tpid = wait(&child_status);
   } while(tpid != kidpid);
   return child_status;
   }
   }
 */
