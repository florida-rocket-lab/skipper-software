# ground_station_gui.py — Skipper GS live GUI with multi-panels
# CSV columns expected (exact header):
# t_ms,alt_ft,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,esc_us,servo_x_us,servo_y_us,phase,thrust_milli

import sys, time, csv, threading, os
from collections import deque

from PyQt5 import QtCore, QtWidgets
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QLabel, QPushButton, QComboBox,
    QGridLayout, QFileDialog, QCheckBox
)

import serial, serial.tools.list_ports

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

CSV_HEADER = [
    "t_ms","alt_ft","ax_g","ay_g","az_g","gx_dps","gy_dps","gz_dps",
    "esc_us","servo_x_us","servo_y_us","phase","thrust_milli"
]
BAUD = 115200
PLOT_LEN = 2000  # samples kept per axis

# -------------------- Serial worker --------------------
class SerialWorker(QtCore.QObject):
    data_parsed = QtCore.pyqtSignal(dict)
    status_msg  = QtCore.pyqtSignal(str)
    connected   = QtCore.pyqtSignal(bool)

    def __init__(self):
        super().__init__()
        self._ser = None
        self._stop = threading.Event()
        self._thread = None
        self._log_csv = None
        self._writer = None

    def open(self, port, baud=BAUD, log_path=None):
        self.close()
        try:
            self._ser = serial.Serial(port, baudrate=baud, timeout=0.2)
            self._ser.reset_input_buffer(); self._ser.reset_output_buffer()
            self._stop.clear()
            if log_path:
                self._log_csv = open(log_path, "w", newline="")
                self._writer = csv.DictWriter(self._log_csv, fieldnames=CSV_HEADER)
                self._writer.writeheader()
            self._thread = threading.Thread(target=self._rx_loop, daemon=True)
            self._thread.start()
            self.connected.emit(True)
            self.status_msg.emit(f"Connected: {port} @ {baud}")
        except Exception as e:
            self.close()
            self.status_msg.emit(f"Open failed: {e}")
            self.connected.emit(False)

    def close(self):
        self._stop.set()
        if self._thread and self._thread.is_alive():
            self._thread.join(timeout=1.0)
        self._thread = None
        if self._ser:
            try: self._ser.close()
            except: pass
            self._ser = None
        if self._log_csv:
            try: self._log_csv.flush(); self._log_csv.close()
            except: pass
            self._log_csv = None; self._writer = None
        self.connected.emit(False)

    @QtCore.pyqtSlot(bytes)
    def write(self, payload: bytes):
        try:
            if self._ser and self._ser.is_open:
                self._ser.write(payload); self._ser.flush()
        except Exception as e:
            self.status_msg.emit(f"Write failed: {e}")

    def _rx_loop(self):
        t0 = None
        while not self._stop.is_set() and self._ser and self._ser.is_open:
            try:
                line_b = self._ser.readline()
                if not line_b: continue
                line = line_b.decode("utf-8", errors="replace").strip()
                if not line or "," not in line:  # skip banners
                    continue
                # header line? skip
                if line.startswith("t_ms,"): continue
                row = line.split(",")
                if len(row) < len(CSV_HEADER): continue

                d = {}
                try:
                    d["t_ms"] = int(float(row[0]))
                    d["alt_ft"] = float(row[1])
                    d["ax_g"] = float(row[2]); d["ay_g"] = float(row[3]); d["az_g"] = float(row[4])
                    d["gx_dps"] = float(row[5]); d["gy_dps"] = float(row[6]); d["gz_dps"] = float(row[7])
                    d["esc_us"] = int(float(row[8]))
                    d["servo_x_us"] = int(float(row[9])); d["servo_y_us"] = int(float(row[10]))
                    d["phase"] = int(float(row[11])); d["thrust_milli"] = int(float(row[12]))
                except Exception:
                    continue

                if self._writer:
                    try:
                        self._writer.writerow(d)
                        if (d["t_ms"] % 1000) < 50: self._log_csv.flush()
                    except Exception:
                        pass

                self.data_parsed.emit(d)
            except Exception as e:
                self.status_msg.emit(f"RX error: {e}")
                time.sleep(0.1)

# -------------------- Plot widgets --------------------
class PanelBase(FigureCanvas):
    def __init__(self, title, x_label="t (s)", y_label=""):
        fig = Figure(figsize=(6,2.1), dpi=100)
        super().__init__(fig)
        self.ax = fig.add_subplot(111)
        self.ax.set_title(title, fontsize=10)
        self.ax.set_xlabel(x_label, fontsize=9)
        self.ax.set_ylabel(y_label, fontsize=9)
        self.ax.grid(True, alpha=0.3)
        self._t0 = None
        self.x = deque(maxlen=PLOT_LEN)

    def _time_s(self, t_ms):
        if self._t0 is None: self._t0 = t_ms
        return max(0.0, (t_ms - self._t0)/1000.0)

    def _autoscale_x(self):
        if len(self.x) < 5: return
        x2 = self.x[-1]
        self.ax.set_xlim(max(0, x2-30), x2+0.5)

    def _autoscale_y(self, ys, pad_frac=0.08, min_span=1.0):
        ymin = min(min(y) for y in ys if len(y) > 0)
        ymax = max(max(y) for y in ys if len(y) > 0)
        if ymin == ymax: ymin -= min_span/2; ymax += min_span/2
        span = max(min_span, ymax - ymin)
        pad = max(0.1, pad_frac*span)
        self.ax.set_ylim(ymin - pad, ymax + pad)

class Panel1(PanelBase):
    # single-series (altitude)
    def __init__(self): 
        super().__init__("Altitude", y_label="ft")
        (self.line,) = self.ax.plot([], [], lw=1.6)
        self.y = deque(maxlen=PLOT_LEN)
    def push(self, d):
        self.x.append(self._time_s(d["t_ms"]))
        self.y.append(d["alt_ft"])
        self.line.set_data(self.x, self.y)
        self._autoscale_x(); self._autoscale_y([self.y], min_span=0.5)
        self.draw_idle()

class Panel3(PanelBase):
    # three-series (ax/ay/az or gx/gy/gz)
    def __init__(self, title, y_label):
        super().__init__(title, y_label=y_label)
        self.lines = self.ax.plot([], [], [], [], [], [], lw=1.0)
        self.y1 = deque(maxlen=PLOT_LEN)
        self.y2 = deque(maxlen=PLOT_LEN)
        self.y3 = deque(maxlen=PLOT_LEN)
        self.ax.legend(["x","y","z"], fontsize=8, loc="upper right")
    def push(self, t_s, v1, v2, v3):
        self.x.append(t_s); self.y1.append(v1); self.y2.append(v2); self.y3.append(v3)
        self.lines[0].set_data(self.x, self.y1)
        self.lines[1].set_data(self.x, self.y2)
        self.lines[2].set_data(self.x, self.y3)
        self._autoscale_x(); self._autoscale_y([self.y1,self.y2,self.y3])
        self.draw_idle()

class PanelPWM(PanelBase):
    # esc + two servos
    def __init__(self):
        super().__init__("PWM (ESC/Servos)", y_label="µs")
        self.lines = self.ax.plot([], [], [], [], [], [], lw=1.0)
        self.esc = deque(maxlen=PLOT_LEN)
        self.sx  = deque(maxlen=PLOT_LEN)
        self.sy  = deque(maxlen=PLOT_LEN)
        self.ax.legend(["ESC","servo_x","servo_y"], fontsize=8, loc="upper right")
        self.ax.set_ylim(900, 2100)
    def push(self, t_s, esc, sx, sy):
        self.x.append(t_s); self.esc.append(esc); self.sx.append(sx); self.sy.append(sy)
        self.lines[0].set_data(self.x, self.esc)
        self.lines[1].set_data(self.x, self.sx)
        self.lines[2].set_data(self.x, self.sy)
        self._autoscale_x(); self._autoscale_y([self.esc,self.sx,self.sy], min_span=200)
        self.draw_idle()

class PanelThrustPhase(PanelBase):
    # thrust (left axis) + phase (step-ish)
    def __init__(self):
        super().__init__("Thrust & Phase", y_label="thrust (milli)")
        (self.th_line,) = self.ax.plot([], [], lw=1.2)
        self.ax2 = self.ax.twinx()
        (self.ph_line,) = self.ax2.plot([], [], lw=1.0)
        self.ax2.set_ylabel("phase", fontsize=9)
        self.ax2.set_ylim(-0.5, 7.5)
        self.th = deque(maxlen=PLOT_LEN)
        self.ph = deque(maxlen=PLOT_LEN)
    def push(self, t_s, thrust_milli, phase):
        self.x.append(t_s); self.th.append(thrust_milli); self.ph.append(phase)
        self.th_line.set_data(self.x, self.th)
        self.ph_line.set_data(self.x, self.ph)
        self._autoscale_x(); self._autoscale_y([self.th], min_span=50)
        self.draw_idle()

# -------------------- Main window --------------------
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Skipper Ground Station")
        self.worker = SerialWorker()

        w = QWidget(); self.setCentralWidget(w)
        g = QGridLayout(w)

        # top bar
        self.portBox = QComboBox(); self.refreshPorts()
        self.refreshBtn = QPushButton("↻")
        self.connectBtn = QPushButton("Connect")
        self.disconnectBtn = QPushButton("Disconnect")
        self.logCheck = QCheckBox("Log to CSV")
        self.logPathLabel = QLabel("")
        g.addWidget(QLabel("Port:"), 0,0)
        g.addWidget(self.portBox, 0,1)
        g.addWidget(self.refreshBtn, 0,2)
        g.addWidget(self.connectBtn, 0,3)
        g.addWidget(self.disconnectBtn, 0,4)
        g.addWidget(self.logCheck, 0,5)
        g.addWidget(self.logPathLabel, 0,6,1,3)

        # readouts
        self.altLabel = QLabel("-- ft"); self.big(self.altLabel)
        self.phaseLabel = QLabel("phase: -")
        self.escLabel = QLabel("ESC: - us")
        self.gxLabel = QLabel("ωx: -"); self.gyLabel = QLabel("ωy: -"); self.gzLabel = QLabel("ωz: -")
        g.addWidget(self.altLabel, 1,0,1,2)
        g.addWidget(self.phaseLabel,1,2); g.addWidget(self.escLabel,1,3)
        g.addWidget(self.gxLabel,1,4); g.addWidget(self.gyLabel,1,5); g.addWidget(self.gzLabel,1,6)

        # plots
        self.p_alt   = Panel1()
        self.p_accel = Panel3("Accel (g)", "g")
        self.p_gyro  = Panel3("Gyro (°/s)", "°/s")
        self.p_pwm   = PanelPWM()
        self.p_tp    = PanelThrustPhase()

        g.addWidget(self.p_alt,   2,0,1,6)   # span across all columns
        g.addWidget(self.p_accel, 3,0,1,6)
        g.addWidget(self.p_gyro,  4,0,1,6)
        g.addWidget(self.p_pwm,   5,0,1,3)   # left half
        g.addWidget(self.p_tp,    5,3,1,3) 

        # buttons + status
        self.armBtn = QPushButton("ARM (A)")
        self.landBtn = QPushButton("LAND (L)")
        self.armBtn.setStyleSheet("font-size:16px; padding:8px;")
        self.landBtn.setStyleSheet("font-size:16px; padding:8px;")
        self.status = QLabel("idle")
        g.addWidget(self.armBtn, 7,0,1,2)
        g.addWidget(self.landBtn,7,2,1,2)
        g.addWidget(self.status, 7,4,1,5)

        # signals
        self.refreshBtn.clicked.connect(self.refreshPorts)
        self.connectBtn.clicked.connect(self.onConnect)
        self.disconnectBtn.clicked.connect(self.onDisconnect)
        self.armBtn.clicked.connect(lambda: self.sendCmd('a'))
        self.landBtn.clicked.connect(lambda: self.sendCmd('l'))
        self.logCheck.stateChanged.connect(self.pickLog)

        self.worker.data_parsed.connect(self.onData)
        self.worker.status_msg.connect(self.setStatus)
        self.worker.connected.connect(self.setConnected)

        self._log_path = None
        self.setConnected(False)

    def big(self, lbl): lbl.setStyleSheet("font-size:22px; font-weight:600;")
    def setStatus(self, s): self.status.setText(s)

    def setConnected(self, ok: bool):
        self.connectBtn.setEnabled(not ok)
        self.disconnectBtn.setEnabled(ok)
        self.armBtn.setEnabled(ok)
        self.landBtn.setEnabled(ok)
        self.portBox.setEnabled(not ok)
        self.refreshBtn.setEnabled(not ok)
        self.logCheck.setEnabled(not ok)
    def refreshPorts(self):
        self.portBox.clear()
        for p in serial.tools.list_ports.comports():
            self.portBox.addItem(p.device)

    def pickLog(self, state):
        if state == Qt.Checked:
            path, _ = QFileDialog.getSaveFileName(self, "Save CSV log", "flight_log.csv", "CSV (*.csv)")
            if path:
                self._log_path = path
                self.logPathLabel.setText(os.path.basename(path))
            else:
                self.logCheck.setChecked(False)
        else:
            self._log_path = None
            self.logPathLabel.setText("")

    def onConnect(self):
        port = self.portBox.currentText()
        self.worker.open(port, BAUD, log_path=self._log_path)

    def onDisconnect(self):
        self.worker.close()

    def sendCmd(self, c):
        self.worker.write(bytes([ord(c)]))
        self.setStatus(f"sent '{c.upper()}'")

    @QtCore.pyqtSlot(dict)
    def onData(self, d):
        t_s = (d["t_ms"])/1000.0 if True else 0.0  # used by multi-panels
        self.altLabel.setText(f"{d['alt_ft']:.2f} ft")
        self.phaseLabel.setText(f"phase: {d['phase']}")
        self.escLabel.setText(f"ESC: {d['esc_us']} us")
        self.gxLabel.setText(f"ωx: {d['gx_dps']:.1f}")
        self.gyLabel.setText(f"ωy: {d['gy_dps']:.1f}")
        self.gzLabel.setText(f"ωz: {d['gz_dps']:.1f}")

        self.p_alt.push(d)
        self.p_accel.push(t_s, d["ax_g"], d["ay_g"], d["az_g"])
        self.p_gyro.push(t_s, d["gx_dps"], d["gy_dps"], d["gz_dps"])
        self.p_pwm.push(t_s, d["esc_us"], d["servo_x_us"], d["servo_y_us"])
        self.p_tp.push(t_s, d["thrust_milli"], d["phase"])

    def closeEvent(self, e):
        self.worker.close()
        super().closeEvent(e)

def main():
    app = QApplication(sys.argv)
    win = MainWindow()
    win.resize(1200, 900)
    win.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
