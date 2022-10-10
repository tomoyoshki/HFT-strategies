'''
Various risk measurements on StrategyStudio output files
'''

def cumulative_return(security):
    '''
    Measures the Cumulative return of the given security
    Parameters
    ----------
    array : security
        array representing the security to perform analysis on
    '''
    return (security[-1] - security[0]) / security[0]

def sharpe_ratio(security, risk=0.1):
    '''
    Measures the Sharpe ratio of the given profit and loss
    ----------
    array : security
        array representing the security to perform analysis on
    float : risk
        risk rate for the given security
    '''
    return (cumulative_return(security) - risk) / security.std()

def maximum_drowndown(security):
    '''
    Measures the Maximum drowndown of the given security
    ----------
    array : security
        array representing the security to perform analysis on
    '''
    return (security.min() - security.max()) / security.max()
