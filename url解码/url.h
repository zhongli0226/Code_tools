/*
 * @Author: tangwc
 * @Date: 2022-11-08 09:24:51
 * @LastEditors: tangwc
 * @LastEditTime: 2022-11-08 21:47:17
 * @Description: 
 * @FilePath: \esp32_wifi_link\components\web_server\url.h
 *  
 *  Copyright (c) 2022 by tangwc, All Rights Reserved.
 */
#ifndef  __URL_H__
#define  __URL_H__

uint32_t url_decode(char *url_in, char *url_out);
uint32_t url_encode(char *url_in, char *url_out);

#endif