# Serwer wielowątkowy – dokumentacja


## Opis Problemu
W środowiskach sieciowych często zachodzi potrzeba komunikacji wielu klientów w czasie rzeczywistym, np. w aplikacjach czatu, grach online, systemach pomocy technicznej czy współdzielonych narzędziach. Kluczowym wyzwaniem jest umożliwienie wielu użytkownikom jednoczesnej wymiany wiadomości bez wzajemnego blokowania się oraz bez utraty danych. Tradycyjne podejście oparte na pojedynczym wątku nie sprawdza się w sytuacjach, gdy liczba klientów rośnie, ponieważ każde oczekiwanie na dane blokuje pozostałych.

Celem projektu było stworzenie prostego serwera TCP, który będzie w stanie równocześnie obsługiwać wielu klientów bez utraty wydajności, oraz klienta, który umożliwia użytkownikowi łatwe dołączenie do rozmowy. W tym celu wykorzystano mechanizmy wielowątkowości i synchronizacji w języku Python.

## Cel

Projekt implementuje prosty, wielowątkowy **serwer czatowy TCP** oraz klienta CLI w Pythonie (wyłącznie standardowa biblioteka). Aplikacja pozwala wielu użytkownikom na równoczesną komunikację w sieci LAN bez pośredników.

## Architektura

* **Gniazdo nasłuchujące**: `socket.AF_INET`/`SOCK_STREAM` nasłuchuje na `0.0.0.0:12345`.
* **Pula klientów**: tablice `clients` i `usernames` przechowują gniazda oraz nazwy użytkowników.
* **Sekcje krytyczne**: dostęp do struktur współdzielonych chroniony jest przez `threading.Lock`.
* **Obsługa połączeń** – wątek `handle_client`:

  1. odbiera nazwę użytkownika;
  2. rejestruje klienta w puli;
  3. retransmituje komunikaty do pozostałych użytkowników (`broadcast_msg`);
  4. usuwa klienta po rozłączeniu.

Model **„one thread per client”** zapewnia pełną równoległość pracy kosztem liniowego zużycia zasobów.

## Protokół

```
client → server : <username>\n
client → server : <payload UTF‑8 ≤1024 B>
server → clients≠sender : "<username>: " + payload
```

Brak nagłówków sprawia, że do testów wystarczy np. `netcat`.

## Uruchomienie

```bash
python server.py   # start serwera
python client.py   # uruchomienie klienta i podanie nazwy
```

Adres hosta i port można zmienić w nagłówkach plików.

### Wymagania środowiskowe

* Python ≥ 3.8
* Dostęp do portu 12345/TCP


