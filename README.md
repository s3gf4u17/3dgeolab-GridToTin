Statystyki policzone dla fragmentów `78766_1433420_N-34-63-C-c-3-1` (Malbork)

<table width="100%">
  <tr>
  <td width="33%"><img src="https://github.com/s3gf4u17/3dgeolab-GridToTin/assets/86662946/3c068969-294d-47b8-b636-a20b3697c7ac"/></td>
  <td width="33%"><img src="https://github.com/s3gf4u17/3dgeolab-GridToTin/assets/86662946/05e5bb55-fa63-478f-a7bf-e73a834af0b9"/></td>
  <td width="33%"><img src="https://github.com/s3gf4u17/3dgeolab-GridToTin/assets/86662946/1e598fe2-5d5c-4aa6-8ae3-4176fc37adbc"/></td>
  </tr>
</table>

## Spis treści

## Przygotowanie środowiska

Wymagana jest instalacja biblioteki Eigen:

```
sudo apt install libeigen3-dev
```

Biblioteka trafi do jednego z dwóch folderów: `/usr/local/include` lub `/usr/include/`. Możliwe jest sprawdzenie z wykorzystaniem przykładowego testu:

```
ls /usr/local/include | grep eigen
```

Jeżeli wyświetlona zostanie informacja zwrotna, to znaczy że patrzymy na dobry folder. Następnie należy utworzyć symlinki do Eigena:

```
cd <folder z biblioteką Eigen>
sudo ln -sf eigen3/Eigen Eigen
sudo ln -sf eigen3/unsupported unsupported
```

## Kompilacja narzędzi

Narzędzie do konwersji danych z .asc do .xyz:

```
g++ asc2xyz.cpp -o asc2xyz -O3
```

Narzędzie do budowy modelu TIN:

```
cmake -DCGAL_DIR=library/CGAL-5.6 -DBOOST_ROOT=library/boost_1_85_0 -DEIGEN3_INCLUDE_DIR=library/eigen-3.4.0 -DCMAKE_BUILD_TYPE=Release -S . -B build
cd build && make -j8
```

## Wykorzystanie narzędzi

Jeżeli pobrano program z Github releases to należy odblokować uruchamianie:

```
chmod a+wrx asc2xyz xyz2tin
```

Na początku należy wykorzystać narzędzie `asc2xyz`, żeby dane grid z formatu .asc przenieść do chmury punktów w formacie .xyz. Schemat wywołania programu:

```bash
asc2xyz <input> <output> <przyciecie y> <przyciecie x>
```

Przyciecie Y (int) to wiersz do którego zostaną zapisane punkty. Przeciecie X (int) to numer kolumny do tego samego celu. Przykładowo:

```
./asc2xyz data/78766_1433420_N-34-63-C-c-3-1.asc cloud.xyz 1000 1000
```

Następnie można utworzyć model TIN z wykorzystaniem `xyz2tin`:

```
./xyz2tin <input> <output> <COSINE_MAX_ANGLE> <MAX_DISTANCE>
```

Gdzie COSINE_MAX_ANGLE i MAX_DISTANCE to wartości float z przedziału <0.0;1.0>. Przykład:

```
./xyz2tin cloud.xyz tin.obj 0.98 0.5
```

## Licencje

### CGAL

Licencję biblioteki można podejrzeć [tutaj](https://www.cgal.org/license.html)