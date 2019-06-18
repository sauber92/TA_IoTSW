#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <wiringPi.h>
#include <signal.h>
#include "myfifo.h"

const int FndSelectPin[6] = {23, 22, 27, 18, 17 ,4};
const int FndPin[8] = {6, 12, 13, 16, 19, 20, 26, 21};
const int FndFont[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

//HTML
void html();

//FND - STOP Watch
void init();
void printClock(int min, int sec, int msec);
void fnd();

//FIFO
void getMsg();

//CHILD - thread
void child();

//signal handler
void myexit(){
    if(remove(SERV_FIFO) <0) {
        perror("remove");
        exit(1);
    }
    exit(0);
}
void SigIntHandler(int signo) {
    myexit(); 
}

//STOP Watch action value
#define STOP    0
#define START   1
#define CLEAR   2

//Clock variable - static
static int min, sec, msec;

//STOP Watch contorl value
int isRun = 0;

//FIFO file descriptor
static int fd;

//FIFO Msg struct
MsgType msg;

int main() {
    int sfd;
    //check already start
    //false: fork -> child()
    //true: msg send
    if((sfd = open(SERV_FIFO, O_RDWR)) < 0) { 
        html();
        pid_t pid;
        if((pid = fork()) <0) {
            perror("fork");
            exit(1);
        }else if(pid == 0) {
            //child
            child();
        }
    }else {
        //html code print
        html();
        //FIFO msg send
        msg.buttonType = START;
        write(sfd, (char *)&msg, sizeof(msg));
        close(sfd);
    }
}

/*
 *    HTML Code
 */
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

/*
 *    INIT: initailizing Raspi GPIO for FND
 */
void init()
{
    //초기 설정
    //FND가 Array형식으로 접근하기 때문에 SelectPin을 출력으로 셋팅한다
    int i;
    if (wiringPiSetupGpio() == -1) {
        printf("wiringPi setup error\n");
        exit(-1);
    }
    //FND 핀을 출력으로 셋팅
    for (i = 0; i<6; i++) {
        pinMode(FndSelectPin[i], OUTPUT);
        digitalWrite(FndSelectPin[i], HIGH);
    }
    //각 FND에 해당하는 개별 핀을 출력으로 셋팅한다
    for (i = 0; i<8; i++) {
        pinMode(FndPin[i], OUTPUT);
        digitalWrite(FndPin[i], LOW);
    }
}

/*
 *    FNDSELECT: Selected FND On, others OFF
 */
void FndSelect(int position) {
    int i;
    for (i = 0; i < 6; i++) {
        if (i==position) digitalWrite(FndSelectPin[i], LOW);
        else digitalWrite(FndSelectPin[i],HIGH);
    }
}

/*
 *    FND: STOP Watch
 */
void fnd() {
    init();
    int cnt =0;
    while (1) {
        printClock(min, sec, msec);
        //Stop watch controller
        if (isRun){
            cnt++;
        }
        if (cnt == 2) {
            msec++;
            cnt =0;
            if ( msec >= 100) {
                msec =0;
                if (++sec==60) {
                    min++; sec=0;
                }
            }
        }
    }    
}

/*
 *    GETMSG: FIFO msg read, control Stop watch
 */
void getMsg() {
    int n;
    while (1) {
        if ((n=read(fd, (char *)&msg, sizeof(msg))) < 0 ) {
            if (errno == EINTR) {
                continue;
            }
            else {
                perror("read");
                exit(1);
            }
        }
        if (msg.buttonType == STOP) {
            isRun = 0;
        }
        else if (msg.buttonType == START) {
            isRun = 1;
        }
        else if (msg.buttonType == CLEAR) {
            min =0; sec =0; msec =0;
        }
    }

}

/*
 *    PRINTCLOCK: Clock print
 *    min, sec, msec - static variable
 */
void printClock(int min,int sec,int  msec){
    int i, j;
    int flag =0;
    int shift=0x01;
    for (i = 0; i < 6; i++) {
        FndSelect(i);
        flag =0;
        shift = 0x01;
        for (j = 0; j < 8; j++) {
            switch(i){
                case 0:
                    flag = FndFont[min/10] & shift; //10분 단위 출력
                    digitalWrite(FndPin[j], flag);  
                    shift <<= 1;
                    break;
                case 1:
                    flag = FndFont[min%10] & shift; //1분 단위 출력
                    digitalWrite(FndPin[j], flag);
                    shift <<= 1;
                    break;
                case 2:
                    flag = FndFont[sec/10] & shift; //10초 단위 출력
                    digitalWrite(FndPin[j], flag);
                    shift <<= 1;
                    break;
                case 3:
                    flag = (FndFont[sec%10]+ 0b10000000) & shift;   //1초 단위 출력, '.' 출력
                    digitalWrite(FndPin[j], flag);
                    shift <<= 1;
                    break;
                case 4:
                    flag = FndFont[msec/10] & shift;    //0.1초 단위 출력
                    digitalWrite(FndPin[j], flag);
                    shift <<= 1;
                    break;
                case 5:
                    flag = FndFont[msec%10] & shift;    //0.01초 단위 출력  
                    digitalWrite(FndPin[j], flag);
                    shift <<= 1;
                    break;
            }       
        }
        delay(1);       //디스플레이 1주기는 0.006초 소요된다.
    }
}

/*
 *    CHILD: Set handler exit, singal
 *        thread: FND - STOP Watch
 *        thread: FIFO - FIFO Msg Receiver
 *    min, sec, msec initailze 0
 *    isRun initailize 1 - init start
 */
void child() {
    if (atexit(myexit) != 0) {
        perror("atexit");
        exit(1);
    }
    signal(SIGINT, SigIntHandler);
    if (mkfifo(SERV_FIFO, 0600) < 0) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(1);
        }
    }
    if ((fd = open(SERV_FIFO, O_RDWR)) < 0) {
        perror("open");
        exit(1);
    }
    pthread_t FND, FIFO;
    min=sec=msec =0;
    isRun =1;

    if (pthread_create(&FND, NULL, (void *)fnd, NULL) < 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&FIFO, NULL, (void *)getMsg, NULL) < 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(FND, NULL) < 0 ) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_join(FIFO, NULL) < 0 ) {
        perror("pthread_create");
        exit(1);
    }
}
