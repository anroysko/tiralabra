# Määrittelydokumentti

Työssä toteutetaan Dinic's algorithm maximum flow -ongelman ratkaisemiseen. Se implementoidaan Link/Cut Treen avulla. Toteutetaan Myös Link/Cut Treen sisäisesti käyttämä Splay Tree.

Jo normaali, pelkästään depth first searchia käyttävä dinic toimii käytännössä todella nopeasti, melkein lineaarisesti, vaikka sen aikavaatimus onkin `O(V^2 E)`.

![random_dinic_1.jpg](https://github.com/anroysko/tiralabra/blob/master/doc/pictures/random_dinic_1.png)

Käyttämällä link/cut treetä polkujen saturoimiseen suoraviivaisen DFS:n sijasta, aikavaativuus on `O(VE log V)`. Näin saadaan käytännössä nopea algoritmi joka on vain logaritmisen kertoimen päässä parhaasta tunnetusta algoritmistä. 

Ohjelma saa syötteeksi suunnatun verkon, edgejen kapasiteetit ja sinkin sekä sourcen indeksit verkossa. Se tuottaa jonkin maksimaalisen flown kyseiselle graafille. Teen ohjelman c++:lla.

Algoritmin aikavaatimus on `O(VE log V)`, missä `V` on vertexien määrä ja `E` Edgejen määrä.

[Dinic's Algorithm](https://en.wikipedia.org/wiki/Dinic's_algorithm])

[Link/Cut tree](https://en.wikipedia.org/wiki/Link/cut_tree)

[Original Link/Cut tree paper](https://dl.acm.org/citation.cfm?id=3835)

[Maximum flow problem](https://en.wikipedia.org/wiki/Maximum_flow_problem)

Bonus: Kuva yhden Link/Cut treen kehittäjistä, Daniel Sleatorin kanssa, kiinan muurilla:

![greatwallsleator.jpg](https://github.com/anroysko/tiralabra/blob/master/doc/pictures/greatwallsleator.jpg)
