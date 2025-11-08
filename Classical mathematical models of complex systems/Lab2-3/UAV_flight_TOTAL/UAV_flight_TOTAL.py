from math import exp, cos, sin
from matplotlib.pyplot import show, subplots
import numpy as np
from pathlib import Path
import pandas as pd

############################
#           EXCEL          #
############################
class ExcelParser:
    """ Чтение исходных данных из Excel """
    def __init__(self, filepath: str):
        self.filepath = Path(filepath)
        if not self.filepath.exists():
            raise FileNotFoundError(f"[ERROR] Файл не найден: {self.filepath}")

    def _list_sheets(self):
        """Возвращает список всех листов Excel."""
        xls = pd.ExcelFile(self.filepath)
        return xls.sheet_names

    def _read_sheet(self, sheet_name=0):
        """ Возвращает обработанный DataFrame для чтения указанного листа Excel с заголовками """
        df = pd.read_excel(self.filepath, sheet_name=sheet_name, header=None)   # DataFrame для чтения указанного листа Excel с заголовками
        df = df[pd.to_numeric(df.iloc[:, 0], errors='coerce').notna()]          # Фильтр для оставления только числовых данных
        df = df.reset_index(drop=True)                                          # Реиндексация данных после удаления "неправильных"
        return df

    def _extract_variables(self, df):
        """
        Извлекает переменные из таблицы:
        t, H, Vist, Vpr, tkk, betta, teta, gamma, psi, X, Z, W, omega
        Возвращает словарь numpy-массивов.
        /-/ (!) - не используется напрямую /-/
        """
        data = {
            't': df.iloc[:, 0].to_numpy(),       #     время (c)
            'H': df.iloc[:, 1].to_numpy(),       # (!) высота (м)
            'Vist': df.iloc[:, 3].to_numpy(),    # (!) истинная скорость (м/c) - c учетом ветра
            'Vpr': df.iloc[:, 5].to_numpy(),     #     приборная скорость (м/c) - без учета ветра, собственная
            'tkk': df.iloc[:, 6].to_numpy(),     #     угол атаки (градусы)
            'betta': df.iloc[:, 7].to_numpy(),   # (!) угол скольжения (градусы)
            'teta': df.iloc[:, 8].to_numpy(),    # (!) тангаж (градусы)
            'gamma': df.iloc[:, 9].to_numpy(),   # (!) крен (градусы)
            'psi': df.iloc[:, 10].to_numpy(),    #     курс (градусы)
            'X': df.iloc[:, 11].to_numpy(),      #     X спутника (широта) (м)
            'Z': df.iloc[:, 12].to_numpy(),      #     Z спутника (долгота) (м)
            'W': df.iloc[:, 13].to_numpy(),      #     скорость ветра (м/c)
            'omega': df.iloc[:, 14].to_numpy(),  #     направление ветра (градусы)
        }
        return data
    
    def ExecuteDataFromExcel(self, sheet_name:str):
        """ Чтение экспериментальных данных из Excel """
        sheets = self._list_sheets()          # Список листов в Excel
        if sheet_name in sheets:              # Поиск нужного листа
           df = self._read_sheet(sheet_name)  # Объект для чтения данных с листа
           data = self._extract_variables(df) # Чтение данных
        else:
            raise ValueError(f"[ERROR] Лист {sheet_name} не найден в Excel!")
        return data

############################
# МОДЕЛИРОВАНИЕ ПОЛЕТА БЛА #
############################
class FlightModelSimulator:
    """ Моделирование полета БЛА.
        - V_air          : воздушная скорость БЛА (м/с);
        - V_wind         : скорость ветра (м/с);
        - K_angle        : курсовой угол БЛА (градусы);
        - W_direction    : направление ветра (градусы);
        - A_angle        : угол атаки БЛА (градусы);
        - flight_duration: временной промежуток моделирования полета БЛА (продолжительность) (с);
        - dt             : временной шаг моделирования (с).
    """
    def __init__(self, V_air, V_wind, K_angle, W_direction, A_angle, flight_duration, dt):
        # Исходные данные
        self.V_air = V_air
        self.V_wind = V_wind
        self.K_angle = K_angle
        self.W_direction = W_direction
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
            # В случае Excel-данных - массивы значений, в случае синтетических данных - константы
            V_air = self.V_air[int(t/self.dt)] if isinstance(self.V_air, (list, np.ndarray)) else self.V_air
            V_wind = self.V_wind[int(t/self.dt)] if isinstance(self.V_wind, (list, np.ndarray)) else self.V_wind
            K = np.deg2rad(self.K_angle[int(t/self.dt)] if isinstance(self.K_angle, (list, np.ndarray)) else self.K_angle)
            A = np.deg2rad(self.A_angle[int(t/self.dt)] if isinstance(self.A_angle, (list, np.ndarray)) else self.A_angle)
            d = np.deg2rad(self.W_direction[int(t/self.dt)] if isinstance(self.W_direction, (list, np.ndarray)) else self.W_direction)

            # Вычисление долготной и широтной составляющей путевой скорости
            dx = (V_air * sin(K) + V_wind * sin(d)) * cos(A)
            dy = (V_air * cos(K) + V_wind * cos(d)) * cos(A)
            
            # Если это начальный момент времени, то начальная точка (0;0)
            if (t <= 1e-6):
                self.x.append(0)
                self.y.append(0)
            else: # Иначе определяем координаты положения БЛА (долгота/широта) методом Эйлера
                self.x.append(self.x[-1] + dx * self.dt)
                self.y.append(self.y[-1] + dy * self.dt)
        return self.x, self.y

##################################################################
#    МОДЕЛИРОВАНИЕ ПОГРЕШНОСТЕЙ СИСТЕМ (СИМУЛЯЦИЯ, 2 л/р)        #
# - моделирование погрешности системы комплексирования           #
# - моделирование погрешности бортовой системы БЛА               #   
##################################################################
class ErrorsModelSimulatorBySynthetic:
    """ Моделирование погрешностей бортовой системы БЛА и системы комплексирования.
        - flight_duration: время моделирования полета БЛА (с);
        - GPS_period     : период видимости спутника/GPS (с);
        - GPS_duration   : время комплексирования/видимости спутника (с);
        - accuracy       : точность (доля запомненной ошибки);
        - C              : коэффициент роста ошибки определения координат положения БЛА по квадратичной зависимости от времени полета БЛА (дрейф).
        - dt             : временной шаг моделирования полета БЛА (с).
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
        self.k = self._calculate_k_parametr() # Основной параметр комплексирования
   
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
        # Ошибка определения координат положения БЛА
        error_X = 0
        error_Y = 0
        # Память
        errors_X = list()
        errors_Y = list()
        # Цикл вычисления ошибки
        for t in np.arange(0, self.flight_duration + self.dt, self.dt):            
            # В виду упрощения будем считать, что рост ошибки определения координат положения БЛА по долготе и широте одинаковый и определяется по квадратичной зависимости от времени полета (дрейф)
            error_X += self.C * t * self.dt
            error_Y += self.C * t * self.dt
            errors_X.append(error_X)
            errors_Y.append(error_Y)

        return errors_X, errors_Y
                
    def _calculate_with_correction(self):
        """ Вычисление ошибки комплексирования (GPS) """
        # Ошибка комплексирования
        error_X = 0
        error_Y = 0
        # Память
        errors_X = list()
        errors_Y = list()
        # Время, прошедшее с последней видимости спутника (для учета повторяющегося интервала видимости спутника (self.GPS_period + self.GPS_duration))
        time_since_GPS = 0
        
        # Цикл вычисления ошибки комплексирования
        for t in np.arange(0, self.flight_duration + self.dt, self.dt):
            # Если повторяющийся интервал стал >= self.GPS_period, то спутник виден
            # eps = 1e-6 - для учета погрешности машинного вычисления
            if time_since_GPS - self.GPS_period >= 1e-6:
                error_X -= error_X * (1 - exp(-self.k * self.dt))
                error_Y -= error_Y * (1 - exp(-self.k * self.dt))
                # Если time_since_GPS == self.GPS_period + self.GPS_duration, то спутник больше не виден
                if np.abs(time_since_GPS - (self.GPS_period + self.GPS_duration)) <= 1e-6:
                    time_since_GPS = 0
            # Спутник не виден
            else:
                # В виду упрощения будем считать, что рост ошибки определения координат положения БЛА по долготе и широте одинаковый и определяется по квадратичной зависимости от времени полета (дрейф)
                error_X += self.C * time_since_GPS * self.dt
                error_Y += self.C * time_since_GPS * self.dt
            errors_X.append(error_X)
            errors_Y.append(error_Y)
            time_since_GPS += self.dt
            
        return errors_X, errors_Y
    
    def simulate(self):
        """ Симуляция """
        # Вычисление ошибки бортовой системы БЛА и комплексной системы
        return self._calculate_without_correction(), self._calculate_with_correction()
    
##################################################################
#    МОДЕЛИРОВАНИЕ ПОГРЕШНОСТЕЙ СИСТЕМ (ЭКСПЕРИМЕНТАЛЬНО)        #
# - моделирование погрешности системы комплексирования           #
# - моделирование погрешности бортовой системы БЛА               #   
##################################################################
class ErrorsModelSimulatorByExperiment:
    """ Моделирование погрешностей бортовой системы БЛА и системы комплексирования.
        - flight_duration: время моделирования полета БЛА (с);
        - GPS_period     : период видимости спутника/GPS (с);
        - GPS_duration   : время комплексирования/видимости спутника (с);
        - accuracy       : точность (доля запомненной ошибки);
        Коэффициент роста квадратичной зависимости ошибки определения координат положения БЛА от времени полета (дрейф):
        -*- C1           : для долготы;
        -*- C2           : для широты;
        - dt             : временной шаг моделирования полета БЛА (с).
    """
    def __init__(self, flight_duration, GPS_period, GPS_duration, accuracy, C1, C2, dt):
        # Исходные данные
        self.flight_duration = flight_duration
        self.GPS_period = GPS_period
        self.GPS_duration = GPS_duration
        self.accuracy = accuracy
        self.C1 = C1
        self.C2 = C2
        self.dt = dt
        # Вычисляемые данные
        self.k = self._calculate_k_parametr() # Основной параметр комплексирования
   
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
        # Ошибка определения координат положения БЛА
        error_X = 0
        error_Y = 0
        # Память
        errors_X = list()
        errors_Y = list()
        # Цикл вычисления ошибки
        for t in np.arange(0, self.flight_duration + self.dt, self.dt):
            # Фактическая ошибка составляет: GPS - СВС (система воздушных сигналов).
            # В виду упрощения будем считать, что рост ошибки определения координат положения БЛА по долготе и широте одинаковый и определяется по квадратичной зависимости от времени полета (дрейф)
            error_X += self.C1 * t * self.dt
            error_Y += self.C2 * t * self.dt
            errors_X.append(error_X)
            errors_Y.append(error_Y)

        return errors_X, errors_Y
                
    def _calculate_with_correction(self,  x=None, y=None, x_sp=None, y_sp=None):
        """ Вычисление ошибки комплексирования (GPS) """
        # Ошибка комплексирования
        error_X = 0
        error_Y = 0
        # Запоминаемая ошибка комплексирования
        q_X = 0
        q_Y = 0
        # Память
        errors_X = list()
        errors_Y = list()
        errors_q_X = list()
        errors_q_Y = list()
        # Время, прошедшее с последней видимости спутника (для учета повторяющегося интервала видимости спутника (self.GPS_period + self.GPS_duration))
        time_since_GPS = 0
        # Флаг для рассчета фактической ошибки координат положения БЛА
        calc_by_GPS = True
        
        # Цикл вычисления ошибки комплексирования
        for t in np.arange(0, self.flight_duration + self.dt, self.dt):
            # Если повторяющийся интервал стал >= self.GPS_period, то спутник виден
            # eps = 1e-6 - для учета погрешности машинного 
            if time_since_GPS - self.GPS_period >= 1e-6:
                if calc_by_GPS:
                    error_X = x_sp[int(t/self.dt)] - x[int(t/self.dt)] - q_X
                    error_Y = y_sp[int(t/self.dt)] - y[int(t/self.dt)] - q_Y
                    calc_by_GPS = False
                q_X += error_X * (1 - exp(-self.k * self.dt))
                q_Y += error_Y * (1 - exp(-self.k * self.dt))
                error_X -= error_X * (1 - exp(-self.k * self.dt))
                error_Y -= error_Y * (1 - exp(-self.k * self.dt))
                # Если time_since_GPS == self.GPS_period + self.GPS_duration, то спутник больше не виден
                if np.abs(time_since_GPS - (self.GPS_period + self.GPS_duration)) <= 1e-6:
                    time_since_GPS = 0
                    calc_by_GPS = True
            # Спутник не виден
            else:
                # В виду упрощения будем считать, что рост ошибки определения координат положения БЛА по долготе и широте одинаковый и определяется по квадратичной зависимости от времени полета (дрейф)
                error_X += self.C1 * time_since_GPS * self.dt
                error_Y += self.C2 * time_since_GPS * self.dt
            errors_X.append(error_X)
            errors_Y.append(error_Y)
            errors_q_X.append(q_X)
            errors_q_Y.append(q_Y)
            time_since_GPS += self.dt
            
        return errors_X, errors_Y, errors_q_X, errors_q_Y
    
    def simulate(self, x=None, y=None, x_sp=None, y_sp=None):
        """ Симуляция """
        # Вычисление ошибки бортовой системы БЛА и комплексной системы
        return self._calculate_without_correction(), self._calculate_with_correction(x, y, x_sp, y_sp)
    
############################
#         МЕНЕДЖЕР         #
############################
class SimulationManager:
    """ Управление симуляцией полета и вычисления ошибок определения координат положения (без комплексирования и с комплексированием (GPS))
        - V_air             : воздушная скорость БЛА (м/c);
        - W_wind            : скорость ветра (м/с);
        - K_angle           : курсовой угол БЛА (градусы);
        - W_direction       : направление ветра (градусы);
        - A_angle           : угол атаки БЛА (градусы);
        - GPS_period        : период видимости спутника (GPS) (с);
        - GPS_duration      : время видимости спутника (GPS) (с);
        - accuracy          : точность (доля запомненной ошибки);
        Коэффициент роста квадратичной зависимости ошибки определения координат положения БЛА от времени полета (дрейф):
        -*- C1              : для синтетических данных или для долготы (экспериментальные данные);
        -*- C2              : для широты (экспериментальные данные);
        - flight_duration   : временной промежуток моделирования (с);
        - dt                : временной шаг моделирования (с);
        - x_sp              : координаты положения БЛА по спутнику (GPS) - долгота (м);
        - y_sp              : координаты положения БЛА по спутнику (GPS) - широта (м).
    """
    def __init__(self, V_air=150, V_wind=25, K_angle=80, W_direction=100, A_angle=5,
                 GPS_period=1200, GPS_duration=60, accuracy=0.95, C1=0.001, C2=0.001,
                 flight_duration=3600, dt=0.1,
                 x_sp=None, y_sp=None):
        # Моделирование полета БЛА
        self.model_flight = FlightModelSimulator(V_air, V_wind, K_angle, W_direction, A_angle, flight_duration, dt)
        # Моделирование ошибок полета БЛА
        if (x_sp is None and y_sp is None):
            # Синтетические данные
            self.model_errors = ErrorsModelSimulatorBySynthetic(flight_duration, GPS_period, GPS_duration, accuracy, C1, dt)
        else:
            # Экспериментальные данные
            self.model_errors = ErrorsModelSimulatorByExperiment(flight_duration, GPS_period, GPS_duration, accuracy, C1, C2, dt)
        
        # Спутник (GPS)
        self.x_sp = x_sp
        self.y_sp = y_sp

    def _compute_path(self, x, y):
        """ Вычисление пройденного пути БЛА """
        # Приведение к массивам NumPy
        x = np.array(x, dtype=float)
        y = np.array(y, dtype=float)
        # Вычисление
        dx = np.diff(x)              # приращения по долготе
        dy = np.diff(y)              # приращения по широте
        ds = np.sqrt(dx**2 + dy**2)  # пройденный путь в каждый момент времени
        path = np.cumsum(ds)         # накопленный путь в каждый момент времени
        return path

    def run(self):
        """ Реализация моделирования полета БЛА и расчета ошибок """
        x, y = self.model_flight.simulate()
        ERRORS = (
            self.model_errors.simulate()
            if self.x_sp is None and self.y_sp is None
            else self.model_errors.simulate(x, y, self.x_sp, self.y_sp)
        )
        without_GPS, with_GPS = ERRORS

        return x, y, without_GPS, with_GPS

    def show_plots(self, excel_path="not specified", sheet_name="not defined"):
        """ Визуализация.
            - Широта: от экватора к полюсам (от -90 градусов [Южный полюс] до +90 градусов [Северный полюс]);
            - Долгота: от нулевого меридиана (Гринвич) на В или З (от -180 градусов [Восток] до +180 градусов [Запад]).
            Измерения смещения производятся в метрах для простоты (без учета радиуса и кривизны (для небольших участков) Земли).
        """
        # Чтение данных
        x, y, without_GPS, with_GPS = self.run()
        err_without_GPS_X, err_without_GPS_Y = without_GPS
        if self.x_sp is None and self.y_sp is None:     # Синтетические данные
            err_with_GPS_X, err_with_GPS_Y = with_GPS
        else:                                           # Экспериментальные данные
            err_with_GPS_X, err_with_GPS_Y, q_X, q_Y = with_GPS
        # Временной массив, соответствующий временным шагам моделирования
        t = np.arange(0, self.model_flight.flight_duration + self.model_flight.dt, self.model_flight.dt)

        # Настройка отрисовки окна с графиками
        fig, axs = subplots(2, 2, figsize=(14, 7))
        fig.canvas.manager.set_window_title("Имитационная модель курсовой системы БЛА с комплексированием и без комплексирования")
        if self.x_sp is None and self.y_sp is None:
            fig.suptitle(
                f"Время полета = {self.model_flight.flight_duration} с, "
                f"Период видимости спутника = {self.model_errors.GPS_period} с, Время комплексирования = {self.model_errors.GPS_duration} c, "
                f"\nПараметр комплексирования k = {self.model_errors.k:.5f} с$^{{-1}}$, Процент запомненной ошибки = {(1 - exp(-self.model_errors.k * self.model_errors.GPS_duration)) * 100}%, "
                f"\nСкорость БЛА = {self.model_flight.V_air} м/c, Скорость ветра = {self.model_flight.V_wind} м/с, Направление ветра = {self.model_flight.W_direction}\u00B0, "
                f"\nКурсовой угол = {self.model_flight.K_angle}\u00B0, Угол атаки = {self.model_flight.A_angle}\u00B0",
                fontsize=12
            )
        else:
            fig.suptitle(
                f"Исходные данные взяты из Excel: {excel_path} [Лист: {sheet_name}] "
                f"\n\nВремя полета = {self.model_flight.flight_duration} с, "
                f"Период видимости спутника = {self.model_errors.GPS_period} с, Время комплексирования = {self.model_errors.GPS_duration} c, "
                f"\nПараметр комплексирования k = {self.model_errors.k:.5f} с$^{{-1}}$, Процент запомненной ошибки = {(1 - exp(-self.model_errors.k * self.model_errors.GPS_duration)) * 100}%",
                fontsize=12
            )
        
        # Определение координат положения БЛА: бортовая система БЛА, система комлексирования и спутник (GPS)
        # Синтетические данные (x и y - это спутник/GPS)
        if self.x_sp is None and self.y_sp is None:    
            x_without_GPS = [x[i] + err_without_GPS_X[i] for i in range(len(x))]
            y_without_GPS = [y[i] + err_without_GPS_Y[i] for i in range(len(y))]
            x_with_GPS = [x[i] + err_with_GPS_X[i] for i in range(len(x))]
            y_with_GPS = [y[i] + err_with_GPS_Y[i] for i in range(len(y))]
            x_GPS = x
            y_GPS = y
        # Экспериментальные данные
        else:
            x_without_GPS = x
            y_without_GPS = y
            x_with_GPS = [x[i] + q_X[i] for i in range(len(x))]
            y_with_GPS = [y[i] + q_Y[i] for i in range(len(y))]
            x_GPS = self.x_sp
            y_GPS = self.y_sp
            
        # Вывод
        print("Среднеквадратические ошибки определения координат положения БЛА:")
        MSE_X_without_GPS = np.sqrt(np.mean((np.array(x_GPS) - np.array(x_without_GPS)) ** 2))
        MSE_Y_without_GPS = np.sqrt(np.mean((np.array(y_GPS) - np.array(y_without_GPS)) ** 2))
        MSE_X_with_GPS = np.sqrt(np.mean((np.array(x_GPS) - np.array(x_with_GPS)) ** 2))
        MSE_Y_with_GPS = np.sqrt(np.mean((np.array(y_GPS) - np.array(y_with_GPS)) ** 2))
        print(f"- Долгота [без комплексирования]: MSE = {MSE_X_without_GPS:5f} м")
        print(f"- Широта  [без комплексирования]: MSE = {MSE_Y_without_GPS:5f} м")
        print(f"- Долгота [с комплексированием]: MSE = {MSE_X_with_GPS:5f} м")
        print(f"- Широта  [с комплексированием]: MSE = {MSE_Y_with_GPS:5f} м")
        print(f"- Снижение MSE (долгота) : MSE = {100 - MSE_X_with_GPS/MSE_X_without_GPS * 100:5f}%")
        print(f"- Снижение MSE  (широта) : MSE = {100 - MSE_Y_with_GPS/MSE_Y_without_GPS * 100:5f}%")
        print()
        
        print("Среднеквадратические ошибки вычисления пройденного пути БЛА:")
        path_without_GPS = self._compute_path(x_without_GPS, y_without_GPS)
        path_with_GPS = self._compute_path(x_with_GPS, y_with_GPS)
        path_GPS = self._compute_path(x_GPS, y_GPS)
        MSE_path_without_GPS = np.sqrt(np.mean((np.array(path_GPS) - np.array(path_without_GPS))**2))
        MSE_path_with_GPS = np.sqrt(np.mean((np.array(path_GPS) - np.array(path_with_GPS))**2))
        print(f"- [без комплексирования] MSE = {MSE_path_without_GPS:5f} м")
        print(f"- [c комплексированием]  MSE = {MSE_path_with_GPS:5f} м")
        print(f"- Снижение MSE: {100 - MSE_path_with_GPS/MSE_path_without_GPS * 100:5f}%")
        print("\n")
        
        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (долгота + время)
        axs[0, 0].plot(t, x_GPS, label="Траектория полета БЛА по спутнику/GPS", color='green', linestyle='-')
        axs[0, 0].plot(t, x_without_GPS, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 0].plot(t, x_with_GPS, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 0].set_title("Сравнение всех тректорий полета БЛА (долгота)", fontsize=10)
        axs[0, 0].set_xlabel("t (время, мин)", fontsize=9)
        axs[0, 0].set_ylabel("$\lambda$ (долгота, м)", fontsize=9)
        axs[0, 0].legend(fontsize=8)
        axs[0, 0].grid(True)
        
        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + время)
        axs[0, 1].plot(t, y_GPS, label="Траектория полета БЛА по спутнику/GPS", color='green', linestyle='-')
        axs[0, 1].plot(t, y_without_GPS, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[0, 1].plot(t, y_with_GPS, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[0, 1].set_title("Сравнение всех тректорий полета БЛА (широта)", fontsize=10)
        axs[0, 1].set_xlabel("t (время, мин)", fontsize=9)
        axs[0, 1].set_ylabel("$\phi$ (широта, м)", fontsize=9)
        axs[0, 1].legend(fontsize=8)
        axs[0, 1].grid(True)

        # Траектория полета БЛА + Траектории полета БЛА без комплексирования и с ним (широта + долгота)
        axs[1, 0].plot(x_GPS, y_GPS, label="Траектория полета БЛА по спутнику/GPS", color='green', linestyle='-')
        axs[1, 0].plot(x_without_GPS, y_without_GPS, label="Траектория полета БЛА без комплексирования", color='red', linestyle='--')
        axs[1, 0].plot(x_with_GPS, y_with_GPS, label="Траектория полета БЛА с комплексированием", color='blue', linestyle='--')
        axs[1, 0].set_title("Сравнение всех тректорий полета БЛА", fontsize=10)
        axs[1, 0].set_xlabel("$\lambda$ (долгота, м)", fontsize=9)
        axs[1, 0].set_ylabel("$\phi$ (широта, м)", fontsize=9)
        axs[1, 0].legend(fontsize=8)
        axs[1, 0].grid(True)

        # Ошибка без коплексирования и с комплексированием
        axs[1, 1].plot(t, err_without_GPS_X, label="Ошибка бортовой системы БЛА (долгота)", color='red', linestyle='-.')
        axs[1, 1].plot(t, err_with_GPS_X, label="Ошибка системы комплексирования (долгота)", color='blue', linestyle='-')
        axs[1, 1].plot(t, err_without_GPS_Y, label="Ошибка бортовой системы БЛА (широта)", color='green', linestyle=':')
        axs[1, 1].plot(t, err_with_GPS_Y, label="Ошибка системы комплексирования (широта)", color='orange', linestyle='--')
        axs[1, 1].set_title("Сравнение погрешностей бортовой и комплексной системы", fontsize=10)
        axs[1, 1].set_xlabel("t (время, мин)", fontsize=9)
        axs[1, 1].set_ylabel("$\epsilon$ (ошибка, м)", fontsize=9)
        axs[1, 1].legend(fontsize=8)
        axs[1, 1].grid(True)
        
        fig.tight_layout(rect=[0, 0, 1, 1]) # left, bottom, right, top
        show()        

if __name__ == "__main__":
    """ ЗАПУСК ПРОГРАММЫ """
    # ИСХОДНЫЕ ДАННЫЕ
    # 1. Синтетические данные
    # --- Инициалиция и моделирование ---
    plane_synthetic = SimulationManager(V_air=150, V_wind=25, K_angle=80, W_direction=100, A_angle=5,
                                        GPS_period=20*60, GPS_duration=1*60, 
                                        accuracy=0.95, C1=0.001, 
                                        flight_duration=60*60, dt=0.1)
    # 2. Эксперементальные данные
    # --- Excel ----
    filepath = "../ЛР2-3 Исходные данные.xls" # Путь до Excel-файла
    sheet_name = "Лист2"                      # Название листа в Excel-файле
    # --- Извлечение данных ---
    data = ExcelParser(filepath=filepath).ExecuteDataFromExcel(sheet_name=sheet_name)
    # --- Инициализация и моделирование ---
    plane_real = SimulationManager(V_air=data['Vpr'], V_wind=data['W'], K_angle=data['psi'], W_direction=data['omega'], A_angle=data['tkk'],
                                   GPS_period=210, GPS_duration=70,
                                   accuracy=0.95, C1=0.067, C2=0.087,
                                   flight_duration=data['t'][-1], dt=data['t'][1]-data['t'][0],
                                   x_sp=data['Z'], y_sp=data['X'])

    # ВИЗУАЛИЗАЦИЯ
    plane_synthetic.show_plots()
    plane_real.show_plots(excel_path=filepath, sheet_name=sheet_name)