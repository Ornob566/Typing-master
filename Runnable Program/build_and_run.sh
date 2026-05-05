#!/usr/bin/env bash
# ============================================================
#  Typing Master MVC - Auto Build Script (Linux / macOS)
# ============================================================
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

echo "============================================================"
echo " Typing Master MVC - Auto Builder"
echo "============================================================"
echo ""

# ── Detect OS ────────────────────────────────────────────────
OS="$(uname -s)"

# ── Install raylib if missing ─────────────────────────────────
if ! pkg-config --exists raylib 2>/dev/null && \
   ! [ -f /usr/local/include/raylib.h ] && \
   ! [ -f /usr/include/raylib.h ]; then

  echo "raylib not found. Installing..."

  if [ "$OS" = "Darwin" ]; then
    if command -v brew &>/dev/null; then
      brew install raylib
    else
      echo "Homebrew not found. Install it from https://brew.sh then re-run."
      exit 1
    fi

  elif [ "$OS" = "Linux" ]; then
    DISTRO=$(grep -oP '(?<=^ID=).+' /etc/os-release | tr -d '"' || true)
    case "$DISTRO" in
      ubuntu|debian|linuxmint|pop)
        sudo apt-get update -qq
        # Try package manager first
        if apt-cache show libraylib-dev &>/dev/null; then
          sudo apt-get install -y libraylib-dev
        else
          # Build from source
          echo "Building raylib 5.0 from source (takes ~1 min)..."
          sudo apt-get install -y build-essential git cmake \
            libgl1-mesa-dev libglu1-mesa-dev \
            libx11-dev libxi-dev libxcursor-dev \
            libxrandr-dev libxinerama-dev libasound2-dev
          git clone --depth 1 --branch 5.0 \
            https://github.com/raysan5/raylib.git /tmp/raylib_src
          cmake -S /tmp/raylib_src -B /tmp/raylib_build \
            -DBUILD_EXAMPLES=OFF -DCMAKE_BUILD_TYPE=Release
          make -C /tmp/raylib_build -j"$(nproc)"
          sudo make -C /tmp/raylib_build install
          rm -rf /tmp/raylib_src /tmp/raylib_build
        fi
        ;;
      fedora|rhel|centos)
        sudo dnf install -y raylib-devel || \
        sudo dnf install -y cmake gcc-c++ mesa-libGL-devel libX11-devel && \
          git clone --depth 1 --branch 5.0 \
            https://github.com/raysan5/raylib.git /tmp/raylib_src && \
          cmake -S /tmp/raylib_src -B /tmp/raylib_build \
            -DBUILD_EXAMPLES=OFF && \
          make -C /tmp/raylib_build -j"$(nproc)" && \
          sudo make -C /tmp/raylib_build install
        ;;
      arch|manjaro)
        sudo pacman -S --noconfirm raylib
        ;;
      *)
        echo "Unknown distro '$DISTRO'. Please install raylib manually."
        exit 1
        ;;
    esac
  fi
fi

# ── Gather compiler flags ────────────────────────────────────
CXXFLAGS="-std=c++17 -O2 -Wall"
INCLUDES=""
LIBS="-lraylib"

# pkg-config is the cleanest path
if pkg-config --exists raylib 2>/dev/null; then
  INCLUDES="$(pkg-config --cflags raylib)"
  LIBS="$(pkg-config --libs raylib)"
fi

# Platform link extras
if [ "$OS" = "Linux" ]; then
  LIBS="$LIBS -lGL -lm -lpthread -ldl -lrt -lX11"
elif [ "$OS" = "Darwin" ]; then
  LIBS="$LIBS -framework OpenGL -framework Cocoa \
        -framework IOKit -framework CoreAudio -framework CoreVideo"
fi

# ── Compile ───────────────────────────────────────────────────
echo ""
echo "Compiling..."
g++ $CXXFLAGS $INCLUDES \
    main.cpp Model.cpp View.cpp Controller.cpp \
    -o TypingMasterMVC \
    $LIBS

echo ""
echo "============================================================"
echo " BUILD SUCCESS!  Launching TypingMasterMVC ..."
echo "============================================================"
echo ""
./TypingMasterMVC
