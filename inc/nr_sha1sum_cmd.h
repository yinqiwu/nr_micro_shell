/**
 * @file nr_sha1sum_cmd.h
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
#ifndef __NR_SHA1SUM_CMD_H__
#define __NR_SHA1SUM_CMD_H__
/*Adapt to the target system to add, delete in other projects*/
#include "system_api.h"
#include "fs_interface.h"

void shell_sha1sum_cmd(char argc, char *argv);
#endif