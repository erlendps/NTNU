---
geometry: margin=2cm
header-includes: |
    \usepackage{proof}
output: 
  pdf_document:
    extra_dependencies: ["proof"]
---

# Exercise 5

## Task 1

### 1.1

a) It is entailed.

|$A$|$B$|$\neg A \wedge \neg B$|$\neg A \wedge \neg B \implies \neg B$|
|:-:|:-:|:-:|:-:|
|F|F|T|T|
|F|T|F|T|
|T|F|F|T|
|T|T|F|T|

b) It is not entailed.

|$A$|$B$|$\neg A \vee \neg B$|$\neg A \vee \neg B \implies \neg B$|
|:-:|:-:|:-:|:-:|
|F|F|T|T|
|F|T|T|F|
|T|F|T|T|
|T|T|F|T|

c) It is not entailed.

|$A$|$B$|$\neg A \wedge \neg B$|$\neg A \wedge \neg B \implies A \vee B$|
|:-:|:-:|:-:|:-:|
|F|F|T|F|
|F|T|F|T|
|T|F|F|T|
|T|T|F|T|

d) It is not entailed.

|$A$|$B$|$A \implies B$|$A \iff  B$|$(A \implies B) \implies (A \iff  B)$|
|:-:|:-:|:-:|:-:|:-:|
|F|F|T|T|T|
|F|T|T|F|F|
|T|F|F|F|T|
|T|T|T|T|T|

e) It is entailed.

|$A$|$B$|$C$|$A \implies B$|$(A \implies  B)$\
$\iff C$|$A \vee \neg B \vee C$|$((A \implies  B)$\
$\iff C)$\
$\implies (A \vee \neg B \vee C)$|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|F|F|F|T|F|T|T|
|F|F|T|T|T|T|T|
|F|T|F|T|F|F|T|
|F|T|T|T|T|T|T|
|T|F|F|F|T|T|T|
|T|F|T|F|F|T|T|
|T|T|F|T|F|T|T|
|T|T|T|T|T|T|T|

f) It is satisfiable for $A \wedge \neg B$

|$A$|$B$|$\neg A \implies \neg B$|$A \wedge \neg B$|$(\neg A \implies \neg B)$\
$\wedge (A \wedge \neg B)$|
|:-:|:-:|:-:|:-:|:-:|
|F|F|T|F|F|
|F|T|F|F|F|
|T|F|T|T|T|
|T|T|T|F|F|

g) It is not satisfiable

|$A$|$B$|$\neg A \iff \neg B$|$A \wedge \neg B$|$(\neg A \iff \neg B)$\
$\wedge (A \wedge \neg B)$|
|:-:|:-:|:-:|:-:|:-:|
|F|F|T|F|F|
|F|T|F|F|F|
|T|F|F|T|F|
|T|T|T|F|F|

### 1.2

a) I assume it means the packet you can send. In that case, the vocabulary is:
$$
(0|1)\{8\}
$$
i.e 0 or 1 exactly 8 times.

b) For sensor $S_2$ and $S_3$, I assume a high value is equal to a True value.
$C_1$: $\neg S_2 \wedge \neg S_3$
$C_2$: $\neg S_1 \wedge S_2$
$C_3$: $S_3$

c) For each tank, this would be the truth table.

|$S_1$|$S_2$|$S_3$|$\neg S_2 \wedge \neg S_3$|$\neg S_1 \wedge S_2$|
|:-:|:-:|:-:|:-:|:-:|
|F|F|F|T|F|
|F|F|T|F|F|
|F|T|F|F|T|
|F|T|T|F|T|
|T|F|F|T|F|
|T|F|T|F|F|
|T|T|F|F|F|
|T|T|T|F|F|

d) If we receive the packet $01000110$, we first analyze the last 5 bits of the packet, which in shortend form is $110_2 = 6_{10}$, i.e the tank with id 6. For the 3 first bits, we place the values in the truth table. In this case, $010$ is mapped to row 3 of the truth table. We then see that $C_2$ is evaluated to true (i.e for the current KB, we can entail $C_2$). The system then closes the gate.

## Task 2

### 2.1

a)
$$A \vee (B \wedge C \wedge \neg D) \equiv (A \vee B) \wedge (A \vee C) \wedge (A \vee \neg D)
$$

b)
$$
\begin{aligned}
& \neg(A \implies \neg B) \wedge \neg(C \implies \neg D)\\
\equiv & \neg(\neg A \vee \neg B) \wedge \neg(\neg C \vee \neg D)\\
\equiv & (A \wedge B) \wedge (C \wedge D)\\
\equiv & A \wedge B \wedge C \wedge D
\end{aligned}
$$

c)
$$
\begin{aligned}
& \neg((A \implies B) \wedge (C \implies D))\\
\equiv & \neg((\neg A \vee B) \wedge (\neg C \vee D))\\
\equiv & \neg(\neg A \vee B) \vee \neg(\neg C \vee D)\\
\equiv & (A \wedge \neg B) \vee (C \wedge \neg D)\\
\equiv & ((A \wedge \neg B) \vee C) \wedge ((A \wedge \neg B) \vee \neg D)\\
\equiv &(A \vee C) \wedge (\neg B \vee C) \wedge (A \vee \neg D) \wedge (\neg B \vee \neg D)
\end{aligned}
$$

d)
$$
\begin{aligned}
& (A \wedge B) \vee (C \implies D)\\
\equiv & (A \wedge B) \vee (\neg C \vee D)\\
\equiv & (A \vee \neg C \vee D) \wedge (B \vee \neg C \vee D)
\end{aligned}
$$

e)
$$
\begin{aligned}
& A \iff (B \implies \neg C)\\
\equiv &(A \implies (B \implies \neg C)) \wedge ((B \implies \neg C) \implies A)\\
\equiv & (\neg A \vee \neg B \vee \neg C) \wedge (\neg B \vee \neg C \vee A)\\
\equiv & (\neg A \vee \neg B \vee \neg C) \wedge (A \vee \neg B \vee \neg C)
\end{aligned}
$$

### 2.2

Building the knowledge base:
$S$ is true if it's sunny.
$H$ is true if it's warm.
$R$ is true if it's raining.
$E$ is true if I will enjoy.
$B$ is true if I pick up berries.
$W$ is true if I'm wet.

This gives the knowledge:
$$
\begin{aligned}
R_1: & \ (S \wedge H) \implies E \equiv \neg S \vee \neg H \vee E\\
R_2: & \ (H \wedge \neg R) \implies B \equiv \neg H \vee R \vee B\\
R_3: & \ R \implies \neg B \equiv \neg R \vee \neg B\\
R_4: & \ R \implies W \equiv \neg R \vee W\\
R_5: & \ H\\
R_6: & \ R\\
R_7: & \ S
\end{aligned}
$$

a)
Using $R_3$ and $R_6$ we create the unit resolution rule:
$$
\infer{\neg B}{\neg R \vee \neg B, \ \ \ \ \ \ R}
$$
which means that $R_8: \neg B$ can be added to the knowledge base. We then know that $Q_1$ will be true.

b)
Using $R_1$ and $R_7$, we can resolve by doing the following:

$$
\infer{\neg H \vee E}{\neg S \vee \neg H \vee E, \ \ \ \ \ \ S}
$$

So $R_9: \neg H \vee E$ is added to the knowledge base. Then using $R_9$ and $R_5$, we get the resolution rule:

$$
\infer{E}{\neg H \vee E, \ \ \ \ \ \ H}
$$

So we can also add $R_{10}: \ E$ to the KB. This means that also $Q_2$ is proven.

c)
Using $R_4$ and $R_6$ we get the resolution rule:

$$
\infer{W}{\neg R \vee W, \ \ \ \ \ \ R}
$$

We add $R_{11}$ to the KB, and we have therefore proven $Q_3$.

## Task 3

### 3.1

a)
$Occupation(Emily, Lawyer) \vee Occupation(Emily, Doctor)$

b)
$Occupation(Joe, Actor) \wedge \exists x \ Occupation(Joe, x) \implies x \neq Actor$

c)
$\forall x \ Occupation(x, Surgeon) \implies Occupation(x, Doctor)$

d)
$\neg \exists x \ Customer(Joe, x) \implies Occupation(x, Lawyer)$

e)
$\exists x \ Boss(x, Emily) \implies Occupation(x, Lawyer)$

f)
$\exists x \forall y \ Occupation(x, Lawyer) \wedge (Customer(y, x) \implies Occupation(y, Doctor))$

g)
$\forall x \ \exists y \ Occupation(x, Surgeon) \implies (Customer(y, x) \wedge Occupation(y, Lawyer))$

### 3.2

a)
$Divisible(x,y): \exists z \ (z < x) \wedge (x = y * z)$

b)
$Even(x): Divisible(x, 2)$

c)
$Odd(x): \neg Divisible(x, 2)$

d)
$Odd(x): Even(x) \implies Odd(x+1)$

e)
$Prime(x): \forall y \ Divisible(x, y) \implies y = x$

f)
$\exists ! x Prime(x) \wedge Even(x)$

g)
$\forall x \exists k \ (x = \prod_{i = 0}^{k}p_i) \implies \forall i \ Prime(p_i)$

## Task 4

First we define some predicates and constants:

$Identifies(x, y)$ - a user $x$ is a member of the $y$ fandom.
$Likes(x,y)$ - a user $x$ likes the group $y$.
$LikesGenre(x, y)$ - a user $x$ likes the genre $y$
$Sone, Reveluvs, Blinks$ are fandoms.
$GG, RV, BP, CH, HE, DJH, SEO, TAE$ are groups.
$Dance, Ballads, Drama, Electro$ are genres.

a) Our KB is the following (note $x$ is the universally quantifiable variable):
$$
\begin{aligned}
R_1: & \ Identifies(x, Sone) \iff Likes(x, GG)\\
\equiv & \ (\neg Identifies(x, Sone) \vee Likes(x, GG)) \wedge (\neg Likes(x, GG) \vee Identifies(x, Sone)) \\
R_2: & \ Identifies(x, Reveluvs) \iff Likes(x, RV)\\
\equiv & \ (\neg Identifies(x, Reveluvs) \vee Likes(x, RV)) \wedge (\neg Likes(x, RV) \vee Identifies(x, Reveluvs)) \\
R_3: & \ Identifies(x, Blinks) \iff Likes(x, BP)\\
\equiv & \ (\neg Identifies(x, Blinks) \vee Likes(x, BP)) \wedge (\neg Likes(x, BP) \vee Identifies(x, Blinks)) \\
R_4: & \ Identifies(x, Reveluvs) \implies LikesGenre(x, Ballads)\\
\equiv & \ \neg Identifies(x, Reveluvs) \vee LikesGenre(x, Ballads)\\
R_5: & \ Identifies(x, Blinks) \implies LikesGenre(x, Dance)\\
\equiv & \ \neg Identifies(x, Blinks) \vee LikesGenre(x, Dance)\\
R_6: & \ (LikesGenre(x, Dance) \wedge LikesGenre(x, Ballads)) \implies Likes(x, CH)\\
\equiv & \ \neg LikesGenre(x, Dance) \vee \neg LikesGenre(x, Ballads) \vee Likes(x, CH)\\
R_7: & \ (LikesGenre(x, Drama) \wedge LikesGenre(x, Ballads)) \implies Likes(x, HE)\\
\equiv & \ \neg LikesGenre(x, Drama) \vee \neg LikesGenre(x, Ballads) \vee Likes(x, HE)\\
R_8: & \ (Identifies(x, Sone) \wedge LikesGenre(x, Electro)) \implies Likes(x, DJH)\\
\equiv & \ \neg Identifies(x, Sone) \vee \neg LikesGenre(x, Electro) \vee Likes(x, DJH)\\
R_9: & \ (Identifies(x, Sone) \wedge LikesGenre(x, Dance)) \implies Likes(x, SEO)\\
\equiv & \ \neg Identifies(x, Sone) \vee \neg LikesGenre(x, Dance) \vee Likes(x, SEO)\\
R_{10}: & \ (Identifies(x, Sone) \wedge LikesGenre(x, Ballads)) \implies Likes(x, TAE)\\
\equiv & \ \neg Identifies(x, Sone) \vee \neg LikesGenre(x, Ballads) \vee Likes(x, TAE)\\
\end{aligned}
$$

b)
We add the rules to the KB: $R_{11}: \ Identifies(u_1, Reveluvs)$ and $R_{12}: \ Likes(u_1, GG)$. We can then use $R_4$ to get the resolution rule (and by substituting $\theta=\{x/u_1\}$):
$$
\infer{LikesGenre(u_1, Ballads)}{\neg Identifies(u_1, Reveluvs) \vee LikesGenre(u_1, Ballads) \ \ \ \ \ \ Identifies(u_1, Reveluvs)}
$$
$$
R_{13}: \ LikesGenre(u_1, Ballads)
$$

We then use part of $R_1$ to prove the user also identifies as a $Sone$
$$
\infer{Identifies(u_1, Sone)}{\neg Likes(u_1, GG) \vee Identifies(u_1, Sone) \ \ \ \ \ \ Likes(u_1, GG)}
$$
$$
R_{14}: \ Identifies(u_1, Sone)
$$

We then use $R_{10}$:
$$
\infer{Likes(u_1, TAE) \vee \neg LikesGenre(u_1, Ballads)}{\neg Identifies(u_1, Sone) \vee \neg LikesGenre(u_1, Ballads) \vee Likes(u_1, TAE) \ \ \ \ \ \ Identifies(u_1, Sone)}
$$
$$
\infer{Likes(u_1, TAE)}{\neg LikesGenre(u_1, Ballads) \vee Likes(u_1, TAE) \ \ \ \ \ \ LikesGenre(u_1, Ballads)}\\\\
$$
$$
R_{15}: \ Likes(u_1, TAE)
$$
According to resolution, $TAE$ will be a good recommendation.

c)
Using $R_7$:
$$
\infer{\neg LikesGenre(u_1, Drama) \vee Likes(u_1, HE)}{\neg LikesGenre(u_1, Drama) \vee \neg LikesGenre(u_1, Ballads) \vee Likes(u_1, HE) \ \ \ \ \ \ LikesGenre(u_1, Ballads)}
$$
There is no complementary literal for $\neg LikesGenre(u_1, Drama)$, i.e we don't know if $u_1$ likes $Drama$, nor can we infer it. We therefore are unable to prove that $HE$ will be a good recommendation. Consequentially, $HE$ will not be a good recommendation.

d)
We know $u_2$ is a $Sone, Reveluv$ and $Blink$, which from $R_1, R_2$ and $R_3$ we can infer that $u_2$ likes $GG, RV$ and $BP$. From $R_4$ and $R_5$, we can infer that $u_2$ likes $Ballads$ and $Dance$. So the system should recommend these genres. From $R_6$, we infer that $CH$ is a good recommendation. Then, we can also infer from $R_{10}$ that they also will like $TAE$. To summarize, the system will recommend $Ballads$ and $Dance$ as the genres, and $CH$ and $TAE$ as the groups.
