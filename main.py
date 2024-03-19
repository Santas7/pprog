import numpy as np
import matplotlib.pyplot as plt


def check_multiply() -> None:
    """
    функция для проверки перемножения матриц
    :return: None
    """
    for size in [50, 100, 200, 250, 500, 750, 1000]:
        if np.array_equal(np.dot(*[np.loadtxt(f'data/matrix{name}_{size}.txt') \
            for name in ['1', '2']]), np.loadtxt(f'data/multiply_result_{size}.txt')):
            print(f'Размеры {size} совпадают!')
        else:
            print(f'Размеры {size} не совпадают!')


def draw_graph() -> None:
    """
    Функция для построения графика зависимости среднего времени перемножения матриц
    от размера самих матриц.
    """
    sizes = []
    times = []
    with open('data/avg_times.txt', 'r') as file:
        for line in file:
            parts = line.strip().split(' - ')
            size, mean_time = parts[0], parts[1].split(': ')[1].split(' ms')[0]
            sizes.append(int(size))
            times.append(float(mean_time))
    plt.plot(sizes, times, marker='o', linestyle='-')
    plt.title('Среднее время')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время (мс)')
    plt.grid(True)
    plt.show()

if __name__ == '__main__':
    check_multiply()
    draw_graph()
