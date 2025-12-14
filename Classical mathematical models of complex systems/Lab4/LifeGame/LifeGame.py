import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.animation import FuncAnimation, FFMpegWriter
import os
from datetime import datetime

#################################
#       КЛАСС ИГРЫ "ЖИЗНЬ"      #
#################################  
class LifeGame:
    """
        Класс игры "Жизнь" (клеточный автомат).
        ---------------------------------------
        n          : размерность матричной сетки (количество строк);
        m          : размерность матричной сетки (количество столбцов);
        random_init: флаг инициализации матричной сетки случайными 0 и 1;
        p          : вероятность того, что каждая клетка изначально живая при random_init = True.
    """
    def __init__(self, n=10, m=10, random_init=True, p=0.5):
        # Матричная сетка размерности nxm
        self.n = n
        self.m = m
        self.matrix = (np.random.rand(n, m) < p).astype(int) if random_init else np.zeros((n, m), dtype=int)
    
    # --------------------- #
    #   МЕТОДЫ ЛОГИКИ ИГРЫ  #
    # --------------------- #
    def _count_neighbors(self, i, j):
        """ Функция подсчета соседей текущей клетки матричной сетки """
        neighbors = 0
        for x in range(i-1, i+2):
            for y in range(j-1, j+2):
                if 0 <= x < self.n and 0 <= y < self.m:
                    neighbors += self.matrix[x,y]
        neighbors -= self.matrix[i,j] # Текущая клетка не учитывается при подсчете соседей
        return neighbors

    def _step(self):
        """ Обновление матричной сетки за один шаг по правилам игры.
            --------------------------------------------------------
            Основные правила:  
            • ячейка остается живой, если ее окружают две или три живые ячейки;  
            • ячейка умирает от перенаселения, если ее окружает более трех живых ячеек; 
            • ячейка умирает от изоляции, если у нее меньше двух живых соседей; 
            • мертвая ячейка становится живой, если она имеет ровно три живых соседа.
        """
        new_matrix = np.zeros_like(self.matrix)
        for i in range(self.n):
            for j in range(self.m):
                neighbors = self._count_neighbors(i,j)
                if self.matrix[i,j] == 1:
                    if neighbors in [2,3]:
                        new_matrix[i,j] = 1
                    else:
                        new_matrix[i,j] = 0
                else:
                    if neighbors == 3:
                        new_matrix[i,j] = 1
        self.matrix = new_matrix

    # --------------------- #
    # АНИМАЦИЯ И СОХРАНЕНИЕ #
    # --------------------- # 
    def _update_display(self, frame):
        """ Функция обновления для анимации """
        ## -- ШАГ -- ##
        self._step()
        
        ## -- КЛЕТКИ -- ##
        # Раскраска клеток матричной сетки
        for i in range(self.n):
            for j in range(self.m):
                self.rects[i][j].set_facecolor(self.colors[self.matrix[i,j]])
        # Подсчет количества живых клеток
        alive_cells = np.sum(self.matrix)        

        ## -- ТЕКСТОВОЕ ПОЛЕ -- ##
        # Обновление содержимого текстового поля        
        text_str = (f"Поколение (модельное время): {frame + 1}/{self.steps}"
                    f"\nЧастота обновления: {self.interval} мс"
                    f"\nЖивые клетки (количество): {alive_cells}/{self.n*self.m}"
                    f"\n\nМатричная сетка {self.n}x{self.m}:\n\n" + '\n'.join([' '.join(map(str, row)) for row in self.matrix]))
        self.text_display.set_text(text_str)   

        ## -- ОБНОВЛЕНИЕ ОТОБРАЖЕНИЯ -- ##
        # Возвращение всех объектов (необходимо в случае blit=True)
        return [rect for row in self.rects for rect in row] + self.grid_lines + [self.text_display]

    def run_animation(self, steps=50, interval=300, save_results=False, fps=10):
        """ Анимация игры 'Жизнь' """
        ## -- КОЛИЧЕСТВО ПОКОЛЕНИЙ, ЧАСТОТА ОБНОВЛЕНИЯ И СОХРАНЕНИЕ РЕЗУЛЬТАТОВ -- ##
        self.steps = steps
        self.interval = interval
        self.save_results = save_results

        ## -- ПРЕДУПРЕЖДЕНИЕ -- ##
        if save_results:
            print("[WARN] С учетом длительности сохранения результатов в формат .mp4 и реализацией программы (FuncAnimation) видимость результатов на полотне не гарантируется!")
            print("[INFO] Проверьте результаты, открыв сохраненный .mp4 файл.")

        ## -- ПАРАМЕТРЫ СОХРАНЕНИЯ РЕЗУЛЬТАТОВ -- ##
        if (save_results):
            ## -- КАТАЛОГИ -- ##
            base_dir = "animation_results"
            os.makedirs(base_dir, exist_ok=True)
        
            timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
            self.run_dir = os.path.join(base_dir, timestamp)
            os.makedirs(self.run_dir, exist_ok=True)
        
            video_path = os.path.join(self.run_dir, "animation.mp4")

        ## -- ПАЛИТРА ЦВЕТОВ -- ##    
        # Палитра цветов: 0 - белый (мертвая клетка), 1 - серый (живая клетка)
        self.colors = {0: 'white', 1: 'gray'}
        
        ## -- ВИЗУАЛИЗАЦИЯ -- ##
        # Полотно и оси для матричной сетки и текстового поля
        self.fig = plt.figure(figsize=(10,5))
        grid_left, grid_bottom, grid_width, grid_height, gap = -0.1, 0.05, 0.7, 0.9, 0.1
        self.ax_grid = self.fig.add_axes([grid_left, grid_bottom, grid_width, grid_height])                                            # Для матричной сетки
        self.ax_text = self.fig.add_axes([grid_left + grid_width - gap, grid_bottom, 1 - (grid_left + grid_width - gap), grid_height]) # Для текстового поля
        self.fig.canvas.manager.set_window_title("4199_ГоршковАлО_ЛР4_КлеточныйАвтомат_ИграЖизнь")
        # Настройка и масштабирование осей для матричной сетки и текстового поля
        self.ax_grid.set_xlim(0, self.m)
        self.ax_grid.set_ylim(0, self.n)
        self.ax_grid.set_aspect('equal')
        self.ax_grid.set_xticks([])
        self.ax_grid.set_yticks([])
        self.ax_text.axis('off')
        
        ## -- СЕТКА -- ##
        # Внутренние границы матричной сетки с сохранением для перерисовки при анимации (крайние границы отрисовываются отдельно)
        self.grid_lines = []
        for i in range(1, self.n):
            hline = self.ax_grid.axhline(i, color='black', linewidth=0.5)
            self.grid_lines.append(hline)
        for j in range(1, self.m):
            vline = self.ax_grid.axvline(j, color='black', linewidth=0.5)
            self.grid_lines.append(vline)
        # Крайние границы матричной сетки с сохранением для перерисовки при анимации (внутренние границы уже отрисованы)
        self.grid_lines.extend([
            self.ax_grid.axhline(0, color='black', linewidth=2),
            self.ax_grid.axhline(self.n, color='black', linewidth=2),
            self.ax_grid.axvline(0, color='black', linewidth=2),
            self.ax_grid.axvline(self.m, color='black', linewidth=2)
        ])
        
        ## -- КЛЕТКИ -- ##
        # Раскраска клеток матричной сетки
        self.rects = [[None for _ in range(self.m)] for _ in range(self.n)]
        for i in range(self.n):
            for j in range(self.m):
                rect = Rectangle((j, self.n-1-i), 1, 1, facecolor=self.colors[self.matrix[i,j]])
                self.ax_grid.add_patch(rect)
                self.rects[i][j] = rect
                
        ## -- ТЕКСТОВОЕ ПОЛЕ -- ##
        # Текстовое поле (содержимое + настройки отображения)
        self.text_display = self.ax_text.text(0, 1, '', va='top', ha='left', fontsize=10, family='monospace')
        
        ## -- АНИМАЦИЯ -- ##
        print("[INFO] Анимация модели клеточного автомата была запущена...")
        ani = FuncAnimation(self.fig, self._update_display, frames=steps, interval=interval, blit=True, repeat=False)
        
        ## -- СОХРАНЕНИЕ АНИМАЦИИ В .MP4 -- ##
        if (self.save_results):
            writer = FFMpegWriter(fps=fps, metadata={"artist": "LifeGame"})
            ani.save(video_path, writer=writer, dpi=150)
            print(f"[INFO] Анимация модели клеточного автомата сохранена в файл: {video_path}")
            
        ## -- ОТОБРАЖЕНИЕ АНИМАЦИИ -- ##
        plt.show()
        
        ## -- ЗАВЕРШЕНИЕ АНИМАМЦИИ -- ##
        print("[INFO] Анимация модели клеточного автомата была завершена...\n")

#################################
#       ЗАПУСК ПРОГРАММЫ        #
#################################    
if __name__ == '__main__':
    game = LifeGame(n=30, m=25, random_init=True, p=0.5)
    game.run_animation(steps=30, interval=300, save_results=False, fps=5)