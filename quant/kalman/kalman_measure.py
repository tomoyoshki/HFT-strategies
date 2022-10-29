'''
Functions that measure the performance of the Kalman Filter
'''
from sklearn.metrics import mean_absolute_error,r2_score,mean_squared_error
import numpy as np

import pandas as pd
import matplotlib.pyplot as plt

from scipy import poly1d
from datetime import datetime

from pykalman import KalmanFilter

plt.style.use('seaborn-darkgrid')
plt.rcParams['figure.figsize'] = (10,7)

DATA_DIR = "../../data/"
SAMPLE_TEXT = "tick_SPY_20220401.txt"

def get_order_book(file_dir):
    '''
    Return the orderbook by reading the file directory provided
    Parameters
    ----------
    string : file_dir
        file directory containing the text tick data information
    '''

    # read the file
    file = pd.read_csv(
        file_dir,
        dtype="str",
        usecols = np.arange(8),
        header=None,
        names=["Date1", "Date2", "id", "Type", "Source", "Mix", "Value", "Quantity"]
    ).fillna(0)

    # specifies the type
    file = file.astype({
        "id": np.int64 or 0,
        "Mix": np.float64 or 0,
        "Value": np.float64 or 0,
        "Quantity": np.int64
    })

    # Distinguish trade book and price book
    file["Price"] = file["Value"]
    file.loc[file["Type"]=="T", "Quantity"]= file[file["Type"]=="T"]["Price"]
    file.loc[file["Type"]=="T", "Price"]= file[file["Type"]=="T"]["Mix"]


    file = file.drop(columns=["Date2", "Mix", "Value"])

    trade_book = file.loc[file["Type"] == "T"]
    price_book = file.loc[file["Type"] == "P"]
    price_book["Date1"] = price_book["Date1"].apply(lambda s : s.split(" ")[1].split(".")[0])

    return trade_book, price_book

def kalman_analyze(book):
    kf = KalmanFilter(
        transition_matrices = [1],
        observation_matrices = [1],
        initial_state_mean = 0,
        initial_state_covariance = 1,
        observation_covariance=1,
        transition_covariance=.0001
    )
    mean, cov = kf.filter(book["Price"])
    mean, std = mean.squeeze(), np.std(cov.squeeze())
    err = (book["Price"] - mean).to_numpy()
    # remove the unconverged part
    err[err > 1] = 1
    err[err < -5] = -5


    plt.figure(figsize=(15,7))
    plt.plot(err, 'm', lw=1)
    plt.plot(np.sqrt(cov.squeeze()), 'y', lw=1)
    plt.plot(-np.sqrt(cov.squeeze()), 'c', lw=1)
    plt.title('Kalman filter estimate')
    plt.legend(['Error: real_value - mean', 'std', '-std'])
    plt.xlabel('Ticks')
    plt.ylabel('Value') 

def statistical_measures(y_pred, y_actual):
    '''
    Measures the performance of the kalman filter
    Parameters
    ----------
    array : predicted
        array representing the value from Kalman Filter
    array: actual
        array representing the actual real-time value
    '''
    r2= r2_score(y_actual, y_pred)
    mae = mean_absolute_error(y_actual,y_pred)
    rmse = np.sqrt(mean_squared_error(y_actual, y_pred))
    return r2, mae, rmse

def relative_error(y_pred, y_actual):
    '''
    Measures the relative error of the kalman filter
    Parameters
    ----------
    array : y_pred
        array representing the value from Kalman Filter
    array: y_actual
        array representing the y_actual real-time value
    '''
    val_size = min(len(y_pred), len(y_actual))
    rel_error = np.divide(y_pred[:val_size] - y_actual[:val_size], y_actual[:val_size])
    rel_error_mean = np.sum(rel_error) / val_size
    return rel_error_mean
