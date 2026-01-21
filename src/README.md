# Metodologia Seturilor de Teste

Seturile de teste se împart în **3 categorii**: 
* Teste aleatoare;
* Teste cu valori apropiate de jumătate;
* Teste cu valori și mici și mari, dar sub jumătate. 

**Intenția** era de a testa cum se comportă algoritmul pe:
1. Seturi de valori care duc la umplerea unui bin cu puține pachete, în cazul testelor cu valori în jurul jumătății capacității;
2. Seturi de valori care produc binuri cu multe elemente în ele, în cazul testelor cu valori mici și mari sub jumătate;
3. Seturi de valori aleatoare.

---

### 1. Construcția seturilor **aleatoare**
* Acestea sunt generate random, fiecare pachet având o valoare aleatoare între 1 și capacitatea - 1. 
* Simulează un scenariu în care datele nu au o distribuție cunoscută și nu pot fi anticipate. 
* Testează comportamentul algoritmului în astfel de condiții. 

### 2. Construcția seturilor **near-half**
* Aceste teste generează valori apropiate ca ordin de mărime de jumătatea capacității, mai precis cu o abatere de $\pm 10^{\frac{\lg(\text{capacity})}{2}}$. 
* Valorile sunt distribuite egal între valori mai mici că jumatateate capacității și valori mai mari.
* Acestea au că tintă algoritmii euristici, în special **first fit**, care pune elementul în primul bin în care găsește loc. 
* Setul de date testează acuratețea algoritmilor greedy.



### 3. Construcția seturilor **two-cat**
* Aceste teste generează valori mici, sub jumătate. 
* Seturile de date conțin fie valori mai mici decât a 10-a parte din capacitate, fie valori mai mari decât a 3-a parte din capacitate. Valorile sunt distribuite egal în cele două categorii.
* Aceste teste au ca scop compararea comportamentului dintre metodele exhaustive (**backtracking**) și cele euristice. 
* Valorile mici produc multe combinări ale pachetelor în containere, care rezultă într-un timp mai mare pentru backtracking. 

> **Soluție:** O soluție pentru a micșora acest timp este să sortăm elementele descrescător și să eliminăm ramificațiile algoritmului backtracking când ramura curentă trece de numărul minim de binuri determinat anterior.