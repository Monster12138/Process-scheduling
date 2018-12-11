#ifndef _SYS_H_
#define _SYS_H_
#include "task.h"
#include <queue>
#include <list>
#include <iostream>
#include <unistd.h>

class sys
{
public:
    sys(int num = 0)
        :_task_num(num),
        _cur_num(0),
        _time(0),
        _cur_task(nullptr),
        _arr_task(new task[num]),
        _head(),
        _l()
    {}

    void set_task(){
        for(int i = 0; i < _task_num; ++i){
            int arv, svc, pri;
            std::string name;
            std::cout << "请输入第[" << i+1 << "]个作业的名称、到来时间、服务时间和优先级:>";
            std::cin >> name >> arv >> svc >> pri;
            _arr_task[i].set_time(name, arv, svc, pri);
        }
    }

    void print_res(){
        printf("┌──────┬────────┬────────┬────────┬────────┬────────┬────────────┐\n");
        printf("│进程名│到达时间│开始时间│执行时间│完成时间│周转时间│带权周转时间│\n");
        for(int i = 0; i < _task_num; ++i){
            printf("├──────┼────────┼────────┼────────┼────────┼────────┼────────────┤\n");
            printf("│%6s│%8d│%8d│%8d│%8d│%8d│%12.2f│\n",
                   _arr_task[i].get_name().c_str(),
                   _arr_task[i].get_arv_time(),
                   _arr_task[i].get_start_time(),
                   _arr_task[i].get_svc_time(),
                   _arr_task[i].get_fns_time(),
                   _arr_task[i].get_wle_time(),
                   _arr_task[i].get_wwle_time());
        }
        printf("└──────┴────────┴────────┴────────┴────────┴────────┴────────────┘\n");
    }

    void RR_run(int n = 2){
        init();
        const int time_slice = n;
        int cur_slice = 0;
        int cur_svc_time = 0;
        while(1){
            push_task();
            if(_cur_task && cur_slice == 0){
                if(!_l.empty())_l.pop_front();
                _cur_task->set_svc_time(cur_svc_time);
                if(cur_svc_time != 0)
                    _l.push_back(_cur_task);
                else {
                    std::cout << "作业[" << _cur_task->get_name() << "]执行完成————[" <<_time << "]s\n";
                    _cur_task->set_fns_time(_time);
                }
            }
            if(_cur_num == _task_num && _l.empty()){
                std::cout << "执行完成\n";
                break;
            }
            if(_cur_task == nullptr || cur_slice == 0){
                if(_l.empty()){
                    _cur_task = nullptr;
                }else{
                    _cur_task = _l.front();
                    cur_svc_time = _cur_task->get_svc_time_bak();
                    if(_cur_task->get_svc_time() == _cur_task->get_svc_time_bak())
                        _cur_task->set_start_time(_time);
                    std::cout << "当前队列:\n";
                    print_list();
                    std::cout << "作业[" << _cur_task->get_name() << "]开始执行————[" << _time << "]s\n";
                    if(cur_svc_time > time_slice){
                        cur_slice = time_slice;
                    }
                    else{
                        cur_slice = cur_svc_time;
                    }
                }
            }

            if(cur_slice > 0)cur_slice--;
            if(cur_svc_time > 0)cur_svc_time--;
            _time++;
            

        }
    }

    void SHR_run(){
        int cur_svc_time = 0;
        init();
        while(1){
            get_arv_task();

            if(_cur_task == nullptr || cur_svc_time == 0){
                _cur_task = get_min_task();
                if(_cur_task != nullptr){
                    std::cout << "作业[" << _cur_task->get_name() << "]开始执行————[" << _time << "]s\n";
                    cur_svc_time = _cur_task->get_svc_time();
                    _cur_task->set_start_time(_time);
                }
            }
            if(_cur_task == nullptr && _cur_num == _task_num){
                std::cout << "执行完成!\n";
                break;
            }

            if(cur_svc_time > 0)cur_svc_time--;
            _time++;
            if(cur_svc_time == 0 && _cur_task != nullptr){
                std::cout << "作业[" << _cur_task->get_name() << "]执行完成————[" <<_time << "]s\n";
                _cur_task->set_fns_time(_time);
            }
        }
    }

    void PR_run(){
        int cur_svc_time = 0;
        init();
        while(1){
            get_arv_task();

            if(_cur_task == nullptr || cur_svc_time == 0){
                _cur_task = get_max_pri();
                if(_cur_task != nullptr){
                    std::cout << "作业[" << _cur_task->get_name() << "]开始执行————[" << _time << "]s\n";
                    cur_svc_time = _cur_task->get_svc_time();
                    _cur_task->set_start_time(_time);
                }
            }
            if(_cur_task == nullptr && _cur_num == _task_num){
                std::cout << "执行完成!\n";
                break;
            }

            if(cur_svc_time > 0)cur_svc_time--;
            _time++;
            if(cur_svc_time == 0 && _cur_task != nullptr){
                std::cout << "作业[" << _cur_task->get_name() << "]执行完成————[" <<_time << "]s\n";
                _cur_task->set_fns_time(_time);
            }
        }
    }

    void RER_run(){
        int cur_svc_time = 0;
        init();
        while(1){
            get_arv_task();
            update_res_ratio();

            if(_cur_task == nullptr || cur_svc_time == 0){
                _cur_task = get_max_res_ratio();
                if(_cur_task != nullptr){
                    std::cout << "作业[" << _cur_task->get_name() << "]开始执行————[" << _time << "]s\n";
                    cur_svc_time = _cur_task->get_svc_time();
                    _cur_task->set_start_time(_time);
                }
            }
            if(_cur_task == nullptr && _cur_num == _task_num){
                std::cout << "执行完成!\n";
                break;
            }

            if(cur_svc_time > 0)cur_svc_time--;
            _time++;
            if(cur_svc_time == 0 && _cur_task != nullptr){
                std::cout << "作业[" << _cur_task->get_name() << "]执行完成————[" <<_time << "]s\n";
                _cur_task->set_fns_time(_time);
            }
        }
    }


    ~sys() 
    {
        if(_arr_task){
            delete[] _arr_task;
            _arr_task = nullptr;
        }

        _task_num = 0;
    }

private:
    int _task_num;      //总任务数
    int _cur_num;       //当前已到达任务数
    int _time;          //系统时间
    task *_cur_task;    //当前正在执行的作业
    task *_arr_task;    //全部作业数组首地址
    task _head;         //链表头结点
    std::list<task*> _l;//双向循环链表

    void init(){
        _time = 0;
        _cur_num = _time = 0;
        _cur_task = nullptr;
        _head._next = nullptr;
       _l.clear();
        for( int i = 0; i < _task_num; ++i ){
            _arr_task[i].init();
            _arr_task[i]._next = nullptr;
            _arr_task[i].set_svc_time(_arr_task[i].get_svc_time());
        }
    }

    void push_back(task * t){
        task *cur = &_head;
        while(cur->_next){
            cur = cur->_next;
        }
        cur->_next = t;
    }

   bool get_arv_task(){
        bool flag = false;
        for(int i = 0; i < _task_num; ++i){
            if(_time == _arr_task[i].get_arv_time()){
                push_back(&_arr_task[i]);
                _cur_num++;
                flag = true;
            }
        }
        return flag;
    }

    bool push_task(){
        bool flag = false;
        for(int i = 0; i < _task_num; ++i){
            if(_time == _arr_task[i].get_arv_time()){
                _l.push_back(&_arr_task[i]);
                _cur_num++;
                flag = true;
            }
        }
        return flag;
    }

    void erase_node(task *del){
        task *cur = &_head;
        while(cur->_next != del)
            cur = cur->_next;

        cur->_next = cur->_next->_next;
    }

    task* get_min_task(){
        if(_head._next == nullptr)return nullptr;
        task* cur = _head._next, *min_task;
        min_task = cur;
        cur = cur->_next;

        while(cur){
            if(cur->get_svc_time() < min_task->get_svc_time()){
                min_task = cur;
            }
            cur = cur->_next;
        }
        
        erase_node(min_task);
        return min_task;
    }

    task *get_max_pri(){
        if(_head._next == nullptr)return nullptr;
        task* cur = _head._next, *max_task;
        max_task = cur;
        cur = cur->_next;

        while(cur){
            if(cur->get_priority() > max_task->get_priority()){
                max_task = cur;
            }
            cur = cur->_next;
        }
        
        erase_node(max_task);
        return max_task;
    }

    void update_res_ratio(){
        if(nullptr == _head._next)return;
        task* cur = _head._next;
        while(cur){
           cur->set_res_ratio(_time); 
           cur = cur->_next;
        }
    }

     task *get_max_res_ratio(){
        if(_head._next == nullptr)return nullptr;
        task* cur = _head._next, *max_task;
        max_task = cur;
        cur = cur->_next;

        while(cur){
            if(cur->get_res_ratio() > max_task->get_res_ratio()){
                max_task = cur;
            }
            cur = cur->_next;
        }
        
        erase_node(max_task);
        return max_task;
    }

    void print_list(){
        if(_l.empty()){
            std::cout << "[null]\n";
            return;
        }
        auto it = _l.begin();
        std::cout << " [" << (*it)->get_name() << "] ";
        for(++it; it != _l.end(); ++it){
            std::cout << "——> [" << (*it)->get_name() << "] ";
        }
        std::cout << std::endl;
    }
    
    void print_list(int){
        if(nullptr == _head._next){
            std::cout << "[null]\n";
            return;
        }
        task* it = _head._next;
        std::cout << " [" << it->get_name() << "] ";
        for(it = it->_next; it != nullptr; it = it->_next){
            std::cout << "——> [" << it->get_name() << "] ";
        }
        std::cout << std::endl;
    }
    
};

#endif
