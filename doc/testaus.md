# Testausdokumentti

## Ajanotto
Kaikille projektin osille, eli Splay-puulle, Link/Cut-puulle, ja Dinic-algoritmille on performanssitestit. Testeissä mitataan c++:n chrono-headerin [high resolution clock](http://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now)-in avulla koodin käyttämää aikaa.

## Testisyötteet
Testaukseen on käytetty random-syötteitä, ja niiden lisäksi erilaisia reunatapauksia testaavia syötteitä.
- Splay-puun syötteessä vähän ajan välein accessataan jokainen puun node järjestyksessä, milloin siitä tulee n:n noden ketju.
- Link/Cut treelle tein sekä testin joka testaa erityisen syviä puita, että testin joka testaa Leveitä puita, joissa jokaisella kyselyllä preferred edge pitää vaihtaa pahin mahdollinen määrä kertoja.
- Dinic:lle tein syötteen, jossa algoritmi käyttää `O(VE log V)` aikaa, tai ilman link/cut treetä `O(V^2 E)`.

## Kuvia ja kuvaajia

Dinic'n kuvaajista löytyy logaritmisen y-akselin versiot kuvat-kansiosta.

### Vaikea dinic-syöte

Syötteessä kolmen pitkän ketjun avulla voidaan päättää kuinka monta nodea pitkä "edge" on. Näin saavutetaan, että lyhin  reitti kulkee ensin yläpuolelta a-a -klikkiä alas, kulkien näin kaikki `a^2` reittiä läpi. Seuraavalla kerralla lyhyin reitti tuleekin alapuolelta ylöspäin, kulkien taas koko klikin `a^2` reittiä, tällä kertaa ylöspäin. Lyhyimmän reitin pituus kasvaa aina kahdeksalla, joten algoritmi käy läpi `b/(3*8)` eri pituista reittiä, joissa i:nnessä kuljetaan klikin jälkeen `4*i + 1`:n pituinen polku sinkkiin. Näin saadaan aikavaativuus `O(a^2 b^2)` dfs-dinicillä, ja `(a^2 b log b)` link/cut dinicillä. Edgejä on `O(a^2 + b)` ja nodeja `O(a + b)`, joten aikavaativuus on juuri `O(V^2 E)` tai `O(VE log V)` kun `a ~ b`

![hard dinic input](https://github.com/anroysko/tiralabra/blob/master/doc/pictures/dinic_hard_test.png)

### Link/Cut treen kompleksisuus

Kuvassa x-akseli kuvaa testin indeksiä, ja y-akseli käytettyjä sekuntteja. i:nnessä testissä on `2^(i + 1)` nodea ja `2^(4 + i)` kyselyä. Viivat kuvaavat testin tyyppiä. Vihreä viiva on `O(Q log V)` kasvava kuvaaja, missä `Q` on kyselyiden määrä ja `V` nodejen, sopivalla vakiolla jotta se sopisi dataan.
![link/cut plot](https://github.com/anroysko/tiralabra/blob/master/doc/pictures/link_cut_performance.png)

### Random-inputeilla dinic'n kompleksisuus

i:nnessä testissä `V` on `2^(i)`, ja `E` on `2^(i + 4)`.

Tästä kuvaajasta näkyy hyvin kuinka dinic on käytännössä hyvin nopea algoritmi. Link/Cut dinic ohittaa perusversion vasta erittäin isoissa syötteissä, ja sekin saattaa olla vain sattumaa (Testit ajetaan vain yhdellä seedillä).

Mixed-viiva on versio algoritmistä, joka level graphin syvyydestä riippuen joko käyttää link/cut treetä tai vain dfs:ttää. Se siis hyödyntää sekä dfs:n vakiokertoimen että link/cut treen aikavaativuuden, mutta koska on vaikea tietää milloin link/cut treetä kannattaa käyttää, se on pienissä testeissä usein hitaampi kuin suora dfs.

![random dinic plot](https://github.com/anroysko/tiralabra/blob/master/doc/pictures/random_dinic_1.png)

### Vaikeassa testissä dinic'n kompleksisuus

i:nnessä testissä `a` on `2^(i / 2 + 1)` (Jakolasku pyöristettynä alaspäin) ja `b` on `2^((i+1)/2)`. Heittely keltaisessa viivassa johtuu juuri siitä että `a` ja `b` tuplaantuvat testeissä vuoronperään. `b` aiheuttaa paljon pienemmän vakiokertoimen kuin `a`.

![hard dinic plot](https://github.com/anroysko/tiralabra/blob/master/doc/pictures/hard_dinic_1.png)

## Testien toistaminen

Koodin testit voi suorittaa ajamalla komennon
```
make test
```
Ja performanssitestit komennolla
```
make performance
```
Kun ajat testit ensimmäistä kertaa, ne pitää ensin generoida. Siinä kestää useampi minuutti. performanssitestien ajamisessa kestää noin kymmenen minuuttia.

Performanssitestien output ajokerralta jolta otin ajat kuvaajiin:
https://raw.githubusercontent.com/anroysko/tiralabra/master/doc/example_performance_test_output.txt
