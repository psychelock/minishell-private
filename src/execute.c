#include "execute.h"

int execute_tree (struct Node *root)
{
    int returnval = 0;
    if(!root)
        return returnval;
    if(root->left)
        returnval =  execute_tree(root->left);
    if(!is_seperator(*root->command))
        returnval = execute_node(root->command);
    else
    {
        if(strcmp((*root->command), "&&") == 0 && returnval == 0)
            return execute_tree(root->right);
        else if(strcmp((*root->command), "||") == 0 && returnval != 0)
            return execute_tree(root->right);
        else if(strcmp((*root->command), ";") == 0)
            return execute_tree(root->right);
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
        return 0;
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
