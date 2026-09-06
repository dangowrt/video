#!/bin/sh
# Generate the pre-built Kodi Python API bindings tarball.
# Usage: generate-python-bindings.sh <kodi-source-dir> <version-name>
# Requires: swig, java (JRE >= 11), curl, unzip, tar, xz
set -eu

SRC="$1"
VERNAME="$2"

GROOVY_VER=4.0.30
COMMONS_LANG_VER=3.20.0
COMMONS_TEXT_VER=1.15.0
MIRROR=https://mirrors.kodi.tv/build-deps/sources

MODULES="AddonModuleXbmcaddon AddonModuleXbmcdrm AddonModuleXbmcgui \
	AddonModuleXbmc AddonModuleXbmcplugin AddonModuleXbmcvfs \
	AddonModuleXbmcwsgi"

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

curl -fsSLo "$TMP/groovy.zip" "$MIRROR/apache-groovy-binary-$GROOVY_VER.zip"
curl -fsSLo "$TMP/lang.tar.gz" "$MIRROR/commons-lang3-$COMMONS_LANG_VER-bin.tar.gz"
curl -fsSLo "$TMP/text.tar.gz" "$MIRROR/commons-text-$COMMONS_TEXT_VER-bin.tar.gz"
unzip -q "$TMP/groovy.zip" -d "$TMP"
tar xzf "$TMP/lang.tar.gz" -C "$TMP"
tar xzf "$TMP/text.tar.gz" -C "$TMP"

CLASSPATH="$TMP/groovy-$GROOVY_VER/lib/*"
CLASSPATH="$CLASSPATH:$TMP/commons-lang3-$COMMONS_LANG_VER/*"
CLASSPATH="$CLASSPATH:$TMP/commons-text-$COMMONS_TEXT_VER/*"
CLASSPATH="$CLASSPATH:$SRC/tools/codegenerator:$SRC/xbmc/interfaces/python"

OUT="$TMP/kodi-python-bindings-$VERNAME"
mkdir -p "$OUT"

for m in $MODULES; do
	swig -w401 -c++ -o "$TMP/$m.i.xml" -xml -I"$SRC/xbmc" \
		"$SRC/xbmc/interfaces/swig/$m.i"
	java \
		--add-opens java.base/java.util=ALL-UNNAMED \
		--add-opens java.base/java.util.regex=ALL-UNNAMED \
		--add-opens java.base/java.io=ALL-UNNAMED \
		--add-opens java.base/java.lang=ALL-UNNAMED \
		--add-opens java.base/java.net=ALL-UNNAMED \
		-cp "$CLASSPATH" groovy.ui.GroovyMain \
		"$SRC/tools/codegenerator/Generator.groovy" \
		"$TMP/$m.i.xml" \
		"$SRC/xbmc/interfaces/python/PythonSwig.cpp.template" \
		"$OUT/$m.i.cpp" >/dev/null
	echo "generated: $m.i.cpp"
done

tar --sort=name --owner=0 --group=0 --numeric-owner \
	--mtime="2025-10-31 00:00Z" \
	-C "$TMP" -cJf "kodi-python-bindings-$VERNAME.tar.xz" \
	"kodi-python-bindings-$VERNAME"
sha256sum "kodi-python-bindings-$VERNAME.tar.xz"
