# utf-8
import numpy as np
import matplotlib.pyplot as plt
import time


def lagrange(X, Y, xx):
    "拉格朗日插值法"
    result = 0.0
    for i in range(len(Y)):

        tmp = Y[i]
        for j in range(len(Y)):

            if i != j:
                # y * (x-x0) / (x1- x2)
                tmp *= (xx-X[j])/(X[i]-X[j])
        result += tmp
    return result


def plot_image(X, Y, xq, yq, num):
    # 绘图
    plt.title("Lagrange_interpolation")  # 打印标题
    plt.plot(X, Y, 's', label="original values")  # 蓝色点表示原来的值
    plt.plot(xq, yq, 'r', label="interpolation values")  # 插值曲线
    words = "被插点数：{}".format(num)
    plt.text(3.5, -10, words, fontproperties='FreeSerif')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.legend(loc=4)  # 指定lgend的位置
    plt.savefig('lagrange.png')
    plt.show()


def main():

    start_time = time.clock_gettime(0)

    X = [-1, 0, 1, 2, 3, 4, 5]
    Y = [-20, -12, 1, 15, 4, 21, 41]

    # 计算插值点
    num = 50  # 插值点数
    xq = np.linspace(np.min(X), np.max(X), num, endpoint=True)
    yq = []
    for xx in xq:
        yq.append(lagrange(X, Y, xx))

    plot_image(X, Y, xq, yq, num)

    end_time = time.clock_gettime(0)
    consumer_time = end_time-start_time
    print("Total cost time:" + str(consumer_time))


if __name__ == '__main__':
    main()
