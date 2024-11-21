vazao = []
justica = []
efic = []
over = []
general = []

for i in range(30):
    a = input()
    input()
    input()
    input()
    input()
    input()
    input()
    vazao.append(float(input().split()[-1]))
    justica.append(float(input().split()[-1]))
    efic.append(float(input().split()[-1]) * 10)
    over.append(float(input().split()[-1]) * 10)
    e = input()
    general.append(float(input().split()[-1]))
    e = input()

import matplotlib.pyplot as plt

# Plot each line
plt.plot(vazao, label="Vazão", marker='o')
plt.plot(justica, label="Justiça", marker='s')
plt.plot(efic, label="Eficiência (x10)", marker='^')
plt.plot(over, label="Overhead (x10)", marker='x')
plt.plot(general, label=" Desempenho geral", marker='d')

# Add labels, title, and legend
plt.title("Métricas do Experimento sem Otimização")
plt.xlabel("Rodada")
plt.ylabel("Valor")
plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
# plt.legend()  # Add a legend to differentiate lines

# Show grid for better readability
plt.grid(True)

# Display the graph
plt.savefig("line_graph_2.png", format="png", dpi=300, bbox_inches="tight")

