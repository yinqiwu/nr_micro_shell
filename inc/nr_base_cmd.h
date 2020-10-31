/**
 * @file nr_base_cmd.h
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
#ifndef __NR_BASE_CMD_H__
#define __NR_BASE_CMD_H__
/*Adapt to the target system to add, delete in other projects*/
#include "system_api.h"
#include "fs_interface.h"

void shell_ls_cmd(char argc, char *argv);

void shell_cd_cmd(char argc, char *argv);

void shell_mkdir_cmd(char argc, char *argv);

void shell_rmdir_cmd(char argc, char *argv);

void shell_rm_cmd(char argc, char *argv);

void shell_cat_cmd(char argc, char *argv);

#endif