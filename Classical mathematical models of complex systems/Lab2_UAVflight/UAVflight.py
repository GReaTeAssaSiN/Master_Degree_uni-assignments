from math import exp, cos, sin
from matplotlib.pyplot import show, subplots
import numpy as np

############################
# МОДЕЛИРОВАНИЕ ПОЛЕТА БЛА #
############################
class FlightModelSimulator:
    """ Моделирование полета БЛА.
        - V_air          : воздушная скорость БЛА;
        - V_wind         : скорость ветра;
        - K_angle        : курсовой угол БЛА;
        - wind_angle     : угол ветра;
        - A_angle        : угол атаки БЛА;
        - flight_duration: временной промежуток моделирования полета БЛА (продолжительность);
        - x              : координаты положения БЛА по долготе (м);
        - y              : координаты положения БЛА по широте (м).
    """
    def __init__(self, V_air, V_wind, K_angle, wind_angle, A_angle, flight_duration):
        self.V_air = V_air
        self.V_wind = V_wind
        self.K_angle = K_angle
        self.wind_angle = wind_angle
        self.A_angle = A_angle
        self.duration = flight_duration
        self.x = []
        self.y = []
        
    def simulate(self):
        """ Вычисляет траеторию БЛА """
        # Цикл вычисления значений
        for t in range(self.duration):
            V_air = self.V_air
            V_wind = self.V_wind
            K = np.deg2rad(self.K_angle)
            A = np.deg2rad(self.A_angle)
            d = np.deg2rad(self.wind_angle)
            
            # Вычисление смещений по долготе и широте
            dx = (V_air * cos(K) + V_wind * cos(d)) * cos(A)
            dy = (V_air * sin(K) + V_wind * sin(d)) * cos(A)
            
            # Если это начальный момент времени, то заносим в массивы значения
            if (t == 0):
                self.x.append(dx)
                self.y.append(dy)
            else: # Иначе синтетически моделируем за счет приращения координат
                self.x.append(self.x[-1] + dx)
                self.y.append(self.y[-1] + dy)
        return self.x, self.y

############################
#  Ошибка без GPS и с ним  #
############################
class GPSErrorSimulator:
    """ Вычисление ошибки (без комплексирования и с комплексированием - GPS).
        - GPS_period      : период видимости спутника (GPS);
        - GPS_duration    : время видимости спутника (GPS);
        - k               : основной параметр комплексирования;
        - C               : коэффициент роста квадратичной зависимости ошибки определения положения координат от времени полета;
        - flight_duration : временной промежуток моделирования полета БЛА (продолжительность);
        - dt              : временной шаг моделирования.
    """
    def __init__(self, GPS_period, GPS_duration, k, C, flight_duration, dt):
        self.GPS_period = GPS_period
        self.GPS_duration = GPS_duration
        self.k = k
        self.C = C
        self.duration = flight_duration
        self.dt = dt
        
    def _calculate_with_correction(self):
        """ Вычисление ошибки комплексирования """
        # Ошибка
        error = 0   # Ошибка = 0 в начале
        errors = [] # Память ошибок по долготе и широте (одинаковые)
        # Видимость спутника
        GPS = False         # Флаг видимости спутника
        GPS_counter = 0     # Количество единиц времени видимости спутника
        last_GPS_time = 0   # Время с момента последнего комплексирования
            
        # Цикл
        for t in range(self.duration):
            # Проверяем, виден ли спутник
            if t % self.GPS_period == 0 and t != 0:
                GPS = True
                GPS_counter = 0
                last_GPS_time = t
            
            # Поскольку данные синтетические, то рост ошибки определения координат положения БЛА по широте и долготе одинаковый и является квадратичным от времени полета
            # Если спутник виден, то вычисляем ошибку комплексирования
            if GPS:
                error -= error * (1 - exp(-self.k * GPS_counter))
                GPS_counter += self.dt
                # Проверяем, виден ли спутник в следующий момент времени
                if (GPS_counter >= self.GPS_duration):
                    GPS = False
            else:
                dt_local = t - last_GPS_time             # Локальное время после комплексирования
                error += self.C * dt_local * dt          # Квадратичный рост ошибки определения координат положения БЛА
            errors.append(error)
        return errors
    
    def _calculate_without_correction(self):
        """ Вычисление ошибки комплексирования """
        # Поскольку данные синтетические, то рост ошибки определения координат положения БЛА по широте и долготе одинаковый и является квадратичным от времени полета
        error = 0
        errors = []
        # Квадратичный рост ошибки для синтетических данных
        for t in range(self.duration):
            error += self.C * t * self.dt
            errors.append(error)
        return errors
    
    def simulate(self):
        """ Симуляция """
        # Вычисление ошибок с комплексированием и без комплексирования
        return self._calculate_with_correction(), self._calculate_without_correction()

def CalculateKParameter(choosen_k=None):
    """ Запомненная ошибка комплексирования вычисляется из формулы:
            q = (1 - exp(-k*tau)) * (y - x), где:
    - k   : основной параметр комплексирования;
    - tau : время комплексирования (GPS_duration);
    - y   : координата (долготы/широты) со спутника;
    - x   : координата (долготы/широты) по СВС (система воздушних сигналов).
    """
    # Необходимо, чтобы q >= 0.95 * (y-x) (по усл.) => 1 - exp(-k * tau) >= 0.95 => k >= -ln(0.05)/tau.
    # Возьмем k как безразмерную величину по отношению к tau, т.е. k = k'/tau. Тогда k' >= - ln(0.05).
    # Из теории автоматического управления известно, что exp(-5) < 0.01, т.е. k=5/tau покрывает как минимум 99% ошибки определения координат положения.
    # В случае k >= -ln(0.05) покрывается как минимум 95% ошибки ошибки определения координат положения.
    
    # Минимальное значение k для обеспечения покрытия не менее 95% ошибки определения координат положения БЛА 
    return choosen_k if choosen_k else -np.log(0.05)

############################
#         МЕНЕДЖЕР         #
############################
class SimulationManager:
    """ Управление симуляцией полета и вычисления ошибок определения координат положения (без комплексирования и с комплексированием (GPS))
        - V_air             : воздушная скорость БЛА;
        - W_wind            : скорость ветра;
        - K_angle           : курсовой угол БЛА;
        - wind_angle        : угол ветра;
        - A_angle           : угол атаки БЛА;
        - GPS_period        : период видимости спутника (GPS);
        - GPS_duration      : время видимости спутника (GPS);
        - k                 : основной параметр комплексирования;
        - C                 : коэффициент роста квадратичной зависимости ошибки определения положения координат от времени полета;
        - flight_duration   : временной промежуток моделирования;
        - dt                : временной шаг моделирования.
    """
    def __init__(self, V_air=23, V_wind=7, K_angle=45, wind_angle=120, A_angle=5,
                 GPS_period=20, GPS_duration=1, k=-np.log(0.05), C=0.001, 
                 flight_duration=60, dt=1):
        # Моделирование полета БЛА
        self.flight = FlightModelSimulator(V_air, V_wind, K_angle, wind_angle, A_angle, flight_duration)
        # Моделирование ошибок полета БЛА
        self.error_sim = GPSErrorSimulator(GPS_period, GPS_duration, k, C, flight_duration, dt)

    def run(self):
        """ Реализация моделирования полета БЛА и расчета ошибок """
        x, y = self.flight.simulate()
        err_corr, err_no_corr = self.error_sim.simulate()
        return x, y, err_corr, err_no_corr

    def show_plots(self):
        """ Отрисовка.
            - Широта: от экватора к полюсам (от -90 градусов [Южный полюс] до +90 градусов [Северный полюс]);
            - Долгота: от нулевого меридиана (Гринвич) на В или З (от -180 градусов [Восток] до +180 градусов [Запад]).
            Измерения смещения производятся в метрах для простоты (без учета радиуса и кривизны (для небольших участков) Земли).
        """
        # Чтение данных
        x, y, err_corr, err_no_corr = self.run()                  # Траектория по спутнику (синтетика) + Ошибка с комплексированием + Ошибка без комплексирования
        t = np.arange(0, self.flight.duration, self.error_sim.dt) # Временная шкала
        
        # Настройка отрисовки окна с графиками
        fig, axs = subplots(2, 2, figsize=(12, 6))
        fig.canvas.manager.set_window_title("Моделирование полета БЛА с постоянными исходными данными")
        fig.suptitle(
            f"Скорость БЛА={self.flight.V_air} м/c, Скорость ветра={self.flight.V_wind} м/с, "
            f"Курсовой угол={self.flight.K_angle}\u00B0, Направление ветра={self.flight.wind_angle}\u00B0, "
            f"Угол атаки={self.flight.A_angle}\u00B0",
            fontsize=12
        )
        
        # Расчет значений
        x_no_corr = [x[i] + err_no_corr[i] for i in range(len(x))]
        x_corr = [x[i] + err_corr[i] for i in range(len(x))]
        y_no_corr = [y[i] + err_no_corr[i] for i in range(len(y))]
        y_corr = [y[i] + err_corr[i] for i in range(len(y))]
        
        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + время)
        axs[0, 0].plot(t, x, label="Траектория полета БЛА", color='green')
        axs[0, 0].plot(t, x_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 0].plot(t, x_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 0].set_title("Сравнение всех тректорий полета БЛА (долгота)", fontsize=10)
        axs[0, 0].set_xlabel("t (время, мин)", fontsize=9)
        axs[0, 0].set_ylabel("$W_\lambda$ (долгота, м)", fontsize=9)
        axs[0, 0].legend(fontsize=8, loc='upper left')
        
        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (долгота + время)
        axs[0, 1].plot(t, y, label="Траектория полета БЛА", color='green')
        axs[0, 1].plot(t, y_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 1].plot(t, y_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 1].set_title("Сравнение всех тректорий полета БЛА (широта)", fontsize=10)
        axs[0, 1].set_xlabel("t (время, мин)", fontsize=9)
        axs[0, 1].set_ylabel("$W_\phi$ (широта, м)", fontsize=9)
        axs[0, 1].legend(fontsize=8, loc='upper left')

        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + долгота)
        axs[1, 0].plot(x, y, label="Траектория полета БЛА", color='green')
        axs[1, 0].plot(x_no_corr, y_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[1, 0].plot(x_corr, y_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[1, 0].set_title("Сравнение всех тректорий полета БЛА", fontsize=10)
        axs[1, 0].set_xlabel("$W_\lambda$ (долгота, м)", fontsize=9)
        axs[1, 0].set_ylabel("$W_\phi$ (широта, м)", fontsize=9)
        axs[1, 0].legend(fontsize=8, loc='upper left')

        # Ошибка без коплексирования и с комплексированием
        axs[1, 1].plot(t, err_corr, label="С комплексированием", color='blue')
        axs[1, 1].plot(t, err_no_corr, label="Без комплексирования", color='red')
        axs[1, 1].set_title("Сравнение ошибок с комплексированием и без", fontsize=10)
        axs[1, 1].set_xlabel("t (время, мин)", fontsize=9)
        axs[1, 1].set_ylabel("$\epsilon$ (ошибка, м)", fontsize=9)
        axs[1, 1].legend(fontsize=8, loc='upper left')
        
        fig.tight_layout(rect=[0.05, 0.05, 0.95, 0.95]) # left, bottom, right, top
        show()        

if __name__ == "__main__":
    """ ЗАПУСК ПРОГРАММЫ """
    #######################
    ### ИСХОДНЫЕ ДАННЫЕ ###
    #######################   
    # Константы
    flight_duration = 60        # Время моделирования (мин)
    V_air = 23                  # Воздушная скорость БЛА (м/с)
    V_wind = 7                  # Скорсоть ветра (м/c)
    K_angle = 45                # Курсовой угол БЛА (градусы)
    wind_angle = 120            # Угол ветра (градусы)
    A_angle = 5                 # Угол атаки (градусы)
    dt = 1                      # Шаг времени моделирования (мин)
    
    # GPS
    GPS_period = 20            # Период видимости спутника (мин)
    GPS_duration = 1           # Время видимости спутника (мин)
    
    # Коэффициент квадратичного роста ошибки определения координат положения
    C = 0.05

    # Основной параметр комплексирования
    k = CalculateKParameter()
    
    ################################
    ### МОДЕЛИРОВАНИЕ ПОЛЕТА БЛА ###
    ################################
    # Синтетические данные
    sim_synthetic = SimulationManager(V_air=V_air, V_wind=V_wind, K_angle=K_angle, wind_angle=wind_angle, A_angle=A_angle,
                                      GPS_period=GPS_period, GPS_duration=GPS_duration, k=k, C=C,
                                      flight_duration=flight_duration, dt=dt)
    sim_synthetic.show_plots()