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
    if(root->pipe)
    {
        int fds [2];
        assert(pipe(fds) != -1);
        pid_t pid = fork();
        if (pid == -1)
        {
            fprintf(stderr, "Pipe fail\n");
            return 127;
        }
        if(pid)
        {
            close(fds[0]);
            close(fds[1]);
            int exitstat;
            assert(waitpid(pid, &exitstat, 0) != -1);
            return exitstat;
        }

        pid_t pid2 = fork();
        if (pid2 == -1)
        {
            fprintf(stderr, "Pipe fail\n");
            return 127;
        }
        if (pid2)
        {
            int exitstat;
            assert(waitpid(pid2, &exitstat, 0) != -1);
            close(fds[1]);
            assert(dup2(fds[0], 0) != -1);
            close(fds[0]);
            return execute_pipes(root->pipe);
        }
        close(fds[0]);
        assert(dup2(fds[1], 1) != -1);
        int returnval = execute_tree(root, 0);
        close(fds[1]);
        return returnval;
    }
    return execute_tree(root, 0);
}

int execute_tree (struct Node *root, int returnval)
{
    if((returnval >=420 && returnval <= 655) || (!root))
        return returnval;
    if(root->left)
        returnval =  execute_tree(root->left, returnval);
    if(root->command)
    {
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
    }
    else
    {
        returnval = execute_redir(root->redir);
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
            return 126;
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

int execute_redir_greater(struct Redir *node, int ionum)
{
    pid_t child_pid, parent_pid;
    int child_status;
    int fd = open(node->next->string[0], O_WRONLY|O_TRUNC|O_CREAT, 0644);
    if (fd < 0) { fprintf (stderr, "Redirection file open\n"); return 1;}
    switch(child_pid = fork())
    {
        case -1:
            fprintf(stderr, "Fork fail\n");
            return -1;
        case 0:
            if(dup2(fd, ionum) < 0)
            {
                fprintf(stderr, "dup2 fail\n");
                return -1;
            }
            close(fd);
            execvp(node->string[0], node->string);
            fprintf(stderr, "execvp fail\n");
            return 127;
        default:
            close(fd);
            do
            {
                parent_pid = wait(&child_status);
            }while(parent_pid != child_pid);
            return child_status;
    }
}


int execute_redir(struct Redir* node)
{
    if(node->redir == 0)
    {
        int ionum = node->ionum;
        if(node->ionum == -1)
            ionum = 1;
        return execute_redir_greater(node, ionum);
    }
    else
        return 0;
}

