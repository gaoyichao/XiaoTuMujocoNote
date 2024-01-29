#include <mujoco/mujoco.h>

#include <iostream>

char error[1000];
mjModel* m;
mjData* d;

int main()
{
    m = mj_loadXML("hello.xml", NULL, error, 1000);
    if( !m ) {
        printf("%s\n", error);
        return 1;
    }

   // make data corresponding to model
    d = mj_makeData(m);

    // run simulation for 10 seconds
    while(d->time < 10)
       mj_step(m, d);

    // free model and data
    mj_deleteData(d);
    mj_deleteModel(m);

    std::cout << "hahahha" << std::endl;
    std::cout << "version:" << mj_versionString() << std::endl;
    std::cout << "mjVERSION_HEADER:" << mjVERSION_HEADER << std::endl;
    return 0;
}
