# HTTP Client - Management Film Server

**Autor: Panait Marian-Alexandru**

Acest proiect reprezintă realizarea unui client scris în C care comunică cu un server HTTP pentru a gestiona utilizatori, filme și colecții de filme. Comunicarea se realizează prin API-ul de socket-uri in limbajul C.

## 📋 Funcționalități implementate

Clientul interpretează comenzi introduse de la tastatură și trimite cereri către server în format JSON (dacă este cazul). Răspunsurile serverului sunt afișate în consolă, conform convențiilor de mesaje.

## 📜 Comenzi disponibile

Clientul suportă următoarele comenzi:

*   `exit`: Închide clientul.
*   `login_admin`: Autentifică un administrator. Cere `username` și `password`.
*   `add_user`: Adaugă un utilizator nou. Necesită autentificare ca admin. Cere `username` și `password` pentru noul utilizator.
*   `get_users`: Afișează lista utilizatorilor. Necesită autentificare ca admin.
*   `delete_user`: Șterge un utilizator. Necesită autentificare ca admin. Cere `username`-ul utilizatorului de șters.
*   `logout_admin`: Deconectează administratorul.
*   `login`: Autentifică un utilizator. Cere `admin_username`, `username` și `password`.
*   `get_access`: Obține un token JWT pentru acces la bibliotecă. Necesită autentificare ca utilizator.
*   `get_movies`: Afișează lista tuturor filmelor din bibliotecă. Necesită token JWT.
*   `get_movie`: Afișează detalii despre un film specific. Necesită token JWT. Cere `id`-ul filmului.
*   `add_movie`: Adaugă un film nou în bibliotecă. Necesită token JWT. Cere `title`, `year`, `description`, `rating`.
*   `logout`: Deconectează utilizatorul.
*   `update_movie`: Actualizează detaliile unui film existent. Necesită token JWT. Cere `id`-ul filmului și noile `title`, `year`, `description`, `rating`.
*   `delete_movie`: Șterge un film din bibliotecă. Necesită token JWT. Cere `id`-ul filmului.
*   `get_collections`: Afișează lista colecțiilor de filme. Necesită token JWT.
*   `get_collection`: Afișează detalii despre o colecție specifică. Necesită token JWT. Cere `id`-ul colecției.
*   `add_collection`: Creează o nouă colecție de filme. Necesită token JWT. Cere `title`-ul colecției și `num_movies`, urmat de `movie_id`-urile corespunzătoare.
*   `delete_collection`: Șterge o colecție de filme. Necesită token JWT și să fii proprietarul colecției. Cere `id`-ul colecției.
*   `add_movie_to_collection`: Adaugă un film într-o colecție existentă. Necesită token JWT și să fii proprietarul colecției. Cere `collection_id` și `movie_id`.
*   `delete_movie_from_collection`: Șterge un film dintr-o colecție. Necesită token JWT și să fii proprietarul colecției. Cere `collection_id` și `movie_id`.

## ⚙️ Detalii de Implementare

Clientul funcționează într-o buclă `while` care așteaptă comenzi de la utilizator. Fiecare comandă este procesată printr-o serie de structuri `if-else if`.

Pentru fiecare comandă:
1.  Se citesc datele necesare de la utilizator (de exemplu, username, password, ID-uri etc.).
2.  Se construiește un payload JSON, dacă este necesar, folosind biblioteca `parson`.
3.  Se formează cererea HTTP corespunzătoare (GET, POST, PUT, DELETE) utilizând funcțiile din `requests.c`.
4.  Cererea este trimisă la server prin funcția `send_to_server`.
5.  Răspunsul serverului este primit prin `receive_from_server`.
6.  Răspunsul este parsat (dacă este JSON) și se extrag informațiile relevante.
7.  Se afișează un mesaj de succes sau eroare către utilizator.

### Managementul Cookie-urilor și Token-urilor

Clientul gestionează cookieurile de sesiune și token-urile JWT pentru autentificare și autorizare:

*   **Cookie-uri generale**: Sunt stocate în `cookies[0]`. Acestea sunt extrase din răspunsurile serverului (din header-ul `Set-Cookie`) și trimise în cererile ulterioare.
*   **Token JWT**: Este stocat în `cookies[1]`. Token-ul este obținut după comanda `get_access` și este necesar pentru majoritatea operațiunilor legate de filme și colecții.
*   Clientul este configurat să poată stoca până la `MAX_COOKIES` (definit ca 10), deși în implementarea curentă se utilizează în principal primele două locații pentru cookie-ul de sesiune și token-ul JWT.

### Construirea Payload-ului JSON

Pentru comenzile care necesită trimiterea de date în format JSON (ex: `login_admin`, `add_user`, `add_movie`), clientul utilizează două matrici de caractere:

*   `matrix`: Această matrice stochează valorile datelor care trebuie incluse în JSON (de exemplu, username-ul, parola, titlul filmului etc.).
*   `matrix_names`: Această matrice stochează numele câmpurilor corespunzătoare valorilor din `matrix` (de exemplu, "username", "password", "title").

După ce datele sunt citite de la utilizator și populate în aceste matrici, funcția `json_builder(matrix, matrix_names)` este apelată. Această funcție parcurge cele două matrici și construiește string-ul JSON corespunzător, utilizând biblioteca `parson`. String-ul JSON rezultat este apoi inclus în corpul cererii POST sau PUT.

