/**
 * @file nr_xmodem.c
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
#include "system_api.h"
#include "nr_xmodem.h"
#include "nr_micro_shell.h"


static p_xm_send_func xmodem_out_func;
static p_xm_recv_func xmodem_in_func;

// Line control codes
#define XM_SOH 0x01
#define XM_STX 0x02 // TH: Marker for 1K Blocks
#define XM_ACK 0x06
#define XM_NAK 0x15
#define XM_CAN 0x18
#define XM_EOT 0x04

void xmodem_init(p_xm_send_func send_func, p_xm_recv_func recv_func)
{
    logi("xmodem init ....\r\n");
    xmodem_out_func = send_func;
    xmodem_in_func = recv_func /*recv_func*/;
}

// This private function receives a x-modem record to the pointer and
// returns 1 on success and 0 on error
static int xmodem_get_record(unsigned char blocknum, unsigned char *pbuf, /*TH*/ unsigned int pack_sz /*TH*/)
{
    unsigned int chk, j, size;
    int ch;

    // Read packet
    for (j = 0; j < (pack_sz + 4); j++)
    {
        //TH : First read with zero timeout until the input fifo is empty
        // This should avoid FIFO overflows caused by the overhead of the eLua timers.
        ch = xmodem_in_func(0);
        if (ch == -1)
        {
            // When no char available wait for XMODEM_TIMEOUT if a char arrives
            ch = xmodem_in_func(XMODEM_TIMEOUT);
            // If not we have a timout error
            if (ch == -1)
                goto err;
        }
        pbuf[j] = (unsigned char)ch;
        //logi("pbuf[%d] = %d\r\n", j, pbuf[j]);
    }

    // Check block number
    if (*pbuf++ != blocknum){
        loge("blocknum = %d, rev block num = %d\r\n",blocknum,*pbuf);
        goto err;
    }
        
    if (*pbuf++ != (unsigned char)~blocknum){
        loge("~blocknum = %d, rev block num = %d\r\n", ~blocknum, *pbuf);
        goto err;
    }
       
    // Check CRC
    for (size = chk = 0; size < pack_sz; size++, pbuf++)
    {
        chk = chk ^ *pbuf << 8;
        for (j = 0; j < 8; j++)
        {
            if (chk & 0x8000)
                chk = chk << 1 ^ 0x1021;
            else
                chk = chk << 1;
        }
    }
    chk &= 0xFFFF;
    if (*pbuf++ != ((chk >> 8) & 0xFF)){
        loge("checksun verfiy failed!\r\n");
        goto err;
    }
        
    if (*pbuf++ != (chk & 0xFF)){
        loge("checksun verfiy2 failed!\r\n");
        goto err;
    }
        
    return 1;

err:
    xmodem_out_func(XM_NAK);
    return 0;
}

static void * nr_realloc(void *ptr, size_t new_size,size_t old_size){
    void *new_ptr = (void *)malloc(new_size);
    if(0 == new_ptr){
        return 0;
    }
    memset(new_ptr, 0, sizeof(new_ptr));
    memcpy(new_ptr, ptr, old_size);
    free(ptr);
    return new_ptr;
}

// This global function receives a x-modem transmission consisting of
// (potentially) several blocks.  Returns the number of bytes received or
// an error code an error
int xmodem_receive(char **dest)
{
    int starting = 1, ch;
    unsigned char buf[1024 + 4];
    unsigned char packnum = 1;
    unsigned int retries = XMODEM_RETRY_LIMIT;
    unsigned int limit = XMODEM_INITIAL_BUFFER_SIZE, size = 0;
    void * p;
    unsigned int pack_sz; // TH
    memset(buf, 0, sizeof(buf));
    while ((retries--) > 0)
    {

        if (starting)
            xmodem_out_func('C');
        if (((ch = xmodem_in_func(XMODEM_TIMEOUT)) == -1) || (ch != XM_SOH && /* TH */ ch != XM_STX && /*TH*/ ch != XM_EOT && ch != XM_CAN))
            continue;

        switch (ch)
        { // TH
        case XM_EOT:{
            xmodem_out_func(XM_ACK);
            return size;
        }
        case XM_CAN:{
            xmodem_out_func(XM_ACK);
            return XMODEM_ERROR_REMOTECANCEL;
        }
        case XM_SOH:
            pack_sz = 128;
            logi("pack_sz = 128\r\n");
            break;
        case XM_STX:
            logi("pack_sz = 1024\r\n");
            pack_sz = 1024;
            break;
        default: // TH: Should never happen
            xmodem_out_func(XM_CAN);
            return XMODEM_ERROR_INTERNAL;
        }
        starting = 0;
        // Get XMODEM packet
        if ( xmodem_get_record(packnum, buf, pack_sz) == 0)
            continue; // allow for retransmission


        retries = XMODEM_RETRY_LIMIT;
        packnum++;

        // Got a valid packet
        if (size + pack_sz > limit)
        {
            unsigned int old_size = limit;
            limit += XMODEM_INCREMENT_AMMOUNT;
            if ((p = nr_realloc(*dest, limit,old_size)) == NULL)
            {
                xmodem_out_func(XM_CAN);
                logw("memory alloc failed\r\n");
                return XMODEM_ERROR_OUTOFMEM;
            }
            *dest = (char *)p;
        }
        // Acknowledge and consume packet

        memcpy(*dest + size, buf + 2, pack_sz);
        size += pack_sz;
        xmodem_out_func(XM_ACK);
    }
    // Exceeded retry count
    xmodem_out_func(XM_CAN);
    return XMODEM_ERROR_RETRYEXCEED;
}

/**
 * @brief xmodem command
 */
void shell_xmodem_cmd(char argc, char *argv)
{
    char *p;
    int actsize;
    char * shell_prog = 0;
    if ((shell_prog = malloc(XMODEM_INITIAL_BUFFER_SIZE)) == NULL)
    {
        shell_printf("Unable to allocate memory\n");
        return;
    }
    shell_printf("Waiting for file ... ");
    if ((actsize = xmodem_receive(&shell_prog)) < 0)
    {
        if (actsize == XMODEM_ERROR_OUTOFMEM)
        {
            shell_printf("file too big\r\n");
        }
        else{
            shell_printf("XMODEM error,actsize = %d\r\n", actsize);
        }
        goto exit;
    }
    shell_printf("xmodem receive success,waiting for write data to flash...\r\n");
    // Eliminate the XMODEM padding bytes
    p = shell_prog + actsize - 1;
    while (*p == '\x1A')
        p--;
    p++;
    shell_printf("\r\ndone, got %u bytes\r\n", (unsigned int )(p - shell_prog));

exit:
    logi("free memory\r\n");
    free(shell_prog);
}