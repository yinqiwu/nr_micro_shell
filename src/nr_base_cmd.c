/**
 * @file nr_base_cmd.c
 * @author bruce.yin (crazybruce.yin@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-31
 * 
 * @copyright Copyright (c) 2020
 * 
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2020-10-31 <td>1.0     <td>bruce.yin     <td>Modfiy context
 * </table>
 * @verbatim
 * ==============================================================================
 *                      ##### How to use  #####                       
 * ==============================================================================
 * @endverbatim
 * @attention
 */
#include "nr_base_cmd.h"
#include "nr_micro_shell.h"
/**
 * @brief ls command
 */
void shell_ls_cmd(char argc, char *argv)
{
    /* filesystem ls */
    char open_dir[128] = {0};
    memset(open_dir, 0, sizeof(open_dir));
    if (argc == 1)
    {
        snprintf(open_dir, sizeof(open_dir), "./");
    }
    else
    {
        snprintf(open_dir, sizeof(open_dir), "./%s", &(argv[(int)argv[1]]));
    }
    DIR *root_dir = opendir(open_dir);
    if (root_dir == 0)
    {
        logi("root_dir == 0\n");
    }
    struct dirent *root_dirent = 0;
    while ((root_dirent = readdir(root_dir)) != NULL)
    {
        shell_printf("%s\t%s\r\n", (root_dirent->d_type == DT_DIR) ? "DIR" : "REG", root_dirent->d_name);
    }
    closedir(root_dir);
}

/**
 * @brief cd command
 */
void shell_cd_cmd(char argc, char *argv)
{
    /* filesystem ls */
    char open_dir[128] = {0};
    memset(open_dir, 0, sizeof(open_dir));
    snprintf(open_dir, sizeof(open_dir), "./%s", &(argv[(int)argv[1]]));
    DIR *root_dir = opendir(open_dir);
    if (root_dir == 0)
    {
        shell_printf("directory %s is exist\r\n", &(argv[(int)argv[1]]));
    }
    struct dirent *root_dirent = 0;
    while ((root_dirent = readdir(root_dir)) != NULL)
    {
        shell_printf("%s\r\n", root_dirent->d_name);
    }
    closedir(root_dir);
}

/**
 * @brief mkdir command
 */
void shell_mkdir_cmd(char argc, char *argv)
{
    /* filesystem ls */
    char open_dir[128] = {0};
    memset(open_dir, 0, sizeof(open_dir));
    snprintf(open_dir, sizeof(open_dir), "./%s", &(argv[(int)argv[1]]));
    mkdir(open_dir, 0);
}

/**
 * @brief rm command
 */
void shell_rmdir_cmd(char argc, char *argv)
{
    /* filesystem ls */
    char open_dir[128] = {0};
    memset(open_dir, 0, sizeof(open_dir));
    snprintf(open_dir, sizeof(open_dir), "./%s", &(argv[(int)argv[1]]));
    if (rmdir(open_dir) < 0)
    {
        shell_printf("%s deleate failed.\r\n", open_dir);
    }
}

/**
 * @brief rm command
 */
void shell_rm_cmd(char argc, char *argv)
{
    /* filesystem ls */
    char open_dir[128] = {0};
    memset(open_dir, 0, sizeof(open_dir));
    snprintf(open_dir, sizeof(open_dir), "./%s", &(argv[(int)argv[1]]));
    if (remove(open_dir) < 0)
    {
        shell_printf("%s remove failed.\r\n", open_dir);
    }
}

/**
 * @brief cat command
 */
void shell_cat_cmd(char argc, char *argv)
{
    /* filesystem ls */
    char open_dir[128] = {0};
    memset(open_dir, 0, sizeof(open_dir));
    snprintf(open_dir, sizeof(open_dir), "./%s", &(argv[(int)argv[1]]));
    int open_fd = -1;
    if ((open_fd = open(open_dir, O_RDONLY)) < 0)
    {
        shell_printf("%s does not exist\r\n", open_dir);
    }
    char data;
    while (read(open_fd, &data, 1) > 0)
    {
        shell_printf("%c", data);
    }
    close(open_fd);
}
