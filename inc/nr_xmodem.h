/**
 * @file nr_xmodem.h
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
#ifndef __NR_XMODEM_H__
#define __NR_XMODEM_H__

// XMODEM constants
#define XMODEM_INITIAL_BUFFER_SIZE 4096 // TH
#define XMODEM_INCREMENT_AMMOUNT 4096   // TH

// xmodem timeout/retry parameters
#define XMODEM_TIMEOUT 1000
#define XMODEM_RETRY_LIMIT 20

// error return codes
#define XMODEM_ERROR_REMOTECANCEL (-1)
#define XMODEM_ERROR_OUTOFSYNC (-2)
#define XMODEM_ERROR_RETRYEXCEED (-3)
#define XMODEM_ERROR_OUTOFMEM (-4)
#define XMODEM_ERROR_INTERNAL (-5) // TH

typedef int (*p_xm_send_func)(int data);
typedef int (*p_xm_recv_func)(int timeout );

void xmodem_init(p_xm_send_func send_func, p_xm_recv_func recv_func);
int xmodem_receive(char **dest);
void shell_xmodem_cmd(char argc, char *argv);

#endif