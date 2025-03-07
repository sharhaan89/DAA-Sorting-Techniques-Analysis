# analyze_results.py
import pandas as pd
import matplotlib.pyplot as plt

# Load the results CSV
df = pd.read_csv("output.csv")

# Plot Execution Time vs. Input Size for each algorithm
plt.figure(figsize=(10, 6))
for algo in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algo]
    plt.plot(subset['InputSize'], subset['Time'], marker='o', label=algo)
plt.xlabel("Input Size (n)")
plt.ylabel("Execution Time (seconds)")
plt.title("Execution Time vs. Input Size for Sorting Algorithms")
plt.legend()
plt.grid(True)
plt.show()

# Plot Comparisons vs. Input Size (skip those with NA)
plt.figure(figsize=(10, 6))
for algo in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algo]
    # Only plot if comparisons are numeric (skip RadixSort, etc.)
    try:
        subset['Comparisons'] = pd.to_numeric(subset['Comparisons'])
        plt.plot(subset['InputSize'], subset['Comparisons'], marker='s', label=algo)
    except:
        continue
plt.xlabel("Input Size (n)")
plt.ylabel("Number of Comparisons")
plt.title("Comparisons vs. Input Size for Sorting Algorithms")
plt.legend()
plt.grid(True)
plt.show()
