#ifndef _TASK_H_
#define _TASK_H_
#include <string>

class task
{
public:
    task(std::string name = std::string(), 
         int arv = 0,
         int svc = 0,
         int priority = 0)
        :_next(nullptr),
        _name(name),
        _arv_time(arv),
        _start_time(0),
        _svc_time(svc),
        _svc_time_bak(svc),
        _priority(priority),
        _res_ratio(0),
        _fns_time(0),
        _wle_time(0),
        _wwle_time(0)
    {}

    void set_time(std::string name = std::string(), int arv = 0,int svc = 0, int pri = 0){
        _arv_time = arv;
        _svc_time = svc;
        _svc_time_bak = _svc_time;
        _priority = pri;
        _name = name;
    }

    void init(){
        _start_time = 0;
        _fns_time = 0;
        _wle_time = 0;
        _wwle_time = 0;
        _res_ratio = 0;
    }

    std::string get_name(){ return _name; }

    int get_arv_time(){ return _arv_time; }

    int get_svc_time(){ return _svc_time; }

    int get_svc_time_bak(){ return _svc_time_bak; }

    void set_svc_time(int svc_time){ _svc_time_bak = svc_time; }

    int get_priority(){ return _priority; }

    float get_res_ratio(){ return _res_ratio; }

    int get_start_time(){ return _start_time; }

    int get_fns_time(){ return _fns_time; }

    int get_wle_time(){ return _wle_time; }

    float get_wwle_time(){ return _wwle_time; }

    bool is_finish(int time){ return _arv_time + _svc_time >= time; }

    void set_start_time(int time){ _start_time = time; }

    void set_res_ratio(int time) {
        if(0 == _svc_time)return;
        _res_ratio = 1 + (time - _start_time)/((float)_svc_time);
    }

    void set_fns_time(int time){ 
        _fns_time = time; 
        _wle_time = _fns_time - _arv_time;
        if(0 == _svc_time)return;
        _wwle_time = _wle_time/(float)_svc_time;
    }
    task *_next;
private:
    std::string _name;   //作业名
    int _arv_time;  //到达时间
    int _start_time;//开始时间
    int _svc_time;  //服务时间
    int _svc_time_bak;//服务时间备份
    int _priority;  //优先级
    float _res_ratio; //响应比
    int _fns_time;  //完成时间
    int _wle_time;  //周转时间
    float _wwle_time; //带权周转时间

    task(const task& t);    //禁用拷贝构造和赋值运算符重载
    task& operator=(const task& t);
};
#endif
