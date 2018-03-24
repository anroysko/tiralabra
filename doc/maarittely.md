# Määrittelydokumentti

Työssä toteutetaan Dinic's algorithm maximum flow -ongelman ratkaisemiseen. Se implementoidaan Link/Cut Treen avulla. Toteutetaan Myös Link/Cut Treen sisäisesti käyttämä Splay Tree.

Kyseiset tietorakenteet on valittu koska niillä saadaan ratkaistua maximum flow -ongelma käytännossä nopeasti, ja asymptioottinen aikavaatimus on vain logaritmisen kertoimen päässä parhaasta tunnetusta algoritmistä.

Ohjelma saa syötteeksi suunnatun verkon, edgejen kapasiteetit ja sinkin sekä sourcen indeksit verkossa. Ohjelman teen C++:lla. Se tuottaa jonkin maksimaalisen flown kyseiselle graafille.

Algoritmin aikavaatimus on O(VE log(V)), missä V on vertexien määrä ja E Edgejen määrä.

[Dinic's Algorithm](https://en.wikipedia.org/wiki/Dinic's_algorithm])

[Link/Cut tree](https://en.wikipedia.org/wiki/Link/cut_tree)

[Maximum flow problem](https://en.wikipedia.org/wiki/Maximum_flow_problem)

