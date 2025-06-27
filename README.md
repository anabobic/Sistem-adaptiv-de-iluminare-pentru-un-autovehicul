# Sistem adaptiv de iluminare pentru un autovehicul


##  Descriere generală
Acest proiect constă în implementarea unui sistem adaptiv de iluminare pentru autovehicule, care reglează direcția și intensitatea luminilor în funcție de viteză, unghiul de viraj și condițiile simulate. Proiectul este dezvoltat în două componente:
- o parte hardware controlată cu microcontroller ESP32;
- o simulare software în LabVIEW.


##  Repository Git
Codul sursă complet este disponibil la adresa:
[https://github.com/anabobic/Sistem-adaptiv-de-iluminare-pentru-un-autovehicul](https://github.com/anabobic/Sistem-adaptiv-de-iluminare-pentru-un-autovehicul)


##  Livrabilele proiectului
Structura principală a repository-ului este următoarea:
─ Cod sursă pentru Arduino IDE (.ino)
─ Fișier LabVIEW pentru simulare (.vi)
─ README.md 


## Pașii de compilare – Arduino
1. Se descarcă și se instalează **Arduino IDE** 
2. Se deschide fișierul `ARDUINO_iluminare_control.ino`
3. Se selectează tipul de placă utilizat: `Tools > Board > ESP32 MODULE` 
4. Se selectează portul de conectare corespunzător: `Tools > Port`.
5. Se apasă butonul **Verify** pentru compilarea codului sursă.
6. Se apasă butonul **Upload** pentru încărcarea codului pe placa Arduino.


##  Pașii de compilare – LabVIEW
1. Se instalează LabVIEW 2020 Student Edition
2. Se deschide fișierul `LABVIEW_Simulare_Adaptiv_Iluminare_Autovehicul.vi`
3. Dacă apar erori de compatibilitate, se rulează opțiunea `Tools > Advanced > Mass Compile`
4. Se apasă butonul **Run** pentru lansarea simulării
5. Interfața permite testarea comportamentului adaptiv al sistemului de iluminare în funcție de inputuri simulate precum virajul sau viteza.



