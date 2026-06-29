#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>


using namespace std;

class DatasetGenerator {
public:

    static void generarTodo(
        int numPeliculas = 20,
        int numSeries = 10,
        int numUsuarios = 15)
    {
        srand((unsigned)time(nullptr));

        generarPeliculas(numPeliculas);
        generarSeries(numSeries);
        generarTemporadas(numSeries);
        generarUsuarios(numUsuarios);
        generarSuscripciones(numUsuarios);
        generarPerfiles(numUsuarios);
        generarFacturas(numUsuarios);
    }

    static void menuGenerador()
    {

        cout << "\n";
        cout << "=============================================\n";
        cout << "      GENERADOR DE DATASET - NETFLIX\n";
        cout << "=============================================\n";
        cout << "1. Generar dataset completo\n";
        cout << "2. Generar dataset personalizado\n";
        cout << "3. Limpiar archivos\n";
        cout << "0. Continuar al sistema\n";
        cout << "=============================================\n";
        cout << "Opcion: ";

        int op;
        cin >> op;
        cin.ignore();

        switch (op)
        {
        case 1:
            generarTodo();
            cout << "\nDataset generado correctamente.\n";
            break;

        case 2:
        {
            int peliculas, series, usuarios;

            cout << "Cantidad de peliculas (1-50): ";
            cin >> peliculas;

            cout << "Cantidad de series (1-30): ";
            cin >> series;

            cout << "Cantidad de usuarios (1-50): ";
            cin >> usuarios;

            cin.ignore();

            peliculas = max(1, min(50, peliculas));
            series = max(1, min(30, series));
            usuarios = max(1, min(50, usuarios));

            generarTodo(
                peliculas,
                series,
                usuarios);

            cout << "\nDataset personalizado generado.\n";
            break;
        }

        case 3:
            limpiarArchivos();
            cout << "\nArchivos limpiados.\n";
            break;

        default:
            break;
        }
    }

private:

    static const int N_TITULOS_PELICULA = 30;
    static const char* titulosPelicula[N_TITULOS_PELICULA];

    static const int N_TITULOS_SERIE = 20;
    static const char* titulosSerie[N_TITULOS_SERIE];

    static const int N_GENEROS = 7;
    static const char* generos[N_GENEROS];

    static const int N_DIRECTORES = 10;
    static const char* directores[N_DIRECTORES];

    static const int N_PRODUCTORAS = 8;
    static const char* productoras[N_PRODUCTORAS];

    static const int N_IDIOMAS = 5;
    static const char* idiomas[N_IDIOMAS];

    static const int N_PAISES = 6;
    static const char* paises[N_PAISES];

    static const int N_CLASIFICACIONES = 4;
    static const char* clasificaciones[N_CLASIFICACIONES];

    static const int N_PLANES = 3;
    static const char* planes[N_PLANES];
    static const float precios[N_PLANES];

    static const int N_DESCRIPCIONES = 10;
    static const char* descripciones[N_DESCRIPCIONES];

    static const int N_NOMBRES = 15;
    static const char* nombres[N_NOMBRES];

    static int aleatorio(int min, int max)
    {
        return min + rand() % (max - min + 1);
    }

    static string generarEmail(int id)
    {
        return "usuario" + to_string(id) + "@gmail.com";
    }

    static string generarFecha()
    {
        int mes = aleatorio(1, 12);
        int dia = aleatorio(1, 28);

        string m = (mes < 10) ? "0" + to_string(mes) : to_string(mes);
        string d = (dia < 10) ? "0" + to_string(dia) : to_string(dia);

        return "2026-" + m + "-" + d;
    }

    static string generarIdFactura(int n)
    {
        string numero = to_string(n);

        while (numero.size() < 3)
            numero = "0" + numero;

        return "F" + numero;
    }

    //=====================================================
// GENERADORES DE ARCHIVOS
//=====================================================

    static void generarPeliculas(int n)
    {
        ofstream f("peliculas.txt");

        for (int i = 0; i < n; i++)
        {
            int id = 100 + i + 1;

            string titulo = titulosPelicula[i % N_TITULOS_PELICULA];
            string genero = generos[aleatorio(0, N_GENEROS - 1)];
            int anio = aleatorio(1990, 2026);
            int duracion = aleatorio(80, 200);
            string clasificacion = clasificaciones[aleatorio(0, N_CLASIFICACIONES - 1)];
            string director = directores[aleatorio(0, N_DIRECTORES - 1)];
            string productora = productoras[aleatorio(0, N_PRODUCTORAS - 1)];
            string idioma = idiomas[aleatorio(0, N_IDIOMAS - 1)];
            string pais = paises[aleatorio(0, N_PAISES - 1)];

            f << id << "|"
                << titulo << "|"
                << genero << "|"
                << anio << "|"
                << duracion << "|"
                << clasificacion << "|"
                << director << "|"
                << productora << "|"
                << idioma << "|"
                << pais << "\n";
        }

        f.close();

        cout << "Peliculas generadas: "
            << n << endl;
    }

    static void generarSeries(int n)
    {
        ofstream f("series.txt");

        for (int i = 0; i < n; i++)
        {
            int id = i + 1;

            string titulo = titulosSerie[i % N_TITULOS_SERIE];
            string genero = generos[aleatorio(0, N_GENEROS - 1)];
            string descripcion = descripciones[aleatorio(0, N_DESCRIPCIONES - 1)];
            string clasificacion = clasificaciones[aleatorio(0, N_CLASIFICACIONES - 1)];

            f << id << "|"
                << titulo << "|"
                << genero << "|"
                << descripcion << "|"
                << clasificacion << "\n";
        }

        f.close();

        cout << "Series generadas: "
            << n << endl;
    }

    static void generarTemporadas(int numSeries)
    {
        ofstream f("temporadas.txt");

        for (int serie = 1; serie <= numSeries; serie++)
        {
            int cantidadTemporadas = aleatorio(1, 5);
            int anioBase = aleatorio(2010, 2023);

            for (int t = 1; t <= cantidadTemporadas; t++)
            {
                f << serie << "|"
                    << t << "|"
                    << anioBase + t - 1
                    << "\n";
            }
        }

        f.close();

        cout << "Temporadas generadas." << endl;
    }

    static void generarUsuarios(int n)
    {
        ofstream f("usuarios.txt");

        for (int i = 1; i <= n; i++)
        {
            string email = generarEmail(i);
            string password = "pass" + to_string(1000 + i);

            f << email
                << "|"
                << password
                << "\n";
        }

        f.close();

        cout << "Usuarios generados: "
            << n << endl;
    }

    static void generarSuscripciones(int n)
    {
        ofstream f("suscripciones.txt");

        for (int i = 1; i <= n; i++)
        {
            int plan = aleatorio(0, N_PLANES - 1);

            f << i << "|"
                << planes[plan] << "|"
                << fixed << setprecision(2)
                << precios[plan] << "|"
                << generarFecha() << "|"
                << generarEmail(i)
                << "\n";
        }

        f.close();

        cout << "Suscripciones generadas: "
            << n << endl;
    }

    static void generarPerfiles(int n)
    {
        ofstream f("perfiles.txt");

        for (int i = 1; i <= n; i++)
        {
            string nombre = nombres[(i - 1) % N_NOMBRES];
            int infantil = aleatorio(0, 4) == 0;

            f << i << "|"
                << nombre << "|"
                << infantil << "|"
                << generarEmail(i)
                << "\n";
        }

        f.close();

        cout << "Perfiles generados: "
            << n << endl;
    }

    static void generarFacturas(int n)
    {
        ofstream f("facturas.txt");

        for (int i = 1; i <= n; i++)
        {
            int plan = aleatorio(0, N_PLANES - 1);

            f << generarIdFactura(i) << "|"
                << generarFecha() << "|"
                << fixed << setprecision(2)
                << precios[plan]
                << "|Pendiente\n";
        }

        f.close();

        cout << "Facturas generadas: "
            << n << endl;
    }

    static void limpiarArchivos()
    {
        const char* archivos[] =
        {
            "peliculas.txt",
            "series.txt",
            "temporadas.txt",
            "usuarios.txt",
            "suscripciones.txt",
            "perfiles.txt",
            "facturas.txt"
        };

        for (int i = 0; i < 7; i++)
        {
            ofstream f(archivos[i], ios::trunc);
            f.close();
        }
    }
};

//=====================================================
// DEFINICIÓN DE LOS ARRAYS ESTÁTICOS
//=====================================================

const char* DatasetGenerator::titulosPelicula[N_TITULOS_PELICULA] = {
    "Inception",
    "Interstellar",
    "The Godfather",
    "Parasite",
    "Mad Max Fury Road",
    "The Grand Budapest Hotel",
    "Avengers Endgame",
    "The Dark Knight",
    "Pulp Fiction",
    "Forrest Gump",
    "The Matrix",
    "Gladiator",
    "Schindler List",
    "The Lion King",
    "Jurassic Park",
    "Titanic",
    "Avatar",
    "The Silence of the Lambs",
    "Fight Club",
    "Goodfellas",
    "The Shawshank Redemption",
    "Joker",
    "Oppenheimer",
    "Barbie",
    "Top Gun Maverick",
    "Spider Man No Way Home",
    "Doctor Strange",
    "Black Panther",
    "The Batman",
    "Everything Everywhere"
};

const char* DatasetGenerator::titulosSerie[N_TITULOS_SERIE] = {
    "Stranger Things",
    "Breaking Bad",
    "Dark",
    "Squid Game",
    "The Crown",
    "Money Heist",
    "Game of Thrones",
    "The Witcher",
    "Ozark",
    "Narcos",
    "Black Mirror",
    "Peaky Blinders",
    "The Boys",
    "Severance",
    "Ted Lasso",
    "Succession",
    "Chernobyl",
    "Mindhunter",
    "House of Cards",
    "Better Call Saul"
};

const char* DatasetGenerator::generos[N_GENEROS] = {
    "Accion",
    "Drama",
    "Sci-Fi",
    "Thriller",
    "Comedia",
    "Terror",
    "Documental"
};

const char* DatasetGenerator::directores[N_DIRECTORES] = {
    "Christopher Nolan",
    "Martin Scorsese",
    "Steven Spielberg",
    "Bong Joon-ho",
    "Quentin Tarantino",
    "Denis Villeneuve",
    "George Miller",
    "Wes Anderson",
    "James Cameron",
    "David Fincher"
};

const char* DatasetGenerator::productoras[N_PRODUCTORAS] = {
    "Warner Bros",
    "Paramount",
    "Universal",
    "Sony Pictures",
    "20th Century Studios",
    "Netflix Originals",
    "Amazon Studios",
    "A24"
};

const char* DatasetGenerator::idiomas[N_IDIOMAS] = {
    "Ingles",
    "Espanol",
    "Frances",
    "Coreano",
    "Aleman"
};

const char* DatasetGenerator::paises[N_PAISES] = {
    "EE.UU.",
    "Reino Unido",
    "Francia",
    "Corea del Sur",
    "Alemania",
    "Australia"
};

const char* DatasetGenerator::clasificaciones[N_CLASIFICACIONES] = {
    "ATP",
    "+7",
    "+13",
    "+16"
};

const char* DatasetGenerator::planes[N_PLANES] = {
    "Basico",
    "Estandar",
    "Premium"
};

const float DatasetGenerator::precios[N_PLANES] = {
    24.90f,
    35.90f,
    45.90f
};

const char* DatasetGenerator::descripciones[N_DESCRIPCIONES] = {
    "Un grupo de jovenes enfrenta misterios sobrenaturales.",
    "Una historia de poder, traicion y redencion.",
    "Viajes en el tiempo y secretos familiares oscuros.",
    "Personas desesperadas compiten por su vida en juegos mortales.",
    "La historia de una familia que oculta un gran secreto.",
    "Un genial estratega planea el golpe perfecto.",
    "El ascenso y caida de un imperio criminal.",
    "Tecnologia y humanidad colisionan con consecuencias inesperadas.",
    "Un detective brillante resuelve crimenes imposibles.",
    "La lucha por la supervivencia en un mundo postapocaliptico."
};

const char* DatasetGenerator::nombres[N_NOMBRES] = {
    "Carlos",
    "Maria",
    "Luis",
    "Ana",
    "Pedro",
    "Sofia",
    "Jorge",
    "Lucia",
    "Miguel",
    "Valentina",
    "Diego",
    "Camila",
    "Andres",
    "Isabella",
    "Sebastian"
};