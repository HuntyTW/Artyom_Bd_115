#!/usr/bin/env bash

# Сборка проекта на Linux. Запуск: ./build.sh (или bash build.sh)
set -e

SOURCE_FOLDER=.
IMG_FOLDER=img

# Ninja, если он установлен, иначе обычные Makefile
if command -v ninja > /dev/null 2>&1; then
	BUILD_TYPE=Ninja
	BUILD_SUFFIX=ninja
else
	BUILD_TYPE="Unix Makefiles"
	BUILD_SUFFIX=make
fi

BUILD_FOLDER=build_${BUILD_SUFFIX}

# Работаем из папки, в которой лежит сам скрипт
cd "$(dirname "$0")"

mkdir -p "${BUILD_FOLDER}"

cd "${BUILD_FOLDER}"

cmake -G "${BUILD_TYPE}" "../${SOURCE_FOLDER}"
cmake --build .

mkdir -p "${IMG_FOLDER}"
cp "../${IMG_FOLDER}/grustnii-smail.png" "${IMG_FOLDER}/"
cp "../${IMG_FOLDER}/veselii-smail.png" "${IMG_FOLDER}/"

cd ..

echo
echo "Готово. Запуск: cd ${BUILD_FOLDER} && ./keyboard"
