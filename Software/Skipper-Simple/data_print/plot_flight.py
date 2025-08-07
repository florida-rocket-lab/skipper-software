import pandas as pd
import matplotlib.pyplot as plt

# ---------- load the CSV ----------
df = pd.read_csv("flight.csv")
df["t"] = df["t"] / 1000.0          # ms → s

# ---------- panel configuration ----------
plots = [
    ("Altitude",          ["alt"],                     "m"),
    ("Throttle PWM",      ["escPWM"],                  "µs"),     # <─ rename
    ("Gimbal Deg",        ["rollDeg", "pitchDeg"],     "deg"),    # <─ rename
    ("Accel (g)",         ["ax", "ay", "az"],          "g"),
    ("Gyro (rad/s)",      ["gx", "gy", "gz"],          "rad/s"),
]

# ---------- draw ----------
fig, axes = plt.subplots(len(plots), 1,
                         sharex=True, figsize=(11, 9))
fig.suptitle("Flight log: flight.csv")

for ax, (title, cols, units) in zip(axes, plots):
    for c in cols:
        ax.plot(df["t"], df[c], label=c)
    ax.set_ylabel(f"{title}\n({units})")
    ax.grid(True, linestyle=":")
    if len(cols) > 1:
        ax.legend(loc="upper right", fontsize="small")

axes[-1].set_xlabel("time (s)")
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
