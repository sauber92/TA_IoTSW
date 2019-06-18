#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myfifo.h"
#include <errno.h>
#include <fcntl.h>

#define STOP    0
#define START   1
#define CLEAR   2


//HTML
void html();

MsgType msg;

int main() {

    html();

    int sfd;
    if ((sfd = open(SERV_FIFO, O_RDWR)) < 0) {
        perror("open");
        exit(1);
    }
    msg.buttonType = CLEAR;
    write(sfd, (char *)&msg, sizeof(msg));
    close(sfd);
    return 0;
}

void html() {
    printf("Content-type:text/html\n\n");
    printf("<html>");
    printf("<head>");
    printf("<title>Term Project</title>");
    printf("</head>");

    printf("<body>");

    printf("<p>STOP Watch</p");
    printf("<br>");

    printf("<form method=get action=\"start.cgi\">");
    printf("<input type=\"submit\" name=\"button\" value=\"start\">");
    printf("</form>");

    printf("<form method=get action=\"stop.cgi\">");
    printf("<input type=\"submit\" name=\"button\" value=\"stop\">");
    printf("</form>");

    printf("<form method=get action=\"clear.cgi\">");
    printf("<input type=\"submit\" name=\"button\" value=\"clear\">");
    printf("</form>");

    printf("</body>");
    printf("</html>");
}
