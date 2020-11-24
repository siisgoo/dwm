pkgname=dwm
pkgver=6.2
pkgrel=1
pkgdesc="A dynamic window manager for X"
url="http://dwm.suckless.org"
arch=('i686' 'x86_64')
license=('MIT')
options=(zipman)
depends=('git' 'libx11' 'libxinerama' 'libxft' 'freetype2' 'st' 'dmenu')
install=dwm.install
patchefiles=(dwm-attachdirection-2020.11.24-d297fc6.diff
	dwm-cfacts-20200913-61bb8b2.diff
	dwm-fakefullscreen-20170508-ceac8c9.diff
	dwm-floatrules-20201124-ed6fed0.diff
	dwm-movestack-20201124-bf05c62.diff
	dwm-mpdcontrol-20201125-20d5fe9.diff
	dwm-pertag-20200914-61bb8b2.diff)
source=(git+https://git.suckless.org/${pkgname}
	config.h
	dwm.desktop
	${patchefiles[@]})
sha256sums=('SKIP'
            '3530114df264f486684b76f65a7991bc25e3d90d94d5a16edcfb465b45e5e311'
            'bc36426772e1471d6dd8c8aed91f288e16949e3463a9933fee6390ee0ccd3f81'
            '55e95ffde7392952ee869ecef3bbf54dc0a84c6cb8e3eced5edaea21e9efc129'
            '1e7082fdf72b4b425a9f7dd2d29b7e33ce9bbe62016c8abaae36afa389d1cf8f'
            '3ab0872f66ca32ccef66e851a07087456a69fad6e7729dbec08f78e7ee93b0f3'
            'c9a78a7b13ad1229af60d3362a593c827651f33b044575ffc070a705d0f9bfb6'
            'af0f657d3f7d998808ae27bb68d6a0a01734cef3a7aee011ca3876eafd9bc26c'
            '72b9f276f0185ccb02da0e7adc029e9fcd0e2fbd4c13e2a2f407dcd99e41c4ea'
            '01cf4b387d2b2aee77e9c6288b5193b2cb5192405d1a9916ea82687152967fd1')

prepare() {
  HASH=('61bb8b2241d4db08bea4261c82e27cd9797099e7')

  cd "$srcdir/$pkgname"
  git reset --hard $HASH
  cp "$srcdir/config.h" config.h
  for patchfile in ${patchefiles[@]}
  do
		  git apply "$startdir/$patchfile"
  done
}

build() {
  cd "$srcdir/$pkgname"
  make X11INC=/usr/include/X11 X11LIB=/usr/lib/X11 FREETYPEINC=/usr/include/freetype2
}

package() {
  cd "$srcdir/$pkgname"
  make PREFIX=/usr DESTDIR="$pkgdir" install
  install -m644 -D LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
  install -m644 -D README "$pkgdir/usr/share/doc/$pkgname/README"
  install -m644 -D "$srcdir/dwm.desktop" "$pkgdir/usr/share/xsessions/dwm.desktop"
}
