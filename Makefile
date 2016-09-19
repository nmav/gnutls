# Release process:
#  1. Add a news entry in news-entries (see news/entries/README)
#  2. Updated the documentation ('make web' in the gnutls source)
#  3. Type 'make'
#  4. Type 'make tweet'

#DOC_TAG=gnutls_3_5_4
DOC_TAG=master
WML=wml
WMLFLAGS=-DTABLE_BGCOLOR="\#e5e5e5" -DTABLE_HDCOLOR="\#ccbcbc" \
	-DTABLE_BGCOLOR2="\#e0d7d7" -DWHITE="\#ffffff" -DEMAIL=\"bugs@gnutls.org\" \
	-DSTABLE_VER="3.4" -DSTABLE_OLD_VER="3.3" -DSTABLE_ABI="3.4.0" -DSTABLE_OLD_ABI="3.0.0" \
	-DSTABLE_NEXT_VER="3.5" -DSTABLE_NEXT_ABI="3.5.0" 

COMMON=common.wml bottom.wml head.wml rawnews.wml
OUTPUT=public/index.html public/contrib.html public/devel.html public/support.html	\
 public/download.html public/gnutls-logo.html public/news.html \
 public/documentation.html public/help.html public/openpgp.html \
 public/security.html public/commercial.html public/faq.html \
 public/manual/index.html public/css/layout.css

all: stamp_pages

stamp_layout:
	rm -rf public
	mkdir -p public/css
	mkdir -p public/manual
	mkdir -p public/reference
	cp -ar graphics public/
	cp css/*.css public/css
	touch $@

stamp_pages: stamp_layout $(OUTPUT) public/news.atom stamp_manual
	touch $@

stamp_manual: stamp_layout
	rm -rf gnutls-git
	git clone --branch $(DOC_TAG) . gnutls-git
	cd gnutls-git && \
	git submodule update --init && make autoreconf && \
	./configure --disable-tests --disable-manpages --enable-gtk-doc && \
	make -j4 && make htmldir=../public/ web
	touch $@

.PHONY: clean public/manual/index.html public/security.html

public/manual/index.html: stamp_layout manual-index.html.bak
	@cp -f manual-index.html.bak $@

NEWS_FILES=$(shell ls news-entries/*.xml)

public/news.atom: stamp_layout $(NEWS_FILES) scripts/atom.pl
	PERL5LIB=$$(pwd) perl scripts/atom.pl >$@

public/security.html: security.wml rawsecurity.wml stamp_layout $(COMMON)
	PERL5LIB=$$(pwd) $(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

public/news.html: news.wml $(COMMON) $(NEWS_FILES) stamp_layout
	PERL5LIB=$$(pwd) $(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

public/index.html: gnutls.wml $(COMMON) $(NEWS_FILES) stamp_layout
	PERL5LIB=$$(pwd) $(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

public/%.html: %.wml $(COMMON) stamp_layout
	PERL5LIB=$$(pwd) $(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

public/css/%.css: css/%.cwml $(COMMON) stamp_layout
	PERL5LIB=$$(pwd) $(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

clean:
	rm -f *~ stamp_pages stamp_layout
	rm -rf public
