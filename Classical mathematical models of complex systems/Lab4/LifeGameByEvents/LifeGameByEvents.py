import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.animation import FuncAnimation, FFMpegWriter
import os
from datetime import datetime
 
#################################
#       КЛАСС ИГРЫ "ЖИЗНЬ"      #
#################################    

class LifeGameByEvents:
    """
        Класс игры "Жизнь" (клеточный автомат) на базе дискретных событий.
        ------------------------------------------------------------------
        n          : размерность матричной сетки (количество строк);
        m          : размерность матричной сетки (количество столбцов);
        random_init: флаг инициализации матричной сетки значениями 0 и 1 случайных клеток;
        p          : вероятность того, что каждая клетка изначально живая при random_init = True.
    """
    def __init__(self, n=10, m=10, random_init=True, p=0.5):
        # Матричная сетка размерности nxm
        self.n = n
        self.m = m
        self.current_field = (np.random.rand(n, m) < p).astype(int) if random_init else np.zeros((n, m), dtype=int) # Текущее поле
        self.next_field = np.zeros_like(self.current_field) # Следующее поле
        
        # Модельное время (виртуальное время: по смыслу - номер итерации моделирования)
        self.time = 0
        
        # Очередь событий
        self.events = []
        # Добавление в очередь событий события II типа
        self.events.append(EventScanField())

        # Статистика умерших и родившихся клеток
        self.current_dead_cells = 0
        self.current_born_cells = 0
        self.total_dead_cells = 0
        self.total_born_cells = 0

    # --------------------- #
    #   МЕТОДЫ ЛОГИКИ ИГРЫ  #
    # --------------------- #
    def count_neighbors_torodial(self, i, j):
        """ Функция подсчета соседей рассматриваемой клетки текущего поля (тороидальная сетка) """
        neighbors = 0
        for dx in (-1, 0, 1):
            for dy in (-1, 0, 1):
                if dx == 0 and dy == 0:
                    continue;
                ni = (i + dx) % self.n
                nj = (j + dy) % self.m
                neighbors += self.current_field[ni, nj]
        return neighbors
    
    def step_event(self):
        """ Реализация событий из очереди событий """
        ## -- КЛЕТКИ -- ##
        # Обновление цветов жизнеспособности клеток (относительно предыдушего события для визуальной обработки)
        self.update_colors()

        ## -- ЗАВЕРШЕНИЕ ИГРЫ -- ##
        # Если в очереди событий закончились события, то моделирование завершается
        if not self.events:
            print("[INFO] Все события из очереди событий были успешно обработаны.")
            return
        
        ## -- СОБЫТИЯ -- ##
        # Изъятие первого элемента из очереди событий 
        event = self.events.pop(0)
        # Обработка полученного события
        event.handle(self)
        
        ## -- ОБНОВЛЕНИЕ ИГРЫ -- ##
        # Если обработалось событие II типа или последнее событие I типа, то обновляем текущую модель 
        if isinstance(event, EventScanField) or self.I_events == self.n*self.m * self.steps:
            # Если модельное время != началу моделирования (т.к. иначе следующее поле не сформировано), то текущее поле готово к обновлению
            if (self.time):
                # Подсчет количества родившихся и умерших клеток
                born = np.sum((self.current_field == 0) & (self.next_field == 1))
                dead = np.sum((self.current_field == 1) & (self.next_field == 0))
                self.current_born_cells = born
                self.current_dead_cells = dead
                self.total_born_cells += born
                self.total_dead_cells += dead
                # Обновление текущего поля
                self.current_field[:, :] = self.next_field
            # Обнуление жизнеспособности клеток следующего поля
            self.next_field[:] = 0
            if (self.time != self.steps):
                # Увеличение модельного времени на 1
                self.time += 1

    # --------------------- #
    # АНИМАЦИЯ И СОХРАНЕНИЕ #
    # --------------------- #
    def highlight_cell(self, i, j):
        """ Подсветка рассматриваемой клетки текущего поля """
        self.rects[i][j].set_facecolor(self.color_scan)

    def update_colors(self):
        """ Обновление цветов жизнеспособности клеток """
        for i in range(self.n):
            for j in range(self.m):
                color = self.color_alive if self.current_field[i,j] else self.color_dead
                self.rects[i][j].set_facecolor(color)

    def update_display(self, frame):
        """ Функция обновления для анимации. 
            --------------------------------
            ## ПРИМЕЧАНИЕ:
            # frame - это кадр, используемый FuncAnimation для запуска переданной функции (update_display) frames раз;
            # Поскольку используемуе устройство не позволяет открывать окно plot'а моментально, то frames начинают считаться спустя примерно 3 кадра после открытия окна.
            # Иными словами, они воспроизводятся корректно, но считаются с задержкой. Поэтому к текущему frame прибавляется значение +3 для корректности отображения.
            # Также к frame прибавляется +1, т.к. отсчет начинается с 0.
        """
        ## -- ШАГ -- ##
        # Обработка текущего события из очереди событий
        self.step_event()
        
        ## -- ТЕКСТОВОЕ ПОЛЕ -- ##
        # Обновление содержимого текстового поля
        text_str = (f"Поколение (модельное время): {self.time}/{self.steps}"
                    f"\nframe (текущий кадр): {frame + 3 + 1}/{(self.n*self.m+1)*self.steps}"
                    f"\nI тип (события): {self.I_events}/{self.n*self.m * self.steps}"
                    f"\nII тип (события): {self.II_events}/{self.steps}"
                    f"\nРодившиеся клетки (сейчас): {self.current_born_cells}"
                    f"\nУмершие клетки (сейчас): {self.current_dead_cells}"
                    f"\nРодившиеся клетки (в целом): {self.total_born_cells}"
                    f"\nУмершие клетки (в целом): {self.total_dead_cells}"
                    f"\n\nМатричная сетка {self.n}x{self.m} (текущее поле):\n" + '\n'.join([' '.join(map(str, row)) for row in self.current_field]))
        self.text_display.set_text(text_str)
        
        ## -- ОБНОВЛЕНИЕ ОТОБРАЖЕНИЯ -- ##
        # Возвращаем все объекты (необходимо в случае blit=True)
        return [rect for row in self.rects for rect in row] + self.grid_lines + [self.text_display]

    def run_animation(self, steps=50, interval=300, save_results=False, fps=10):
        """ Запуск анимации игры.
            ---------------------
            ## ПРИМЕЧАНИЕ:
            # В виду примечания, описанного в функции update_display, здесь количество кадров (frames) уменьшается на 3.
        """
        ## -- КОЛИЧЕСТВО ОБРАБОТАННЫХ СОБЫТИЙ I И II ТИПА, КОЛИЧЕСТВО ПОКОЛЕНИЙ, ЧАСТОТА ОБНОВЛЕНИЯ И СОХРАНЕНИЕ РЕЗУЛЬТАТОВ -- ##
        self.I_events = 0
        self.II_events = 0
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
        # Зеленый - живая клетка
        self.color_alive = 'green'
        # Белый - мертвая клетка
        self.color_dead = 'white'
        # Светлосерый - сканируемая клетка
        self.color_scan = 'lightgray'
        
        ## -- ВИЗУАЛИЗАЦИЯ -- ##
        # Полотно и оси для матричной сетки и текстового поля
        self.fig = plt.figure(figsize=(10,5))
        grid_left, grid_bottom, grid_width, grid_height, gap = -0.1, 0.05, 0.7, 0.9, 0.1
        self.ax_grid = self.fig.add_axes([grid_left, grid_bottom, grid_width, grid_height])
        self.ax_text = self.fig.add_axes([grid_left + grid_width - gap, grid_bottom, 1 - (grid_left + grid_width - gap), grid_height])
        self.fig.canvas.manager.set_window_title("4199_ГоршковАлО_ЛР4_КлеточныйАвтоматНаБазеДискретныхСобытий_ИграЖизнь")
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
        # Инициализация (все клетки мертвы)
        self.rects = [[None for _ in range(self.m)] for _ in range(self.n)]
        for i in range(self.n):
            for j in range(self.m):
                rect = Rectangle((j, self.n-1-i), 1, 1, facecolor=self.color_dead)
                self.ax_grid.add_patch(rect)
                self.rects[i][j] = rect
        # Обновление (в зависимости от инициализации текущего поля)
        self.update_colors()        
                
        ## -- ТЕКСТОВОЕ ПОЛЕ -- ##
        # Текстовое поле (содержимое + настройки отображения)
        self.text_display = self.ax_text.text(0, 1, '', va='top', ha='left', fontsize=10, family='monospace')

        ## -- АНИМАЦИЯ -- ##
        print("[INFO] Анимация модели клеточного автомата на базе дискретных событий была запущена...")
        ani = FuncAnimation(self.fig, self.update_display, frames=(self.n*self.m+1)*steps - 3, interval=interval, blit=True, repeat=False)

        ## -- СОХРАНЕНИЕ АНИМАЦИИ В .MP4 -- ##
        if (self.save_results):
            writer = FFMpegWriter(fps=fps, metadata={"artist": "LifeGame"})
            ani.save(video_path, writer=writer, dpi=150)
            print(f"[INFO] Анимация модели клеточного автомата сохранена в файл: {video_path}")

        ## -- ОТОБРАЖЕНИЕ АНИМАЦИИ -- ##
        plt.show()
        
        ## -- ЗАВЕРШЕНИЕ АНИМАЦИИ -- ##
        print("[INFO] Анимация модели клеточного автомата на базе дискретных событий была завершена...\n")

#################################
# КЛАССЫ СОБЫТИЙ И ИХ ОБРАБОТКИ #
#################################       
class Event:
    """ Интерфейс-обработчик событий.
        -----------------------------
        handle: метод, который необходимо реализовать в классах-реализациях интерфейса.
    """
    def handle(self, game: LifeGameByEvents):
        raise NotImplementedError
    
class EventScanField(Event):
    """ Класс-обработчик события II типа.
        ---------------------------------
        handle: обработчик события II типа (порождает события I типа).
    """
    def handle(self, game: LifeGameByEvents):
        # Увеличение счетчика событий II типа
        game.II_events += 1
        # Порождение событий I типа для всех клеток матричной сетки
        for i in range(game.n):
            for j in range(game.m):
                game.events.append(EventScanCell(i,j))
        # Добавление события II типа для зацикливания
        if (game.II_events < game.steps):        
            game.events.append(EventScanField())
        
class EventScanCell(Event):
    """ Класс-обработчик события I типа.
        --------------------------------
        handle: обработчик события I типа (в соответствии с правилами игры);
        i     : номер строки клетки текущего поля;
        j     : номер столбца клетки текущего поля.
    """
    def __init__(self, i, j):
        self.i = i
        self.j = j
        
    def handle(self, game: LifeGameByEvents):
        """ Обновление текущего поля за единицу модельного времени (одновременно) по правилам игры.
        -------------------------------------------------------------------------------------------
        Основные правила (для тороидальной матричной сетки):  
        • ячейка остается живой, если ее окружают две или три живые ячейки;  
        • ячейка умирает от перенаселения, если ее окружает более трех живых ячеек; 
        • ячейка умирает от изоляции, если у нее меньше двух живых соседей; 
        • мертвая ячейка становится живой, если она имеет ровно три живых соседа.
    """
        # Увеличение счетчика событий I типа
        game.I_events += 1
        # Подсчет количества соседей клетки текущего поля
        neighbors = game.count_neighbors_torodial(self.i, self.j)
        # Определение жизнеспособности клетки следующего поля на основе клетки текущего поля
        if game.current_field[self.i, self.j] == 1:
            game.next_field[self.i, self.j] = 1 if neighbors in (2, 3) else 0
        else:
            game.next_field[self.i, self.j] = 1 if neighbors == 3 else 0
        # Подсветка рассматриваемой клетки текущего поля
        game.highlight_cell(self.i, self.j)

#################################
#       ЗАПУСК ПРОГРАММЫ        #
#################################    
if __name__ == '__main__':
    game = LifeGameByEvents(n=10, m=10, random_init=True, p=0.2)
    game.run_animation(steps=5, interval=1, save_results=False, fps=60)