/*
 * @Author: tangwc
 * @Date: 2022-11-08 09:24:51
 * @LastEditors: tangwc
 * @LastEditTime: 2022-11-08 21:47:14
 * @Description:
 * @FilePath: \esp32_wifi_link\components\web_server\url.c
 *
 *  Copyright (c) 2022 by tangwc, All Rights Reserved.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "url.h"
static int hex2dec(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    return 0;
}

static char dec2hex(short int c)
{
    if (0 <= c && c <= 9)
        return c + '0';
    else if (10 <= c && c <= 15)
        return c + 'A' - 10;
    return 0;
}

/** url解码 **/
uint32_t url_decode(char *url_in, char *url_out)
{
    int i = 0, len = 0;
    int i_new = 0;

    if (!url_in || *url_in == 0)
        return 1;
    if (!url_out)
        return 1;

    len = strlen(url_in);

    for (i = 0; i < len; ++i)
    {
        char c = url_in[i];
        if (c != '%')
        {
            if (c == '+')
                url_out[i_new++] = ' ';
            else
                url_out[i_new++] = c;
        }
        else
        {
            char c1 = url_in[++i];
            char c0 = url_in[++i];
            url_out[i_new++] = hex2dec(c1) * 16 + hex2dec(c0);
        }
    }
    url_out[i_new++] = 0;
    return 0;
}

/** url编码 **/
uint32_t url_encode(char *url_in, char *url_out)
{
    int i = 0, j = 0;
    int url_len = 0;
    int m = 0, n = 0;

    if (!url_in)
        return 1;
    if (!url_out)
        return 1;

    url_len = strlen(url_in);

    for (i = 0; i < url_len; i++)
    {
        if ((url_in[i] >= '0' && url_in[i] <= '9') || (url_in[i] >= 'a' && url_in[i] <= 'z') || (url_in[i] >= 'A' && url_in[i] <= 'Z'))
        {
            url_out[j++] = url_in[i];
        }
        else
        {
            short int b = (short int)url_in[i];
            if (b < 0)
                b += 256;
            m = b / 16;
            n = b - m * 16;
            url_out[j++] = '%';
            url_out[j++] = dec2hex(m);
            url_out[j++] = dec2hex(n);
        }
    }
    return 0;
}

// int main(void)
// {
//     char str1[50], str2[50];
//     scanf("%s", str1);
//     printf("url:%s\n", str1);
//     url_decode(str1, str2);
//     url_encode(str2, str1);
//     printf("url-encode:%s\n", str1);
//     printf("url-decode:%s\n", str2);
// }
