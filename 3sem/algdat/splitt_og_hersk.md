# Splitt og hersk

Splitt og hersk går ut på å splitte et problem "i midten". Deler opp i flere delproblemer.

## Rekurrenser 

* rekurrenser er rekursive ligninger
* Poenget er å bruke den rekursive formelen til å jobbe seg nedover til et grunntilfelle.

Eks:
$T(n) = 1 + T(n-1)$
$T(n) = 1 + 1 + T(n-1-1)$
$T(n) = 1 + 1 + ... + 1 + T(n-n+1)$
$T(n) = 1 + 1 + ... + T(1)$
$T(n) = n$

### Oppgave: 20 spørsmål

Løsning: Binærsøk. Del resten i to ca like store mengder. Man må da anta at dette er mulig
Tjue spørsmål gir 2^20 = 10^(20/3.32).

Tenkte ikke på binærsøk. Tenkte ikke at man kunne anta at man kan alltid dele i 2 hver gang.

### Verifikasjon med induksjonsbevis

Gitt antagelsen $T(n-1) = n - 1$, vis at $T(n) = n$

Vi har at
$T(n) = T(n-1) + 1$
Og så bruker vi antagelsen vår
$T(n) = n - 1 + 1$
$T(n) = n$
Som konkluderer beviset.

## Masterteoremet

Bruker masterteoremet når vi vil løse rekurrenser på formen:
$T(n) = aT(n/b) + f(n)$

## Merge-sort

* Sorter venstre halvdel rekursivt
* Sorter høyre halvdel rekursivt
* Flett sammen halvdelene

Merge-Sort(A, p, r)

```python

if p < r:
    q = floor((p+r)/2)
    Merge-Sort(A, p, q)
    Merge-Sort(A, q+1, r)
    Merge(A, p, q, r)
```

## Quick-sort

* Skill "små" og "store" tall
* Sorter små tall rekursivt
* Sorter store tall rekursivt
