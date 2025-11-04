from math import exp
from matplotlib.pyplot import show, subplots
import numpy as np

#######################################
#  Моделирование погрешностей систем  #
#######################################
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
        self.errors = self._calculate_without_correction()  # Ошибка бортовой системы БЛА
        self.errors_GPS = self._calculate_with_correction() # Ошибка комплексной системы БЛА
   
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
        
    def show_plot(self):
        """ Отрисовка.
            - Широта: от экватора к полюсам (от -90 градусов [Южный полюс] до +90 градусов [Северный полюс]);
            - Долгота: от нулевого меридиана (Гринвич) на В или З (от -180 градусов [Восток] до +180 градусов [Запад]).
            Измерения смещения производятся в метрах для простоты (без учета радиуса и кривизны (для небольших участков) Земли).
        """
        # Вычисление велеичин для сравнительного анализа
        E_0_ind = int(self.GPS_period/self.dt)
        E_0 = self.errors[E_0_ind]
        E_q_ind = int((self.GPS_period + self.GPS_duration)/self.dt)
        E_q = self.errors_GPS[E_q_ind]
        q = (1 - exp(-self.k * self.GPS_duration)) * E_0
        percent = (q/E_0) * 100
        
        # Настройка отрисовки окна с графиками
        fig, axs = subplots(1, 1, figsize=(12, 6))
        fig.canvas.manager.set_window_title("Моделирование погрешности бортовой системы БЛА и комплексной системы")
        fig.suptitle(
            f"Время моделирования полета: {self.flight_duration} с, "
            f"Период видимости спутника: {self.GPS_period} с, "
            f"Время комплексирования: {self.GPS_duration} с, "
            f"\nПараметр комплексирования: k={self.k:.5f} с$^{{-1}}$, "
            f"\nОшибка на начало комплексирования: $E_0 = {E_0:.3f}$ м, "
            f"Запомненная ошибка: $q_0 = {q:.3f}$ м, "
            f"\nОшибка после комплексирования: $E_q = {E_q:.3f}$ м, "
            f"Процент запомненной ошибки: {percent:.2f}%",
            fontsize=12
        )
  
        # Массив временных меток
        t = np.arange(0, self.flight_duration + self.dt, self.dt)
        
        # Ошибка без коплексирования и с комплексированием
        axs.plot(t, self.errors_GPS, label="Ошибка системы комплексирования (долгота/широта)", color='blue', linestyle='--')
        axs.plot(t, self.errors, label="Ошибка бортовой системы БЛА (долгота/широта)", color='red', linestyle='-')
        axs.plot(self.GPS_period, E_0, 'co', markersize=4, label="$E_0$ - начало комплексирования (в первый раз)")
        axs.plot(self.GPS_period + self.GPS_duration, E_q, 'co', markersize=4, label="$E_q$ - конец комплексирования (в первый раз)")
        axs.set_title("Сравнение погрешностей бортовой и комплексной системы", fontsize=10)
        axs.set_xlabel("t (время, с)", fontsize=9)
        axs.set_ylabel("$\epsilon$ (ошибка, м)", fontsize=9)
        axs.legend(fontsize=8)
        axs.grid(True)
        
        fig.tight_layout()
        show()   
        
if __name__ == "__main__":
    """ ЗАПУСК ПРОГРАММЫ """
    errors_model_sim = ErrorsModelSimulator(flight_duration=60*60, GPS_period=20*60, GPS_duration=1*60, 
                                            accuracy=0.95, C=0.001, dt=0.1)
    errors_model_sim.show_plot()