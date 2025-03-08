#  Filtriranje sa KFR

Ovaj projekat prikazuje kako koristiti KFR biblioteku za čitanje, obrada i filtriranje stereo WAV fajlova. Proces obuhvata čitanje zvučnih podataka, ekstrakciju kanala, primenu filtriranja i upisivanje izlaznih fajlova.

## Razrada Koda

### 1. Čitanje Audia:

- Kod koristi `kfr::audio_reader_wav` da pročita stereo WAV fajl (dva kanala) u `univector2d<float>`, što je dvodimenzionalni vektor gde svaki red predstavlja jedan kanal.
- Ispisuje osnovne informacije o fajlu, kao što su: brzina uzorkovanja, broj kanala, dužina fajla i drugi metapodaci.

### 2. Ekstrakcija Kanala:

- Kod uzima levi i desni kanal iz 2D vektora i smešta ih u `univector<float>`.
- Proverava da li veličina oba kanala odgovara dužini originalnog fajla.

### 3. Normalizacija:

- Kanali se normalizuju tako što se svaki uzorak podeli sa maksimalnom apsolutnom vrednošću uzorka, kako bi se sprečilo preslučivanje prilikom obrade.

### 4. Pisanje Kanala:

- Nakon normalizacije, levi i desni kanali se upisuju u posebne WAV fajlove koristeći `audio_writer_wav`.

### 5. Filtriranje:

- Kreira se bandpass IIR filter (Butterworth, reda 4) koji se primenjuje na levi kanal.
- Impulsni odgovor filtra se koristi za primenu filtra na podatke levog kanala pomoću `iir` funkcije iz KFR biblioteke.

### 6. Izlazni Podaci:

- Filtrirani podaci se ponovo normalizuju i upisuju u novi WAV fajl.

---

## CMakeLists

Fajlovi vezani za **SND_LIBRARY** nisu potrebni. Uz upustva sa zvaničnog sajta KFR biblioteke ([KFR Installation Guide](https://www.kfrlib.com/docs/latest/installation/)), dodat je sledeći deo za povezivanje sa potrebnim KFR bibliotekama:

```cmake
target_link_libraries(PROJECT PUBLIC 
    kfr_dsp_avx2 
    kfr_dsp_avx512 
    kfr_dsp_avx 
    kfr_dsp_sse2 
    kfr_dsp_sse41 
    kfr_io
)
