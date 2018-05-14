# Testausdokumentti

## Ajanotto
Kaikille projektin osille, eli Splay-puulle, Link/Cut-puulle, ja Dinic-algoritmille on performanssitestit. Testeissä mitataan c++:n chrono-headerin [high resolution clock](http://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now)-in avulla koodin käyttämää aikaa.

## Testisyötteet
Testaukseen on käytetty random-syötteitä, ja niiden lisäksi erilaisia reunatapauksia testaavia syötteitä.
- Splay-puun syötteessä vähän ajan välein accessataan jokainen puun node järjestyksessä, milloin siitä tulee n:n noden ketju.
- Link/Cut treelle tein sekä testin joka testaa erityisen syviä puita, että testin joka testaa Leveitä puita, joissa jokaisella kyselyllä preferred edge pitää vaihtaa pahin mahdollinen määrä kertoja.
- Dinic:lle tein syötteen, jossa algoritmi käyttää `O(VE log V)` aikaa, tai ilman link/cut treetä `O(V^2 E)`.

## Kuvaajia

Kuvassa x-akseli kuvaa testin indeksiä. i:nnessä testissä on `2^(i + 1)` nodea ja `2^(4 + i)` kyselyä. Viivat kuvaavat testin tyyppiä.
![link/cut plot](https://github.com/anroysko/tiralabra/blob/master/doc/pictures/link_cut_performance.png)


Koodin testit voi suorittaa ajamalla komennon
```
make test
```
Ja performanssitestit komennolla
```
make performance
```


