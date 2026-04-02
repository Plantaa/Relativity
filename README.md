# Parábola dos Geomâras no Reino de Emma

## Descrição

Esse programa é destinado como um auxílio para a visualização das coordenadas de pontos em dois planos cartesianos de mesma origem, porém com uma diferença angular.

## Instruções

### Release

Para obter uma cópia do programa pré-compilado, visite a página de Releases desse repositório, e faça o download para sua plataforma.

### Dependências (Linux)

As dependências identificadas para o projeto estão listadas no arquivo _dependencies.txt_. Execute o comando a seguir para instalar-las.

```sh
# Debian / Ubuntu
sudo apt install $(cat dependencies.txt)
```

### CMake

Essa é a forma recomendada, e mais fácil de compilar o programa.

Executar os comandos a seguir na raíz do repositório

```sh
# Configurar
cmake -S . -B build
```

```sh
# Compilar
cmake --build build
```

```sh
# Executar em Linux
./build/ReinoDeEmma

# Executar em Windows
.\build\Debug\ReinoDeEmma.exe
```

### Docker compose

Execute o seguinte comando:

```sh
docker compose up
```

## Agradecimentos

Raylib, como a principal ferramenta que possibilitou o desenvolvimento desse programa.

André Gonçalves, por elucidar a oportunidade para o time de desenvolvimento, e permitir-nos colaborar com a educação e o ensino.
