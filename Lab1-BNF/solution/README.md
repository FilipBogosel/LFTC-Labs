# AutoDrive — Mini-Language Documentation

**Course:** Formal Languages and Compiler Design (LFTC)  
**Lab 1** — BNF Specification & Use Case Documentation

---

## Domain

AutoDrive is a small, C-style language designed for vehicle control systems. It simulates what an onboard car computer does: read sensor inputs, make threshold decisions, and command actuators like brakes or throttle.

I chose this domain because every required statement maps naturally to automotive control logic: reading sensors, checking safety thresholds, updating vehicle state, and running control loops.

---

## Types

| Type | What it represents | Examples |
|---|---|---|
| `int` | Vehicle speed (km/h), RPM, gear, counters | `0`, `120`, `-1` |
| `real` | Distance (m), acceleration, percentages | `3.14`, `0.5`, `-0.75` |
| `T[N]` | Fixed-size array of type `T` and length `N` | `real[8]`, `int[4]` |

---

## Statements & Why They're Needed

### Assignment (`=`)
Used to update vehicle state variables like target speed or braking force.
```c
target_speed = current_speed + 5;
```

### Input / Output (`read`, `write`)
`read` receives telemetry from physical sensors; `write` transmits commands to vehicle actuators or displays values.
```c
read(front_distance);
write(brake_pressure);
```

### Conditional (`if (...) ... else ...`)
Used for critical safety decisions — for example, checking if an obstacle is within the danger zone and immediately activating emergency brakes.
```c
if (front_distance < safe_distance) {
    brake = 100;
} else {
    brake = 0;
}
```

### Loop (`while (...) ...`)
Used for cyclic control loops — for example, gradually adjusting speed until the cruise control setpoint is reached.
```c
while (current_speed < target_speed) {
    current_speed = current_speed + 2;
    write(current_speed);
}
```

---

## Example Programs

### 1. Emergency Braking
Reads current speed and radar distance, calculates dynamic stopping distance, and activates emergency braking if the vehicle is too close to an obstacle.

```c
program EmergencyBraking {
    int current_speed;
    real radar_distance, safe_distance;
    int brake_active;

    read(current_speed);
    read(radar_distance);
    safe_distance = current_speed * 0.5 + 2.0;

    if (radar_distance <= safe_distance) {
        brake_active = 1;
        write(brake_active);
    } else {
        brake_active = 0;
        write(brake_active);
    }
}
```

### 2. Cruise Control
Gradually increases speed in increments of 2 km/h until reaching the target setpoint.

```c
program CruiseControl {
    int current_speed, target_speed;

    read(current_speed);
    read(target_speed);

    while (current_speed < target_speed) {
        current_speed = current_speed + 2;
        write(current_speed);
    }
}
```

### 3. LiDAR Sweep (Array Processing)
Reads 8 distance values from a multi-zone LiDAR sensor into an array, then iterates through the array to find the closest detected obstacle.

```c
program LidarSweep {
    real[8] sensors;
    int i, n;
    real min_dist, reading;

    n = 8;
    i = 0;

    while (i < n) {
        read(reading);
        sensors[i] = reading;
        i = i + 1;
    }

    min_dist = sensors[0];
    i = 1;

    while (i < n) {
        if (sensors[i] < min_dist) {
            min_dist = sensors[i];
        }
        i = i + 1;
    }

    write(min_dist);
}
```
