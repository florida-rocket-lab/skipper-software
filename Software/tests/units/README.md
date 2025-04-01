Here are the following tests that are run in the order that they appear:

# Blink Tests (Test Communication Protocol)
*A.* Tests (1-6): Passing the default `Vector3` packet up and down the chain
---
1. Empty `Vector3` created by `Software/terminal/skipper/utility/serialize` correctly sends from **Terminal** (computer) to **Ground Transceiver** (uno).
2. ..., then from **Ground Transceiver** (uno) to **Skipper Transceiver** (nano).
3. ..., then from **Skipper Transceiver** (nano) to **Flight Computer** (teensy).
4. ..., then from **Flight Computer** (teensy) to **Skipper Transceiver** (nano).
5. ..., then from **Skipper Transceiver** (nano) to **Ground Transceiver** (uno).
6. ..., then from **Ground Transceiver** (uno) to **Terminal** (computer).

*B.* Tests (7-12): Passing a series of 10 `Vector3` packets up and down the chain
---
7. Ten `Vector3`'s created by `Software/terminal/skipper/utility/serialize` correctly sends from **Terminal** (computer) to **Ground Transceiver** (uno)...
8. ..., then from **Ground Transceiver** (uno) to **Skipper Transceiver** (nano).
9. ..., then from **Skipper Transceiver** (nano) to **Flight Computer** (teensy).
10. ..., then from **Flight Computer** (teensy) to **Skipper Transceiver** (nano).
11. ..., then from **Skipper Transceiver** (nano) to **Ground Transceiver** (uno).
12. ..., then from **Ground Transceiver** (uno) to **Terminal** (computer).

*C.* Tests (13-18): Passing a series of 10 `IMUData` packets up and down the chain
---

*D.* Tests (19-24): Passing a series of 10 `StateSpace` packets up and down the chain
---

*E.* Tests (25-30): Passing a series of 10 `Control` packets up and down the chain
---

*F.* Tests (31-36): Passing a series of 10 `TelemetryPacket` packets up and down the chain
---

*G.* Tests (37-42): Passing a series of 10 `CommandPacket` packets up and down the chain
---

*H.* Test (43-48): Passing an empty `CommandPacket` packet up and down the chain
---
