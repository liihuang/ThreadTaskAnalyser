#include "Record.h"

int main(){
    std::string testStr("           <idle>-0     (-----) [003] d..3 2074248.139074: sched_switch: prev_comm=swapper/3 prev_pid=0 prev_prio=120 prev_state=R ==> next_comm=rcu_sched next_pid=8 next_prio=120");
    Record testRec(testStr);
    testRec.print();
    return 0;
}