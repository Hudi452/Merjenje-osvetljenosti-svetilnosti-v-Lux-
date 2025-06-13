# Merjenje-osvetljenosti-svetilnosti-v-Lux-

Opis merilnika:

Merilnik svetilnosti meri osvetljenost okolice s pomočjo fotoupora (LDR), ki tvori skupaj z uporom delilnik napetosti. Napetost delilnika se s spreminjanjem osvetljenosti spreminja. Napetost delilnika se meri s pomočjo analognega vhoda na Arduinu. Arduino nato izmerjeno napetost preko programa pretvori v svetilnost. Merilnik je kalibriran s pomočjo različnih meritev upornosti LDR pri različnih vrednostih osvetljenosti. LDR ima lahko dva različna predupora med katerima lahko preklapljamo s tranzistorjem, ki ima bazo priključeno na digitalni vhod Arduina.

Merilnik lahko napajamo na 2 načina:
- preko USB
- preko vgrajene baterije v ohišju

Osvetljenost se lahko v enoti lux prikaže na serijskem zaslonu ali pa na I2C LCD-ju. S stikalom poleg LCD-ja pa lahko preklapljamo med dvema načinoma prikazovanja na LCD-ju. Če je stikalo v položaju HIGH, se na LCD-ju izpiše izmerjena osvetljenost v lux-ih. Če pa je stikalo v položaju LOW, pa se na LCD-ju prikaže osvetljenost s kvadratki, pri čemer vsak kvadratek predstavlja določeno osvetljenost.  

Merilnik ima tudi 3 svetleče diode različnih barv (zelena, rumena in rdeča). Zelena svetleča dioda se prižge, ko je raven osvetljenosti prostora nizka, rumena svetleča dioda se prižge, ko je raven osvetljenosti prostora priporočena, rdeča svetleča dioda pa se prižge, ko je raven osvetljenosti prostora prevelika.

Kosovnica/seznam uporabljenih materialov in komponent:
- Arduino Nano,
- LDR,
- I2C LCD,
- 3x LED (zelena, rumena, rdeča),
- 3x 330E upori,
- 10k upor,
- 2x 1k upor,
- NPN tranzistor (BC547B),
- stikalo,
- 9V baterija,
- 2x perfBoard,
- 3D tiskano ohišje.

Vezalna shema v EasyEda:

![image](https://github.com/user-attachments/assets/0e05291b-1e8b-4d22-8faf-3d609cb427c0)

Komentar: 

Merilna naprava je kalibrirana z 49 meritvami osvetljenosti in upornosti LDR-ja. Najbolj natančna je med 0 in 1000 lux. Zadovoljivo natančno lahko izmerimo osvetljenost prostora do 2000 lux. Za osvetljenosti večje od 2000 lux pa je odstopanje lahko nekoliko večje, saj merilnik nima tako veliko referenčnih meritev v tem območju. Merilnik je namenjen za merjenje osvetljenosti v zgradbah (nižje osvetljenosti) in ne za merjenje osvetljenosti zunaj zgradbe (zelo visoke osvetljenosti). Programska oprema merilnika je fleksibilna, kar pomeni, da lahko kadarkoli dodamo nove referenčne vrednosti ter s tem povečamo natančnost merilnika.
