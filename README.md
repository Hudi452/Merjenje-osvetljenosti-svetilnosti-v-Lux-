# Merjenje-osvetljenosti-svetilnosti-v-Lux-

Opis merilnika:
Merilnik svetilnosti meri osvetljenost okolice s pomočjo fotoupora (LDR), ki tvori skupaj z uporom delilnik napetosti. Napetost delilnika se s spreminjanjem osvetljenosti spreminja. Napetost delilnika se meri s pomočjo analognega vhoda na Arduinu. Arduino nato izmerjeno napetost preko programa pretvori v svetilnost. Merilnik je kalibriran s pomočjo različnih meritev upornosti LDR pri različnih vrednostih osvetljenosti. 

Merilnik lahko napajamo na 2 načina:
- preko USB
- preko vgrajene baterije v ohišju

Osvetljenost se lahko v enoti lux prikaže na serijskem zaslonu ali pa na I2C LCD-ju. S stikalom poleg LCD-ja pa lahko preklapljamo med dvema načinoma prikazovanja na LCD-ju. Če je stikalo v položaju HIGH, se na LCD-ju izpiše izmerjena osvetljenost v lux-ih. Če pa je stikalo v položaju LOW, pa se na LCD-ju prikaže osvetljenost s kvadratki, pri čemer vsak kvadratek predstavlja določeno osvetljenost.  

Merilnik ima tudi 3 svetleče diode različnih barv (zelena, rumena in rdeča). Zelena svetleča dioda se prižge, ko je raven osvetljenosti prostora nizka, rumena svetleča dioda se prižge, ko je raven osvetljenosti prostora priporočena, rdeča svetleča dioda pa se prižge, ko je raven osvetljenosti prostora prevelika.
