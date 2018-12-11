#include <iostream>
#include "sys.h"

using namespace std;

void menu(){
    int count;
    cout << "请输入作业道数：>";
    cin >> count;
    sys s(count);
    s.set_task();
    cout << "1.短进程优先调度\n2.时间片轮转调度\n3.优先级调度\n4.高响应比优先调度\n0.退出\n";
    while(1){
        cout << "请输入选项:>";
        char c;
        cin >> c;
        switch(c){
        case '1':s.SHR_run();break;
        case '2':{
                     int n;
                     cout << "请输入时间片大小:";
                     cin >> n;
                     s.RR_run(n);
                     break;
                 }
        case '3':s.PR_run();break;
        case '4':s.RER_run();break;
        case '5':exit(0);
        default:cout << endl;
        }
        s.print_res();
    }
}

int main()
{
    menu();
    return 0;
}
