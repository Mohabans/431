import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('timings.csv')

plt.figure(figsize=(10, 6))
plt.plot(df['NumElements'], df['TimSortSeconds'], label='Tim Sort')
# plt.plot(df['NumElements'], df['InsertionSortSeconds'], label='Insertion Sort')

plt.xscale('log', base=2)
plt.xlabel('Number of Elements (log scale)')
plt.ylabel('Time (seconds)')
plt.title('Sorting Times vs Number of Elements')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.tight_layout()
plt.show()