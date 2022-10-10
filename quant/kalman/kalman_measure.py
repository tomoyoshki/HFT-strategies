'''
Functions that measure the performance of the Kalman Filter
'''

def relative_error(predicted, actual):
    '''
    Measures the performance of the kalman filter
    Parameters
    ----------
    array : predicted
        array representing the value from Kalman Filter
    array: actual
        array representing the actual real-time value
    '''
    relative_error_sum = 0
    val_size = min(len(predicted), len(actual))
    for i in range(val_size):
        relative_error_sum += predicted[i] - actual[i] / actual[i]
    return relative_error_sum / val_size

if __name__ == "__main__":
    print("Hello World")
