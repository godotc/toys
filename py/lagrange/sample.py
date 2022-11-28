
#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import time

#print(plt.__file__ )


def lagrange(X, Y, xx):
    """拉格朗日插值法"""
    result = 0.0
    for i in range(len(Y)):

        f_temp = Y[i]
        for j in range(len(Y)):
            if i != j:
                f_temp *= (xx - X[j]) / (X[i] - X[j])

        result += f_temp

    return result


def plot_image(X, Y, xq, yq, num):
    # 绘图
    plt.title("Lagrange_interpolation")  # 打印标题
    plt.plot(X, Y, 's', label="original values")  # 蓝色点表示原来的值
    plt.plot(xq, yq, 'r', label="interpolation values")  # 插值曲线
    words = "被插点数：{}".format(num)
    plt.text(3.5, -10, words, fontproperties='SimHei')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.legend(loc=4)  # 指定lgend的位置
    plt.savefig('lagrange.png')
    plt.show()


def main():
    # 开始计时
    start_time = time.clock_gettime(0)
    # 已30
    X = [-1, 0, 1, 2, 3, 4, 5]
    Y = [-20, -12, 1, 15, 4, 21, 41]
    # 计算的插值点
    num = 50  # 被插点数
    xq = np.linspace(np.min(X), np.max(X), num, endpoint=True)
    yq = []
    for xx in xq:
        yq.append(lagrange(X, Y, xx))

    # 绘图
    plot_image(X, Y, xq, yq, num)

    # 结束计时
    end_time = time.clock_gettime(0)
    consumer_time = end_time - start_time
    print("程序运行消耗时间: " + str(consumer_time))


if __name__ == '__main__':
    main()
