s = ["I1", "I2", "I3", "I4", "I5"]

print("{")
for x in s:
    for y in s:
        print("{", f"{x}, {y}", "}")


print("}")