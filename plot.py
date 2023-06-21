import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

directory = './Plots/Adder/WithRounding/Determinant_Fixed'

def multiple_plots():
    dfs = []
    file_names = []
    for csv_file in os.listdir(directory):
        if (csv_file.endswith('.csv')):
            dfs.append(pd.read_csv(f'{directory}/{csv_file}'))
            name = csv_file.removesuffix('.csv')
            file_names.append(name)
    
    hist_vals = np.zeros((1000, len(dfs)))
    total_error = np.zeros((len(dfs[0].Length.unique()), len(dfs)))
    calc_error = np.zeros((len(dfs[0].Length.unique()), len(dfs)))


    i = 0
    for df in dfs:
        j = 0
        for input_length in df.Length.unique():
            slice = df.loc[df.Length == input_length]
            total_error[j,i] = slice.TotalError.mean()
            calc_error[j,i] = slice.CalcError.mean()
            j += 1
        plt.subplot(1, 2, 1)
        plt.plot(df.Length.unique(), total_error[:,i], label=file_names[i])
        plt.subplot(1, 2, 2)
        plt.plot(df.Length.unique(), calc_error[:,i], label=file_names[i])
        i += 1
    plt.subplot(1, 2, 1)
    plt.legend(prop={'size': 10})
    plt.title('Total Error for Each Operator')
    plt.xlabel('Length')
    plt.ylabel('Error')
    plt.subplot(1, 2, 2)
    plt.legend(prop={'size': 10})
    plt.title('Calc Error for Each Operator')
    plt.xlabel('Length')
    plt.ylabel('Error')
    plt.savefig(f'{directory}/error.png')
    plt.clf()

    for input_length in dfs[0].Length.unique():
        i = 0
        for df in dfs:
            slice = df.loc[df.Length == input_length]
            hist_vals[:,i] = slice.Delay
            i  += 1
        
        plt.hist(x=hist_vals, density=True, histtype='bar', label=file_names)
        plt.legend(prop={'size': 10})
        plt.title(f'Delay Distribution for {input_length}')
        plt.xlabel('Delay')
        plt.ylabel('Amount')
        plt.savefig(f'{directory}/{input_length}.png')
        plt.clf()

def epsilon_plots():
    df = None
    file_name = None
    for csv_file in os.listdir(directory):
        if (csv_file.endswith('.csv')):
            df = pd.read_csv(f'{directory}/{csv_file}')
            name = csv_file.removesuffix('.csv')
            file_name = name

    delay = np.zeros((len(df.Length.unique()), len(df.Epsilon.unique())))
    total_error = np.zeros((len(df.Length.unique()), len(df.Epsilon.unique())))
    calc_error = np.zeros((len(df.Length.unique()), len(df.Epsilon.unique())))

    i = 0
    for e in df.Epsilon.unique():
        j = 0
        for input_length in df.Length.unique():
            slice = df[(df['Length'] == input_length) & (df['Epsilon'] == e)]
            delay[j,i] = slice.Delay.mean()
            total_error[j,i] = slice.TotalError.mean()
            calc_error[j,i] = slice.CalcError.mean()
            j += 1
        plt.figure(1)
        # plt.subplot(1, 2, 1)
        # plt.plot(df.Length.unique(), total_error[:,i], label=e)
        # plt.subplot(1, 2, 2)
        plt.plot(df.Length.unique(), calc_error[:,i], label=e)
        plt.figure(2)
        plt.plot(df.Length.unique(), delay[:,i], label=e)
        i += 1
    plt.figure(1)
    # plt.subplot(1, 2, 1)
    # plt.legend(prop={'size': 10})
    # plt.title(f'Total Error for {file_name}')
    # plt.xlabel('Length')
    # plt.ylabel('Error')
    # plt.subplot(1, 2, 2)
    plt.legend(prop={'size': 10})
    plt.title(f'Calc Error for {file_name}')
    plt.xlabel('Length')
    plt.ylabel('Error')
    plt.savefig(f'{directory}/error.png')
    plt.clf()
    plt.figure(2)
    plt.legend(prop={'size': 10})
    plt.title(f'Mean Output Delay for {file_name}')
    plt.xlabel('Length')
    plt.ylabel('Output Delay')
    plt.savefig(f'{directory}/delay.png')
    plt.clf()

def error_plot():
    df = None
    file_name = None
    for csv_file in os.listdir(directory):
        if (csv_file.endswith('.csv')):
            df = pd.read_csv(f'{directory}/{csv_file}')
            name = csv_file.removesuffix('.csv')
            file_name = name

    total_error = np.zeros((len(df.Length.unique()), len(df.Epsilon.unique())))
    calc_error = np.zeros((len(df.Length.unique()), len(df.Epsilon.unique())))

    i = 0
    for e in df.Epsilon.unique():
        j = 0
        for input_length in df.Length.unique():
            slice = df[(df['Length'] == input_length) & (df['Epsilon'] == e)]
            total_error[j,i] = slice.TotalError.mean() * 100
            calc_error[j,i] = slice.CalcError.mean() * 100
            j += 1
        plt.subplot(1, 2, 1)
        plt.plot(df.Length.unique(), total_error[:,i], label=e)
        plt.subplot(1, 2, 2)
        plt.plot(df.Length.unique(), calc_error[:,i], label=e)
        i += 1
    plt.subplot(1, 2, 1)
    plt.legend(prop={'size': 10})
    plt.title(f'Total Error for Mul2fix')
    plt.xlabel('Length')
    plt.ylabel('Error (%)')
    plt.subplot(1, 2, 2)
    plt.legend(prop={'size': 10})
    plt.title(f'Calc Error for Mul2Fix')
    plt.xlabel('Length')
    plt.ylabel('Error (%)')
    plt.savefig(f'{directory}/error.png')
    plt.clf()


if __name__ == '__main__':
    
    #multiple_plots()
    epsilon_plots()