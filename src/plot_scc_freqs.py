import matplotlib.pyplot as plt
import pandas as pd


data = pd.read_csv("scc_sizes.csv", sep=',', dtype=pd.StringDtype())

fig, ax = plt.subplots()
data['Size'].value_counts().plot(ax=ax, kind='bar')
plt.savefig("scc_freqs.svg")