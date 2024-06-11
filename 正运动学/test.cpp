#include <mujoco/mujoco.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

char error[1000];

void PrintPos(const mjModel* m, mjData* d)
{
    for (int i = 0; i < m->nbody; i++) {
        mjtNum * xpos = d->xpos + 3 * i;
        printf("[%d]:\t%f\t%f\t%f\n", i, xpos[0], xpos[1], xpos[2]);
    }
}


int main()
{
    mjModel *m = mj_loadXML("box_arm.xml", NULL, error, 1000);
    mjData *d = mj_makeData(m);

    std::cout << "刚体数量: " << m->nbody << std::endl;
    std::cout << "关节数量: " << m->njnt << std::endl;
    std::cout << "各刚体初始位置: " << std::endl;

    mj_kinematics(m, d);
    PrintPos(m, d);

    std::cout << "-- d->qpos[0] = M_PI * 0.5 --" << std::endl;
    d->qpos[0] = M_PI * 0.5;
    mj_kinematics(m, d);
    PrintPos(m, d);

    std::cout << "-- d->qpos[1] = M_PI * 0.5 --" << std::endl;
    d->qpos[1] = M_PI * 0.5;
    mj_kinematics(m, d);
    PrintPos(m, d);

    std::cout << "-- d->qpos[0] = 0 --" << std::endl;
    d->qpos[0] = 0;
    mj_kinematics(m, d);
    PrintPos(m, d);
    
    mj_deleteData(d);
    mj_deleteModel(m);
    return 0;
}
