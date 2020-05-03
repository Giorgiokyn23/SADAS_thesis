# SADAS_GDB

Feasibility study for SADAS' query outputs

## Folder structure
* `data/` contains the datasets (not all of them where
pushed, due to size issues)

* `src/` folder containing the source code.
    Inside here, there is the folder `C++/` 
    containing all the code for the query analysis.
    Three more files can be found: `data_preparation.sh`,
    bash script for cleaning the dataset and `data_check.ipynb`,
    a Jupyter notebook used to inspect some properties
    of the data.
    The Python script `plot_scc_freqs.py` is used to export an
    histogram of the frequencies of the sizes of strongly connected
    components