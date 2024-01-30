# [MuJoCo仿真引擎](https://gaoyichao.com/Xiaotu/?book=MuJoCo仿真引擎&title=index)

MuJoCo 是一个通用的物理引擎，全称是 Multi-Joint dynamics with Contact。 最初由 Roboti LLC 开发，在2021年10月份被 DeepMind 收购，2022年5月开源。

* 官网: https://mujoco.org/
* GitHub仓库: https://github.com/deepmind/mujoco
* 官方文档: https://mujoco.readthedocs.io/en/stable/overview.html
* 原 Roboti LLC: https://www.roboti.us/index.html

物理引擎传统上分为两类。机器人和生物力学引擎在广义或关节坐标中使用高效且准确的递归算法。
然而，它们要么忽略了接触动力学，要么依赖于早期的弹簧阻尼器方法。此类引擎需要非常小的仿真步长。
游戏引擎使用更现代的方法，通过解决优化问题来找到接触力。然而，它们经常采用笛卡尔坐标系，
通过数值的方法描述关节约束，当涉及复杂的运动结构时，会导致不准确和不稳定。
MuJoCo 是第一个将两者结合起来的通用物理引擎，广义坐标模拟 + 基于优化的接触动力学。

[MuJoCo仿真引擎](https://gaoyichao.com/Xiaotu/?book=MuJoCo仿真引擎&title=index)系列文章是对 MuJoCo 的源码解读。
本仓库是各篇文章中的例程。我们尽量做到一篇文章对应一个子目录，各个子目录均可以独立编译


