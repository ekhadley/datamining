# Ethan Hadley

# 1
## i
| A | B | ~AvB |
|---|---|---|
| 0 | 0 | 1 |
| 0 | 1 | 1 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |

Perceptron output: $w_A \cdot A + w_B \cdot B + b \ge 0$
For $W_A = -1$ and, $W_B = 1$, and $b = 0$, the model outputs -1 for $A=1, B=0$, resulting in false. All other combinations result in true.

## ii
| A | B | AvB |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 1 |

Perceptron output: $w_A \cdot A + w_B \cdot B + b \ge 0$
For $W_A = 1$ and, $W_B = 1$, and $b = 0$, the model outputs 0 for $A=0, B=0$, and 1 or 2 otherwise.



