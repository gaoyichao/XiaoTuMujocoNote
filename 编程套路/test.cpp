#include <mujoco/mujoco.h>
#include <iostream>

char error[1000];
int main()
{
    mjModel *m = mj_loadXML("hello.xml", NULL, error, 1000);
    mjData *d = mj_makeData(m);

    while(d->time < 10)
       mj_step(m, d);

    mj_deleteData(d);
    mj_deleteModel(m);
    return 0;
}
