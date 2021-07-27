/*
 * Copyright(c) 2010-2015 Beck. All rights reserved.
 *
 * Function : get latitude and longitude using cell identity and location area code
 * Author   : Beck <becklc@163.com>
 *
*/
 
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
 
#define     HOSTNAME                    "www.google.com"
 
#define     RequestHttpHeader           "POST /glm/mmap HTTP/1.1\x0D\x0A"\
                                        "Content-Type: application/binary\x0D\x0A"\
                                        "Host: www.google.com\x0D\x0A"\
                                        "Content-Length: 55\x0D\x0A"\
                                        "Expect: 100-continue\x0D\x0A"\
                                        "Connection: Close\x0D\x0A\x0D\x0A"
 
#define     ContentLenFlag              "\x0D\x0A""Content-Length:\x20"
 
#define     SIZE_REQDATA		    55
 
typedef struct {
    uint32_t cti;               /* Cell Identity */
    uint32_t lac;               /* Location Area Code */
    uint32_t mcc;               /* Mobile Country Code */
    uint32_t mnc;               /* Mobile Network Code */
} bsi_t;
 
typedef struct {
    bsi_t bsi;
    uint8_t reqdata[SIZE_REQDATA];
    double latitude;
    double longitude;
} infos_t;
 
static infos_t infos[1];
 
int is_little_endian()
{
    int i = 1;
    return (*(char *) &i == 1) ? 1 : 0;
}
 
 
static char *str2hex(const char *src, char *dec, int len)
{
    char *hex = "0123456789ABCDEF";
    char *ret = dec;
    int i = 0;
    if (NULL == src || NULL == dec)
        return NULL;
 
    for (i = 0; i < len; i++) {
        *ret++ = hex[((*(src + i) & 0xF0) >> 4) & 0x0F];
        *ret++ = hex[*(src + i) & 0x0F];
        *ret++ = ' ';
    }
    return ret = dec;
}
 
 
static int connect_to_server(const char *strhost, uint16_t port)
{
    struct hostent *host;
    struct sockaddr_in serv_addr;
    int hsocket = -1;
    if ((host = gethostbyname(strhost)) == NULL) {
        printf("%s, gethostbyname error, %s\n", __func__, strerror(errno));
        return -1;
    }
 
    if ((hsocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("%s, socket error, %s\n", __func__, strerror(errno));
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *) host->h_addr);
    memset(&(serv_addr.sin_zero), 0x0, 8);
    if (connect(hsocket, (struct sockaddr *) &serv_addr,
         sizeof(struct sockaddr)) == -1) {
        printf("%s, connect error, %s\n", __func__, strerror(errno));
        return -1;
    }
    return hsocket;
}
 
 
/* get the size of content from HttpServer */
static int get_content_size(const char *recvdata, int recvdatalen)
{
    /* check status is ok */
    char *pok = NULL;
    char *p = NULL, *end = NULL;
    int value = 0, valuelen = 0, c;
 
    if (NULL == (pok = strstr(recvdata, "OK"))) {
        printf("Not find 'OK' flag in recv data.\n");
        return -1;
    }
 
    /* find the flag Content-Length: in recvdata */
    if (NULL == (p = strstr(recvdata, ContentLenFlag))) {
        printf("Not find 'Content_Length' flag in recv data.\n");
        return -1;
    }
    p = p + sizeof(ContentLenFlag) - 1;
 
    /* find the end of Content-Length: */
    if (NULL == (end = strstr(p, "\x0D\x0A"))) {
        printf("Not find the end of 'Content_Length' in recv data.\n");
        return -1;
    }
 
    /* str2int */
    value = 0;
    valuelen = end - p;
    for (; valuelen > 0; valuelen--) {
        if ((c = *p++ - '0') >= 10)
            return -1;
        value = value * 10 + c;
    }
 
    /* check the third number in content is 0x00 */
    if (0x00 != recvdata[recvdatalen - value + 3]) {
        printf("the content in recv data is invalid.\n");
        return -1;
    }
 
    return value;
}
 
 
static double getdata(uint8_t * pdata, int pos)
{
    if (NULL == pdata)
        return -1;
 
    if (is_little_endian()) {
        return ((double)((pdata[pos] << 24) | (pdata[pos + 1] << 16) |
                 (pdata[pos + 2] << 8) | (pdata[pos + 3]))) / 1000000;
    } else {
        return ((double) (*(uint32_t *) & pdata[pos])) / 1000000;
    }
}
 
 
static int get_latitude_longitude(infos_t * p)
{
    int hsocket = -1;
    int pos = 0, len = 0, ret = 0;
    int contentSize = 0;
    char recvRsp[512] = { 0 };
 
    if ((hsocket = connect_to_server(HOSTNAME, 80)) == -1) {
        printf("%s, connect to server error, %s\n", __func__,
               strerror(errno));
        ret = -1;
        goto err;
    }
 
    if (-1 == send(hsocket, RequestHttpHeader, strlen(RequestHttpHeader), 0)) {
        printf("%s, send RequestHttpHeader error, %s\n", __func__,
               strerror(errno));
        ret = -1;
        goto err;
    }
 
    if (-1 == send(hsocket, p->reqdata, SIZE_REQDATA, 0)) {
        printf("%s, send reqdata error, %s\n", __func__, strerror(errno));
        ret = -1;
        goto err;
    }
 
    while ((pos = recv(hsocket, recvRsp + len, sizeof(recvRsp) - len, 0)) > 0) {
        len += pos;
    }
 
    contentSize = get_content_size(recvRsp, len);
    p->latitude = getdata((uint8_t *) (recvRsp + len - contentSize), 7);
    p->longitude = getdata((uint8_t *) (recvRsp + len - contentSize), 11);
 
  err:
    close(hsocket);
    return ret;
}
 
 
 
static int reqdata_init(infos_t * p)
{
    p->reqdata[1]  = 0x0e;
    p->reqdata[16] = 0x1b;
 
    /* wcdma unsame to gsm */
    p->reqdata[28] = ((uint64_t) p->bsi.cti > 65536) ? 5 : 3;
 
    p->bsi.cti = htonl(p->bsi.cti);
    p->bsi.lac = htonl(p->bsi.lac);
    p->bsi.mcc = htonl(p->bsi.mcc);
    p->bsi.mnc = htonl(p->bsi.mnc);
 
    memcpy(p->reqdata + 17, &p->bsi.mnc, 4);
    memcpy(p->reqdata + 21, &p->bsi.mcc, 4);
    memcpy(p->reqdata + 31, &p->bsi.cti, 4);
    memcpy(p->reqdata + 35, &p->bsi.lac, 4);
    memcpy(p->reqdata + 39, &p->bsi.mnc, 4);
    memcpy(p->reqdata + 43, &p->bsi.mcc, 4);
 
    memset(p->reqdata + 47, 0xff, 4);
    return 0;
}
 
 
int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage <%s> : %s <CellTowerId> <LocationAreaCode>\n",
               argv[0], argv[0]);
        return 0;
    }
    infos_t *p = infos;
    memset(p, 0x0, sizeof(infos_t));
 
    p->bsi.cti = (uint32_t) atoi(argv[1]);
    p->bsi.lac = (uint32_t) atoi(argv[2]);
 
    reqdata_init(p);
    if (0 == get_latitude_longitude(p)) {
        printf("latitude : %g, longitude : %g\n", p->latitude, p->longitude);
    } else
        printf("failed.\n");
 
    return 0;
}

