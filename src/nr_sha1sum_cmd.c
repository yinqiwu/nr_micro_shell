/**
 * @file nr_sha1sum_cmd.c
 * @author bruce.yin (crazybruce.yin@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-01
 * 
 * @copyright Copyright (c) 2020
 * 
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2020-12-01 <td>1.0     <td>bruce.yin     <td>Modfiy context
 * </table>
 * @verbatim
 * ==============================================================================
 *                      ##### How to use  #####                       
 * ==============================================================================
 * @endverbatim
 * @attention
 */
#include "nr_sha1sum_cmd.h"
#include "nr_micro_shell.h"
#include "sha1.h" //https://github.com/clibs/sha1
/**
 * @brief sha1 command
 */
void shell_sha1sum_cmd(char argc, char *argv)
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
    char data[64];
    memset(data, 0, sizeof(data));
    int read_size = 0;
    SHA1_CTX sha;
    memset(&sha, 0, sizeof(sha));
    SHA1Init(&sha);
    while ((read_size = read(open_fd, data, sizeof(data))) > 0)
    {
        SHA1Update(&sha, (uint8_t *)data, read_size);
        memset(data, 0, sizeof(data));
    }
    close(open_fd);
    char sha1_hash[32] = {0};
    memset(sha1_hash, 0, sizeof(sha1_hash));
    SHA1Final(sha1_hash, &sha);
    for (int i = 0; i < 20; i++)
    {
        shell_printf("%02x", sha1_hash[i]);
    }
    shell_printf("  %s\r\n", open_dir);
}
