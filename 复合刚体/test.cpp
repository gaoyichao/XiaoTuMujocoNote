#include <mujoco/mujoco.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <Eigen/Eigen>

char error[1000];

/**
 * @brief 获取指定 id 的刚体名称
 * 
 * @param [in] m 模型对象
 * @param [in] bid 刚体索引
 * @return 刚体名称
 */
char const * GetBodyName(mjModel const * m, int bid)
{
    int name_bodyadr = m->name_bodyadr[bid];
    return m->names + name_bodyadr;
}

/**
 * @brief 获取指定 id 的关节名称
 * 
 * @param [in] m 模型对象
 * @param [in] jid 关节索引
 * @return 关节名称
 */
char const * GetJointName(mjModel const * m, int jid)
{
    int name_jointadr = m->name_jntadr[jid];
    return m->names + name_jointadr;
}


void PrintPos(mjModel const * m, mjData* d)
{
    for (int i = 0; i < m->nbody; i++) {
        mjtNum * xpos = d->xpos + 3 * i;
        printf("[%d]%s:\t%f\t%f\t%f\n", i, GetBodyName(m, i), xpos[0], xpos[1], xpos[2]);
    }
}

void PrintInert(mjModel const * m, mjData * d, int bid)
{
    std::cout << std::endl;
    mjtNum * cinert = d->cinert + 10 * bid;
    std::cout <<  cinert[0] << "\t" << -cinert[3] << "\t" << -cinert[4] << std::endl
              << -cinert[3] << "\t" <<  cinert[1] << "\t" << -cinert[5] << std::endl
              << -cinert[4] << "\t" << -cinert[5] << "\t" <<  cinert[2] << std::endl;
}


int main(int argc, char * argv[])
{
    if (2 != argc) {
        std::cout << "test /path/to/model.xml" << std::endl;
        return -1;
    }
    std::string path = argv[1];
    std::cout << "加载模型:" << path << std::endl;

    mjModel *m = mj_loadXML(path.c_str(), NULL, error, 1000);
    mjData *d = mj_makeData(m);

    std::cout << "刚体数量: " << m->nbody << std::endl;

    for (int i = 0; i < m->nbody; i++) {
        char const * name = GetBodyName(m, i);
        printf("刚体 %d: %s \n", i, name);
        printf("    body_parentid: %d\n", m->body_parentid[i]);
        printf("    root_id      : %d\n", m->body_rootid[i]);
        printf("    质量         : %f\n", m->body_mass[i]);
        printf("    inertia     : %f, %f, %f\n", m->body_inertia[3 * i + 0],
                                                 m->body_inertia[3 * i + 1],
                                                 m->body_inertia[3 * i + 2]);
    }

    std::cout << "关节数量: " << m->njnt << std::endl;
    for (int i = 0; i < m->njnt; i++) {
        char const * name = GetJointName(m, i);
        printf("关节 %d: %s \n", i, name);
        printf("    jnt_bodyid: %d, %s\n",
                m->jnt_bodyid[i], GetBodyName(m, m->jnt_bodyid[i]));
        printf("    dof_jnt   : %d\n", m->dof_jntid[i]);
    }

    std::cout << "1. 正运动学: " << std::endl;
    mj_kinematics(m, d);
    PrintPos(m, d);

    std::cout << "2. 刚体质心: " << std::endl;
    mj_comPos(m, d);
    for (int i = 0; i < m->nbody; i++) {
        char const * name = GetBodyName(m, i);
        printf("刚体 %d: %s\n", i, name);
        printf("    mass: %f\n", d->cinert[10 * i + 9]);
    }
    PrintInert(m, d, 7);

    std::cout << "3. 复合刚体: " << std::endl;
    mj_crb(m, d);
    
    int nv = m->nv;
    std::cout << "速度自由度: " << nv << std::endl;
    Eigen::MatrixXd qm = Eigen::MatrixXd::Zero(nv, nv);

    for (int i=0; i < nv; i++) {
        int Madr_ij = m->dof_Madr[i];
        for (int j=i; j >= 0; j = m->dof_parentid[j]) {
            std::cout << "(" << i << "," << j << ") "; 
            qm(i, j) = d->qM[Madr_ij++];
        }
        std::cout << std::endl;
    }
    std::cout << qm << std::endl;
    
    mj_deleteData(d);
    mj_deleteModel(m);
    return 0;
}
