### Forelesning 23.08.21
* I all hovedsak handler faget om å dekomponere problemer til mindre problemer og
* Prøve å finne en løsning for et mindre utvalg.

#### Refleksjon håndtrykkoppgave
* Finnes ofte summer
* Personlig tenker jeg for komplisert.

#### Dekomponering
* Står nært induksjon
    * bevise grunntilfelle.
    * Bevise at det gjelder for en vilkårlig verdi k.
    * Viser at dette medfører at k+1 også er sann. 
    * Ved induksjon er det bevist at det gjelder for enhver k.
* Del opp i mindre problemer
* Anta man kan løse disse mindre problemene
* Bruker delløsningene for å løse hovedproblemet

#### Insertion sort algoritme
Insertion-Sort(A)

```
for j = 2 to A.length
    key = A[J]
    i = j-1
    while i > 0 and A[i] > key
        A[i+1] = A[i]
        i = i - 1
    A[i] = key
```

#### Asymptotisk notasjon
* Dropp konstanter og ta høyeste orden, altså droppe lavere orden


