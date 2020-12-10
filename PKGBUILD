pkgname=dwm
pkgver=6.2
pkgrel=1
pkgdesc="A dynamic window manager for X"
url="http://dwm.suckless.org"
arch=('i686' 'x86_64')
license=('MIT')
options=(zipman)
depends=('git' 'libx11' 'libxinerama' 'libxft' 'freetype2' 'st' 'dmenu' 'mpd')
install=dwm.install
patchefiles=(dwm-activetagindicatorbar-6.2.diff
	dwm-alwayscenter-20200625-f04cac6.diff
	dwm-attachdirection-20201124-d297fc6.diff
	dwm-cfacts-20200913-61bb8b2.diff
	dwm-cool-autostart-6.2.diff
	dwm-movestack-20201124-bf05c62.diff
	dwm-mpdcontrol-20201125-20d5fe9.diff
	dwm-notitle-20201127-56f5a42.diff
	dwm-pertag-20200914-61bb8b2.diff
	dwm-status2d-20200508-60bb3df.diff)
source=(git+https://git.suckless.org/${pkgname}
	config.h
	dwm.desktop
	${patchefiles[@]})
md5sums=('SKIP'
         '816204ba1269bfed3e81d0cda911010a'
         '939f403a71b6e85261d09fc3412269ee'
         '5a228a44f10b8f78fb46c1d104a1fe1a'
         'caf67004a0fd9fb0368062b4b563301b'
         '6d6af97228e98181210d6e04a89c0bdf'
         'c61a6eecb95bf2e0535334886c6d2ff5'
         'ccf65a2bd68e517b0900c84507dfb552'
         '1a324df93982b8316e082f864395133b'
         'f61faf7e7bde74f4857a1428b656bd1b'
         '2991e0a1f548f099eb23f6fe4f01f5e4'
         'fe418b7cf2e01f59c05595a8336e6020'
         '0ab8adcd2b70f61366668d7c143a5737')

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
