# Timere și întreruperi pentru microcontrolerele — 8051 / PIC18F8722

---

# 🇷🇴 Română

## Despre proiect

Acest proiect este o colecție de implementări practice în **Embedded C** pentru microcontrolerele **8051** și **PIC18F8722**, construită în jurul utilizării timerelor hardware și a întreruperilor.

Implementările urmăresc trecerea de la o soluție simplă bazată pe **polling** la soluții bazate pe **întreruperi**, utilizarea simultană a mai multor timere și, în cazul PIC18F8722, gestionarea mai multor surse de întrerupere folosind **niveluri diferite de prioritate**.

Fiecare implementare este însoțită de un proiect **Proteus**, astfel încât comportamentul microcontrolerului și al perifericelor să poată fi observat prin simulare.

## Structura proiectului

```text
Timere-si-intreruperi-pentru-microcontrolerele — 8051-si-PIC18F8722/
│
├── README.md
│
├── 8051/
│   ├── 01-timer-polling/
│   │   ├── proteus/
│   │   │   └── project.pdsprj
│   │   └── src/
│   │       └── main.c
│   │
│   ├── 02-timer-interrupt/
│   │   ├── proteus/
│   │   │   └── project.pdsprj
│   │   └── src/
│   │       └── main.c
│   │
│   └── 03-dual-timer-interrupt/
│       ├── proteus/
│       │   └── project.pdsprj
│       └── src/
│           └── main.c
│
└── pic18f8722/
    ├── 01-timer0-interrupt/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    └── 02-multiple-timers-priority/
        ├── proteus/
        │   └── project.pdsprj
        └── src/
            └── main.c
```

---

## 8051

### 1. Timer0 utilizând polling

**Locație:** `8051/01-timer-polling/`

Prima implementare folosește **Timer0** al microcontrolerului 8051 în **Mode 1**, ca timer pe 16 biți.

Timerul este reîncărcat cu:

```c
TH0 = 0xFC;
TL0 = 0x18;
```

Programul pornește timerul și verifică în mod activ flag-ul `TF0` până când apare overflow-ul:

```c
while(!TF0);
```

După overflow, timerul este oprit, iar flag-ul este resetat.

LED-ul conectat la **P1.0** este comutat după aproximativ 1000 de overflow-uri ale Timer0.

Această implementare demonstrează funcționarea unui timer prin **polling**, în care procesorul rămâne ocupat cu verificarea continuă a flag-ului timerului.

---

### 2. Timer0 utilizând întrerupere

**Locație:** `8051/02-timer-interrupt/`

A doua implementare folosește **întreruperea Timer0**, eliminând așteptarea activă utilizată în exemplul anterior.

Rutina de tratare a întreruperii este:

```c
void timer0_isr() interrupt 1
```

La fiecare overflow:

1. Timer0 este reîncărcat;
2. contorul este incrementat;
3. după 1000 de overflow-uri, LED-ul de pe **P1.0** este comutat;
4. contorul este resetat.

Întreruperea Timer0 și întreruperile globale sunt activate prin:

```c
ET0 = 1;
EA = 1;
```

Bucla principală rămâne liberă:

```c
while(1)
{}
```

Diferența principală față de prima implementare este că procesorul nu mai trebuie să aștepte în mod activ overflow-ul timerului. Evenimentul este gestionat automat prin **Interrupt Service Routine (ISR)**.

---

### 3. Două timere cu întreruperi

**Locație:** `8051/03-dual-timer-interrupt/`

A treia implementare utilizează simultan **Timer0** și **Timer1**.

Cele două timere controlează independent două LED-uri:

- **Timer0** → LED pe `P1.0`;
- **Timer1** → LED pe `P1.1`.

Fiecare timer are propria rutină de întrerupere:

```c
void timer0_isr() interrupt 1
void timer1_isr() interrupt 3
```

Ambele timere sunt pornite, iar întreruperile sunt configurate prin registrul `IE`:

```c
IE = 0x8A;
```

Fiecare timer are propriul contor software și comută LED-ul asociat după 1000 de overflow-uri.

Această implementare demonstrează utilizarea **simultană a mai multor timere hardware** și gestionarea mai multor surse de întrerupere pe 8051.

---

# PIC18F8722

## 4. Timer0 cu întrerupere

**Locație:** `pic18f8722/01-timer0-interrupt/`

Prima implementare pentru PIC18F8722 utilizează **Timer0** configurat ca timer pe 8 biți, cu clock intern și prescaler `1:16`.

Configurarea este realizată direct prin registrele microcontrolerului:

```c
T08BIT = 1;
T0CS = 0;
PSA = 0;
T0PS2 = 1;
T0PS1 = 0;
T0PS0 = 0;
```

Întreruperea Timer0 este activată prin:

```c
TMR0IE = 1;
TMR0IF = 0;
```

La fiecare overflow este executată rutina:

```c
void interrupt t0()
```

În ISR este utilizat un contor software. După atingerea valorii configurate, un model digital este transmis către `LATD`:

```c
LATD = ch[i];
```

Modelul utilizat este definit prin:

```c
char ch[]={0b01001010,0b1111010};
```

Bucla principală nu execută operații suplimentare:

```c
while (1){}
```

Astfel, această implementare demonstrează utilizarea unui timer hardware pentru generarea de evenimente periodice gestionate prin întreruperi.

---

## 5. Mai multe timere și priorități de întrerupere

**Locație:** `pic18f8722/02-multiple-timers-priority/`

Ultima implementare combină mai multe mecanisme de întrerupere ale PIC18F8722 și este cea mai complexă implementare din proiect.

Sunt utilizate simultan:

- **Timer0**;
- **Timer1**;
- **Timer2**;
- întreruperea externă **INT0**;
- întreruperea externă **INT1**;
- întreruperea externă **INT2**.

### Priorități de întrerupere

PIC18F8722 este configurat pentru utilizarea întreruperilor cu priorități:

```c
IPEN = 1;
GIEH = 1;
GIEL = 1;
```

Sursele de întrerupere sunt împărțite în două niveluri.

**Prioritate HIGH:**

- `INT0`;
- Timer0;
- Timer2.

**Prioritate LOW:**

- `INT1`;
- Timer1;
- `INT2`.

Rutina pentru întreruperile HIGH este:

```c
void interrupt high_priority ex1()
```

iar rutina pentru întreruperile LOW este:

```c
void interrupt low_priority ex2()
```

Fiecare sursă verifică propriul flag și apelează funcția corespunzătoare.

### Timer0

Timer0 este configurat ca timer pe 8 biți, cu clock intern și prescaler `1:32`:

```c
T08BIT = 1;
T0CS = 0;
PSA = 0;
T0PS2 = 1;
T0PS1 = 1;
T0PS0 = 0;
```

Timer0 are prioritate HIGH:

```c
TMR0IP = 1;
```

La fiecare overflow este incrementat `counter`, iar după 8 evenimente este comutat pinul `RD2`.

### Timer1

Timer1 utilizează clock intern și prescaler `1:4`.

Este configurat cu prioritate LOW:

```c
TMR1IP = 0;
```

La fiecare overflow este procesat `counter1`, iar pinul `RD3` este comutat.

### Timer2

Timer2 este configurat cu prescaler `1:4` și valoarea:

```c
PR2 = 255;
```

Timer2 are prioritate HIGH:

```c
TMR2IP = 1;
```

La fiecare eveniment Timer2 este incrementat `counter2`, iar după 256 de evenimente este comutat pinul `RD4`.

### Întreruperi externe

Pe lângă timere, implementarea folosește trei întreruperi externe:

- `INT0` → `ext0()` → controlează `RD0`;
- `INT1` → `ext1()` → controlează `RD1`;
- `INT2` → `ext5()` → controlează `RD5`.

Frontul de declanșare pentru cele trei întreruperi este configurat prin:

```c
INTEDG0 = 1;
INTEDG1 = 1;
INTEDG2 = 1;
```

Aceste întreruperi demonstrează gestionarea unor evenimente externe în paralel cu evenimentele generate de timere.

---

## Simulare Proteus

Fiecare implementare are propriul proiect **Proteus**:

```text
proteus/project.pdsprj
```

Simulările permit observarea comportamentului implementărilor fără a fi necesară rularea lor direct pe hardware.

Prin simulări pot fi urmărite:

- comutarea LED-urilor și a pinilor de ieșire;
- generarea overflow-urilor timerelor;
- execuția rutinelor de întrerupere;
- funcționarea mai multor timere;
- interacțiunea dintre întreruperile externe și cele generate de timere;
- utilizarea nivelurilor HIGH și LOW de prioritate pe PIC18F8722.

---

## Concepte demonstrate

| Concept | 8051 | PIC18F8722 |
|---|:---:|:---:|
| Programare Embedded C | ✓ | ✓ |
| Timere hardware | ✓ | ✓ |
| Polling | ✓ | |
| Întreruperi pentru timere | ✓ | ✓ |
| Utilizarea mai multor timere | ✓ | ✓ |
| Întreruperi externe | | ✓ |
| Priorități HIGH / LOW | | ✓ |
| Contoare software pentru evenimente | ✓ | ✓ |
| Simulare Proteus | ✓ | ✓ |

---

## Obiectiv tehnic

Proiectul demonstrează modul în care timerele hardware pot fi utilizate pentru generarea și gestionarea evenimentelor temporizate într-un sistem embedded.

Implementările pentru **8051** pornesc de la utilizarea Timer0 prin polling, continuă cu gestionarea Timer0 prin întrerupere și ajung la utilizarea simultană a Timer0 și Timer1.

Implementările pentru **PIC18F8722** extind aceste concepte prin utilizarea Timer0 cu întrerupere și, ulterior, prin integrarea simultană a trei timere și trei întreruperi externe, organizate pe două niveluri de prioritate.

Prin urmare, proiectul evidențiază concepte importante pentru programarea embedded la nivel de microcontroler: **configurarea registrelor perifericelor, timere hardware, overflow-uri, polling, ISR-uri, întreruperi externe și prioritizarea întreruperilor**.

---

# 🇬🇧 English

## About the project

This project is a collection of practical **Embedded C** implementations for **8051** and **PIC18F8722** microcontrollers, focused on hardware timers and interrupts.

The implementations progress from a simple **polling-based** solution to **interrupt-driven** solutions, the simultaneous use of multiple timers and, on the PIC18F8722, the handling of multiple interrupt sources using **different interrupt priority levels**.

Each implementation is accompanied by a **Proteus** project, allowing the behavior of the microcontroller and its peripherals to be observed through simulation.

## Project structure

```text
Timere-si-intreruperi-pentru-microcontrolerele — 8051-si-PIC18F8722/
│
├── README.md
│
├── 8051/
│   ├── 01-timer-polling/
│   │   ├── proteus/
│   │   │   └── project.pdsprj
│   │   └── src/
│   │       └── main.c
│   │
│   ├── 02-timer-interrupt/
│   │   ├── proteus/
│   │   │   └── project.pdsprj
│   │   └── src/
│   │       └── main.c
│   │
│   └── 03-dual-timer-interrupt/
│       ├── proteus/
│       │   └── project.pdsprj
│       └── src/
│           └── main.c
│
└── pic18f8722/
    ├── 01-timer0-interrupt/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    └── 02-multiple-timers-priority/
        ├── proteus/
        │   └── project.pdsprj
        └── src/
            └── main.c
```

---

## 8051

### 1. Timer0 using polling

**Location:** `8051/01-timer-polling/`

The first implementation uses **Timer0** of the 8051 microcontroller in **Mode 1**, configured as a 16-bit timer.

The timer is reloaded with:

```c
TH0 = 0xFC;
TL0 = 0x18;
```

The program starts the timer and actively checks the `TF0` flag until an overflow occurs:

```c
while(!TF0);
```

After the overflow, the timer is stopped and the flag is cleared.

The LED connected to **P1.0** is toggled after approximately 1000 Timer0 overflows.

This implementation demonstrates timer operation using **polling**, where the processor remains busy continuously checking the timer flag.

---

### 2. Timer0 using an interrupt

**Location:** `8051/02-timer-interrupt/`

The second implementation uses a **Timer0 interrupt**, eliminating the active waiting used in the previous example.

The interrupt service routine is:

```c
void timer0_isr() interrupt 1
```

On every overflow:

1. Timer0 is reloaded;
2. the counter is incremented;
3. after 1000 overflows, the LED on **P1.0** is toggled;
4. the counter is reset.

Timer0 interrupts and global interrupts are enabled using:

```c
ET0 = 1;
EA = 1;
```

The main loop remains free:

```c
while(1)
{}
```

The main difference compared with the first implementation is that the processor no longer has to actively wait for the timer overflow. The event is handled automatically through an **Interrupt Service Routine (ISR)**.

---

### 3. Two timers using interrupts

**Location:** `8051/03-dual-timer-interrupt/`

The third implementation uses **Timer0** and **Timer1** simultaneously.

The two timers independently control two LEDs:

- **Timer0** → LED on `P1.0`;
- **Timer1** → LED on `P1.1`.

Each timer has its own interrupt service routine:

```c
void timer0_isr() interrupt 1
void timer1_isr() interrupt 3
```

Both timers are started, and the interrupts are configured through the `IE` register:

```c
IE = 0x8A;
```

Each timer has its own software counter and toggles its associated LED after 1000 overflows.

This implementation demonstrates the **simultaneous use of multiple hardware timers** and the handling of multiple interrupt sources on the 8051.

---

# PIC18F8722

## 4. Timer0 using an interrupt

**Location:** `pic18f8722/01-timer0-interrupt/`

The first PIC18F8722 implementation uses **Timer0** configured as an 8-bit timer with an internal clock and a `1:16` prescaler.

The configuration is performed directly through the microcontroller registers:

```c
T08BIT = 1;
T0CS = 0;
PSA = 0;
T0PS2 = 1;
T0PS1 = 0;
T0PS0 = 0;
```

The Timer0 interrupt is enabled using:

```c
TMR0IE = 1;
TMR0IF = 0;
```

On every overflow, the following routine is executed:

```c
void interrupt t0()
```

A software counter is used inside the ISR. After reaching the configured value, a digital pattern is written to `LATD`:

```c
LATD = ch[i];
```

The pattern is defined as:

```c
char ch[]={0b01001010,0b1111010};
```

The main loop does not perform any additional operations:

```c
while (1){}
```

This implementation demonstrates the use of a hardware timer to generate periodic events handled through interrupts.

---

## 5. Multiple timers and interrupt priorities

**Location:** `pic18f8722/02-multiple-timers-priority/`

The final implementation combines multiple PIC18F8722 interrupt mechanisms and is the most complex implementation in the project.

The following sources are used simultaneously:

- **Timer0**;
- **Timer1**;
- **Timer2**;
- external interrupt **INT0**;
- external interrupt **INT1**;
- external interrupt **INT2**.

### Interrupt priorities

The PIC18F8722 is configured to use interrupt priorities:

```c
IPEN = 1;
GIEH = 1;
GIEL = 1;
```

The interrupt sources are divided into two levels.

**HIGH priority:**

- `INT0`;
- Timer0;
- Timer2.

**LOW priority:**

- `INT1`;
- Timer1;
- `INT2`.

The HIGH-priority interrupt handler is:

```c
void interrupt high_priority ex1()
```

while the LOW-priority handler is:

```c
void interrupt low_priority ex2()
```

Each interrupt source checks its own flag and calls the corresponding function.

### Timer0

Timer0 is configured as an 8-bit timer using the internal clock and a `1:32` prescaler:

```c
T08BIT = 1;
T0CS = 0;
PSA = 0;
T0PS2 = 1;
T0PS1 = 1;
T0PS0 = 0;
```

Timer0 has HIGH priority:

```c
TMR0IP = 1;
```

On every overflow, `counter` is incremented, and after 8 events, pin `RD2` is toggled.

### Timer1

Timer1 uses the internal clock and a `1:4` prescaler.

It is configured with LOW priority:

```c
TMR1IP = 0;
```

On every overflow, `counter1` is processed and pin `RD3` is toggled.

### Timer2

Timer2 is configured with a `1:4` prescaler and:

```c
PR2 = 255;
```

Timer2 has HIGH priority:

```c
TMR2IP = 1;
```

For every Timer2 event, `counter2` is incremented, and after 256 events, pin `RD4` is toggled.

### External interrupts

In addition to the timers, the implementation uses three external interrupts:

- `INT0` → `ext0()` → controls `RD0`;
- `INT1` → `ext1()` → controls `RD1`;
- `INT2` → `ext5()` → controls `RD5`.

The trigger edge for the three external interrupts is configured using:

```c
INTEDG0 = 1;
INTEDG1 = 1;
INTEDG2 = 1;
```

These interrupts demonstrate the handling of external events in parallel with timer-generated events.

---

## Proteus simulation

Each implementation contains its own **Proteus** project:

```text
proteus/project.pdsprj
```

The simulations allow the behavior of the implementations to be observed without requiring the code to run directly on physical hardware.

The simulations can be used to observe:

- LED and output pin toggling;
- timer overflow events;
- interrupt service routine execution;
- multiple timers operating simultaneously;
- interaction between external interrupts and timer interrupts;
- the use of HIGH and LOW interrupt priority levels on the PIC18F8722.

---

## Demonstrated concepts

| Concept | 8051 | PIC18F8722 |
|---|:---:|:---:|
| Embedded C programming | ✓ | ✓ |
| Hardware timers | ✓ | ✓ |
| Polling | ✓ | |
| Timer interrupts | ✓ | ✓ |
| Multiple timers | ✓ | ✓ |
| External interrupts | | ✓ |
| HIGH / LOW priorities | | ✓ |
| Software event counters | ✓ | ✓ |
| Proteus simulation | ✓ | ✓ |

---

## Technical objective

The project demonstrates how hardware timers can be used to generate and handle timed events in a microcontroller-based embedded system.

The **8051** implementations start with Timer0 using polling, continue with Timer0 interrupt handling, and progress to the simultaneous use of Timer0 and Timer1.

The **PIC18F8722** implementations extend these concepts by using Timer0 with interrupts and, finally, by integrating three timers and three external interrupts organized across two priority levels.

As a result, the project demonstrates important low-level embedded programming concepts, including **peripheral register configuration, hardware timers, overflow events, polling, ISRs, external interrupts, and interrupt prioritization**.

---

## 👤 Autor / Author

**IonutD**
