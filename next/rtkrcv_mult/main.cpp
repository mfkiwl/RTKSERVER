/*
 * 基于RTKlib 2.4.3 b.34 Navi\rtkrcv模块的实时RTK解算程序
 * 可以理解为navi去掉了图形化模块，并可以将解算结果输出至数据库
 * 参考了navi（win）和rtkrcv（linux）的代码，利用qt跨平台的特性实现一份代码多平台运行。
 * 暂时没有对rtklib的源码部分进行修改，通过调用rtklib库的方式实现当前的功能
 *
 * 当前实现功能：
 * 基于RTKnavi内核（rtksvrstart()函数）的RTK解算
 * 通过调用计时器来读取rtk解算的结果并写入MySQL数据库（固定的表结构）
 * 通过读取conf文件和dbopt文件来设置解算参数和写入数据库的设置
 *
 * 后续功能完善：
 * monitor功能添加，复刻监视窗口功能，在conf\dbopt添加moni端口设置项，实现与rtkplot的对接
 * 添加日志记录功能，记录重要事件的操作信息（解算参数，错误事件，流数据断开，数据库写入失败等）。
 * 添加数据库写入检测，检测写入数据的连续性，记录数据库没能正常写入数据的事件（数据库断开？写入失败），找到解决方法，暂时保存到缓冲区重新写入？
 * 参考Ntrip Caster 2.0.27 的网页监视端，实现CUI程序的实时监控
 *
 * 实现一个程序同时运行多条基线解算，通过读取list 来自动化建立基线任务
 *
 * 当前存在的问题：
 * 在长时间接收不到数据的时候，数据库因为长时间没有写入数据所以可能会断开连接，导致数据重新接收之后无法继续写入数据库（应当添加一个datebase reconnect功能）
 *
*/

#include <QCoreApplication>
#include "navimain.h"

int  intrflg=1;

int main(int argc, char *argv[])
{

    int dispint=1000;//控制台状态刷新的时间
    int maxtask=50;//最大支持的任务数量
    int tasknum=0;

    char task[maxtask][MAXCHAR];

    char taskpath[MAXCHAR];

    //读文件，获取数据流内容和数据流数量

    if(argc<2)
    {
        strcpy(taskpath,"tasklist.txt");

    }
    else
    {
     for (int i=1;i<argc;i++)
     {
         if (!strcmp(argv[i],"-i")&&i+1<argc)
         {
              strcpy(taskpath,argv[++i]);
         }
     }
    }



     printf("set tasklist path: %s\n",taskpath);


          //  if (!decodepath(argv[++i],types,paths[0],fmts)) return -1;



    FILE *tasklist=fopen(taskpath,"r");
    if(tasklist == NULL)
        {
            printf("taskpath: %s open error!\n",taskpath);
            return 0;
        }


   // char a[MAXCHAR];
    //fgets(a,MAXCHAR,tasklist);//读文件头并略过

    for(tasknum=0;tasknum<maxtask;)
    {

        fgets(task[tasknum],MAXCHAR,tasklist);

        if (strstr(task[tasknum], "END OF LIST") != NULL)
        {
            printf("tasknum=%d end\n",tasknum);
            break;
        }

        printf("%d %s\n",tasknum,task[tasknum]);
        tasknum++;

    }


    //确定要开的线程数量n
    int n=tasknum;

    int argcc[n];
    char *argvv[n][50];


    strsvr_t strsvr[n];



    for(int i=0;i<n;i++)
    {
         char2arg(task[i],&argcc[i],argvv[i],50);
    }
    for(int i=0;i<n;i++)
    {
        str2str(&strsvr[i],argcc[i],argvv[i]);
    }


    dispint=dispint<1000?1000:dispint;

    sleepms(1000);
    for (intrflg=0;!intrflg;)
    {

        printf("taskpath:%s\n",taskpath);

        for(int i=0;i<n;i++)
        {


            //rtkserver(&strsvr[i],i+1);

        }
        sleepms(dispint);
        system("cls");
    }



   // svrstop(strsvr,n);





}


int rtkserver(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    //新建一个navi对象
    NaviMain w;

    //连接信号和槽，初始化timer
    w.init();

    //开始程序 构建函数 读取ini文件 获取OPT file 路径
    w.loadini();

    //初始化rtknavi所需的各项参数 读取conf
    w.svrinit();

    //初始化MySQL连接,读取参数到class内   读取dbopt
    w.SQLInit();

    //解算开始
    w.svrstart();

    //结果的实时输出函数
    //w.outputsol();


    qDebug()<<"emmmm问题很大";
    return a.exec();

}
