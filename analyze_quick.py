import pandas as pd
import matplotlib.pyplot as plt

# Load the QuickSort results CSV (adjust the filename as needed)
df = pd.read_csv("quick_sort_comparison.csv")

# Ensure InputSize is numeric
df['InputSize'] = pd.to_numeric(df['InputSize'], errors='coerce')

# Print unique algorithms found (should be QuickSortFirst, QuickSortRandom, QuickSortMedian)
print("QuickSort Variants:", df['Algorithm'].unique())

# --- Plot 1: Execution Time vs. Input Size ---
plt.figure(figsize=(10, 6))
for algo in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algo]
    plt.plot(subset['InputSize'], subset['Time'], marker='o', label=algo)
plt.xlabel("Input Size (n)")
plt.ylabel("Execution Time (seconds)")
plt.title("QuickSort Variants: Execution Time vs. Input Size")
plt.legend()
plt.grid(True)
plt.show()

# --- Plot 2: Number of Comparisons vs. Input Size ---
plt.figure(figsize=(10, 6))
for algo in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algo].copy()  # Create a copy to avoid modifying the original slice
    subset['Comparisons'] = pd.to_numeric(subset['Comparisons'], errors='coerce')
    subset = subset.dropna(subset=['Comparisons'])
    plt.plot(subset['InputSize'], subset['Comparisons'], marker='s', label=algo)
plt.xlabel("Input Size (n)")
plt.ylabel("Number of Comparisons")
plt.title("QuickSort Variants: Comparisons vs. Input Size")
plt.legend()
plt.grid(True)
plt.show()

# --- Plot 3: Time per Comparison vs. Input Size ---
plt.figure(figsize=(10, 6))
for algo in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algo].copy()  # Create a copy to avoid SettingWithCopyWarning
    subset['Comparisons'] = pd.to_numeric(subset['Comparisons'], errors='coerce')
    subset = subset.dropna(subset=['Comparisons'])
    # Calculate time per comparison
    subset['TimePerComparison'] = subset['Time'] / subset['Comparisons']
    plt.plot(subset['InputSize'], subset['TimePerComparison'], marker='^', label=algo)
plt.xlabel("Input Size (n)")
plt.ylabel("Time per Comparison (seconds)")
plt.title("QuickSort Variants: Time per Comparison vs. Input Size")
plt.legend()
plt.grid(True)
plt.show()
