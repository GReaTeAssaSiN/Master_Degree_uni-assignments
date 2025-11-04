from math import exp, cos, sin
from matplotlib.pyplot import show, subplots
import numpy as np

############################
# МОДЕЛИРОВАНИЕ ПОЛЕТА БЛА #
############################
class FlightModelSimulator:
    """ Моделирование полета БЛА.
        - V_air          : воздушная скорость БЛА (м/с);
        - V_wind         : скорость ветра (м/с);
        - K_angle        : курсовой угол БЛА (градусы);
        - wind_angle     : угол ветра (м/с);
        - A_angle        : угол атаки БЛА (градусы);
        - flight_duration: временной промежуток моделирования полета БЛА (продолжительность) (с);
        - dt             : временной шаг моделирования (с).
    """
    def __init__(self, V_air, V_wind, K_angle, wind_angle, A_angle, flight_duration, dt):
        # Исходные данные
        self.V_air = V_air
        self.V_wind = V_wind
        self.K_angle = K_angle
        self.wind_angle = wind_angle
        self.A_angle = A_angle
        self.flight_duration = flight_duration
        self.dt = dt
        # Вычисляемые данные
        self.x = [] # Координаты положения БЛА по долготе (м)
        self.y = [] # Координаты положения БЛА по широте (м)
        
    def simulate(self):
        """ Вычисляет (симулирует) траеторию БЛА """
        # Цикл по временным шагам
        for t in np.arange(0, self.flight_duration + self.dt, self.dt):
            V_air = self.V_air
            V_wind = self.V_wind
            K = np.deg2rad(self.K_angle)
            A = np.deg2rad(self.A_angle)
            d = np.deg2rad(self.wind_angle)
            
            # Вычисление долготной и широтной составляющей путевой скорости
            dx = (V_air * sin(K) + V_wind * sin(d)) * cos(A)
            dy = (V_air * cos(K) + V_wind * cos(d)) * cos(A)
            
            # Если это начальный момент времени, то начальная точка (0;0)
            if (t <= 1e-6):
                self.x.append(0)
                self.y.append(0)
            else: # Иначе синтетически моделируем методом Эйлера
                self.x.append(self.x[-1] + dx * self.dt)
                self.y.append(self.y[-1] + dy * self.dt)
        return self.x, self.y

##################################################################
#    МОДЕЛИРОВАНИЕ ПОГРЕШНОСТЕЙ СИСТЕМ (из 2 л/р)                #
# - моделирование спутника GPS (искусственное изменение ошибки)  #
# - моделирование погрешностей бортовой системы БЛА              #   
##################################################################
class ErrorsModelSimulator:
    """ Вычисление ошибки (без комплексирования и с комплексированием - GPS).
        - flight_duration: время моделирования полета БЛА (с);
        - GPS_period     : период видимости спутника/GPS (с);
        - GPS_duration   : время комплексирования/видимости спутника (с);
        - accuracy       : точность (доля запомненной ошибки);
        - C              : коэффициент роста ошибки определения координат положения БЛА по квадратичной зависимости от времени полета БЛА (дрейф).
        - dt             : временной шаг моделирования полета БЛА (с);
    """
    def __init__(self, flight_duration=3600, GPS_period=1200, GPS_duration=60, 
                 accuracy=0.95, C=0.001, dt=0.1):
        # Исходные данные
        self.flight_duration = flight_duration
        self.GPS_period = GPS_period
        self.GPS_duration = GPS_duration
        self.accuracy = accuracy
        self.C = C
        self.dt = dt
        # Вычисляемые данные
        self.k = self._calculate_k_parametr()               # Основной параметр комплексирования
   
    def _calculate_k_parametr(self):
        """ Запомненная ошибка комплексирования вычисляется из формулы:
                q = (1 - exp(-k*tau)) * (y - x), где:
        - k   : основной параметр комплексирования;
        - tau : время комплексирования (GPS_duration);
        - y   : координата (долготы/широты) по спутнику/GPS;
        - x   : координата (долготы/широты) по СВС (система воздушных сигналов).
        """
        # Необходимо, чтобы q >= 0.95 * (y-x) (по усл.) => 1 - exp(-k * tau) >= 0.95 => k >= -ln(1 - 0.95)/tau.
        # Из теории автоматического управления известно, что exp(-5) < 0.01, т.е. k=5/tau покрывает как минимум 99% ошибки определения координат положения.
        # В случае k >= -ln(0.05) покрывается как минимум 95% ошибки определения координат положения.
        # Тогда в качестве k необходимо вернуть: 5 / self.GPS_duration
        k = -np.log(1 - self.accuracy) / self.GPS_duration
        return k

    def _calculate_without_correction(self):
        """ Вычисление ошибки определения координат положения БЛА """
        # Память
        errors = list()
        # Цикл вычисления ошибки
        for t in np.arange(0, self.flight_duration + self.dt, self.dt):            
            # В начальный момент времени ошибка = 0
            if (t <= 1e-6):
                errors.append(0)
            # Посольку данные синтетические, то рост ошибки определения координат положения БЛА по долготе и широте одинаковый и определяется по квадратичной зависимости от времени полета (дрейф)
            else:
                errors.append(errors[-1] + self.C * t * self.dt)
        
        return errors
                
    def _calculate_with_correction(self):
        """ Вычисление ошибки комплексирования (GPS) """
        # Ошибка комплексирования
        error = 0
        # Память
        errors_GPS = list()
        # Повторяющийся интервал видимости спутника (self.GPS_period + self.GPS_duration)
        time_since_GPS = 0
        
        # Цикл вычисления ошибки комплексирования
        for t in np.arange(0, self.flight_duration + self.dt, self.dt):
            # Если повторяющийся интервал стал >= self.GPS_period, то спутник виден
            if time_since_GPS - self.dt >= self.GPS_period:
                error -= error * (1 - exp(-self.k * self.dt))
                # Если time_since_GPS == self.GPS_period + self.GPS_duration, то спутник больше не виден
                # eps = 1e-6 - для учета погрешности машинного вычисления
                if np.abs(time_since_GPS - (self.GPS_period + self.GPS_duration)) <= 1e-6:
                    time_since_GPS = 0
            # Поскольку данные синтетические, то рост ошибки определения координат положения БЛА по широте и долготе одинаковый и определяется по квадратичной зависимости от времени полета (дрейф)
            else:
                error += self.C * time_since_GPS * self.dt
            errors_GPS.append(error)
            time_since_GPS += self.dt
        return errors_GPS
    
    def simulate(self):
        """ Симуляция """
        # Вычисление ошибки бортовой системы и комплексной системы
        return self._calculate_without_correction(), self._calculate_with_correction()
     
############################
#         МЕНЕДЖЕР         #
############################
class SimulationManager:
    """ Управление симуляцией полета и вычисления ошибок определения координат положения (без комплексирования и с комплексированием (GPS))
        - V_air             : воздушная скорость БЛА (м/c);
        - W_wind            : скорость ветра (м/с);
        - K_angle           : курсовой угол БЛА (градусы);
        - wind_angle        : угол ветра (градусы);
        - A_angle           : угол атаки БЛА (градусы);
        - GPS_period        : период видимости спутника (GPS) (с);
        - GPS_duration      : время видимости спутника (GPS) (с);
        - accuracy          : точность (доля запомненной ошибки);
        - C                 : коэффициент роста квадратичной зависимости ошибки определения положения координат от времени полета (дрейф);
        - flight_duration   : временной промежуток моделирования (с);
        - dt                : временной шаг моделирования (с).
    """
    def __init__(self, V_air=150, V_wind=25, K_angle=80, wind_angle=100, A_angle=5,
                 GPS_period=1200, GPS_duration=60, accuracy=0.95, C=0.001, 
                 flight_duration=3600, dt=0.1):
        # Моделирование полета БЛА
        self.model_flight = FlightModelSimulator(V_air, V_wind, K_angle, wind_angle, A_angle, flight_duration, dt)
        # Моделирование ошибок полета БЛА
        self.model_errors = ErrorsModelSimulator(flight_duration, GPS_period, GPS_duration, accuracy, C, dt)

    def run(self):
        """ Реализация моделирования полета БЛА и расчета ошибок """
        x, y = self.model_flight.simulate()
        err_without_corr, err_with_corr = self.model_errors.simulate()
        return x, y, err_without_corr, err_with_corr

    def show_plots(self):
        """ Отрисовка.
            - Широта: от экватора к полюсам (от -90 градусов [Южный полюс] до +90 градусов [Северный полюс]);
            - Долгота: от нулевого меридиана (Гринвич) на В или З (от -180 градусов [Восток] до +180 градусов [Запад]).
            Измерения смещения производятся в метрах для простоты (без учета радиуса и кривизны (для небольших участков) Земли).
        """
        # Чтение данных
        x, y, err_without_corr, err_with_corr = self.run()
        t = np.arange(0, self.model_flight.flight_duration + self.model_flight.dt, self.model_flight.dt)

        # Вычисление велеичин для сравнительного анализа
        E_0_ind = int(self.model_errors.GPS_period/self.model_errors.dt)
        E_0 = err_without_corr[E_0_ind]
        E_q_ind = int((self.model_errors.GPS_period + self.model_errors.GPS_duration)/self.model_errors.dt)
        E_q = err_with_corr[E_q_ind]
        q = (1 - exp(-self.model_errors.k * self.model_errors.GPS_duration)) * E_0
        percent = (q/E_0) * 100

        # Настройка отрисовки окна с графиками
        fig, axs = subplots(2, 2, figsize=(12, 9))
        fig.canvas.manager.set_window_title("Имитационная модель курсовой системы БЛА с комплексированием и без комплексирования")
        fig.suptitle(
            f"Время полета = {self.model_flight.flight_duration} с, "
            f"Период видимости спутника = {self.model_errors.GPS_period} с, Время комплексирования = {self.model_errors.GPS_duration} c, "
            f"\nПараметр комплексирования k = {self.model_errors.k:.5f} с$^{{-1}}$, "
            f"\nСкорость БЛА = {self.model_flight.V_air} м/c, Скорость ветра = {self.model_flight.V_wind} м/с, Направление ветра = {self.model_flight.wind_angle}\u00B0, "
            f"\nКурсовой угол = {self.model_flight.K_angle}\u00B0, Угол атаки = {self.model_flight.A_angle}\u00B0, "
            f"\nОшибка на начало комплексирования $E_0 = {E_0:.3f}$ м, Запомненная ошибка q = {q:.3f} м, "
            f"\nОшибка после комплексирования $E_q = {E_q:.3f}$ м, Процент запомненной ошибки = {percent:.2f}%",
            fontsize=12
        )
        
        # Расчет значений
        x_no_corr = [x[i] + err_without_corr[i] for i in range(len(x))]
        y_no_corr = [y[i] + err_without_corr[i] for i in range(len(y))]
        x_corr = [x[i] + err_with_corr[i] for i in range(len(x))]
        y_corr = [y[i] + err_with_corr[i] for i in range(len(y))]
        
        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (долгота + время)
        axs[0, 0].plot(t, x, label="Траектория полета БЛА", color='green', linestyle='-')
        axs[0, 0].plot(t, x_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 0].plot(t, x_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 0].set_title("Сравнение всех тректорий полета БЛА (долгота)", fontsize=10)
        axs[0, 0].set_xlabel("t (время, мин)", fontsize=9)
        axs[0, 0].set_ylabel("$W_\lambda$ (долгота, м)", fontsize=9)
        axs[0, 0].legend(fontsize=8, loc='upper left')
        axs[0, 0].grid(True)
        
        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + время)
        axs[0, 1].plot(t, y, label="Траектория полета БЛА", color='green', linestyle='-')
        axs[0, 1].plot(t, y_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 1].plot(t, y_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 1].set_title("Сравнение всех тректорий полета БЛА (широта)", fontsize=10)
        axs[0, 1].set_xlabel("t (время, мин)", fontsize=9)
        axs[0, 1].set_ylabel("$W_\phi$ (широта, м)", fontsize=9)
        axs[0, 1].legend(fontsize=8, loc='upper left')
        axs[0, 1].grid(True)

        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + долгота)
        axs[1, 0].plot(x, y, label="Траектория полета БЛА", color='green', linestyle='-')
        axs[1, 0].plot(x_no_corr, y_no_corr, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[1, 0].plot(x_corr, y_corr, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[1, 0].set_title("Сравнение всех тректорий полета БЛА", fontsize=10)
        axs[1, 0].set_xlabel("$W_\lambda$ (долгота, м)", fontsize=9)
        axs[1, 0].set_ylabel("$W_\phi$ (широта, м)", fontsize=9)
        axs[1, 0].legend(fontsize=8, loc='upper left')
        axs[1, 0].grid(True)

        # Ошибка без коплексирования и с комплексированием
        axs[1, 1].plot(t, err_with_corr, label="Ошибка системы комплексирования", color='blue', linestyle='-')
        axs[1, 1].plot(t, err_without_corr, label="Ошибка бортовой системы БЛА", color='red', linestyle='-')
        axs[1, 1].plot(self.model_errors.GPS_period, E_0, 'co', markersize=4, label="$E_0$ - начало комплексирования (в первый раз)")
        axs[1, 1].plot(self.model_errors.GPS_period + self.model_errors.GPS_duration, E_q, 'co', markersize=4, label="$E_q$ - конец комплексирования (в первый раз)")
        axs[1, 1].set_title("Сравнение погрешностей бортовой и комплексной системы", fontsize=10)
        axs[1, 1].set_xlabel("t (время, мин)", fontsize=9)
        axs[1, 1].set_ylabel("$\epsilon$ (ошибка, м)", fontsize=9)
        axs[1, 1].legend(fontsize=8, loc='upper left')
        axs[1, 1].grid(True)
        
        fig.tight_layout(rect=[0.05, 0.05, 0.95, 0.95]) # left, bottom, right, top
        show()        

if __name__ == "__main__":
    """ ЗАПУСК ПРОГРАММЫ """
    plane = SimulationManager(V_air=150, V_wind=25, K_angle=80, wind_angle=100, A_angle=5,
                              GPS_period=20*60, GPS_duration=1*60, 
                              accuracy=0.95, C=0.001, 
                              flight_duration=60*60, dt=0.1)
    plane.show_plots()