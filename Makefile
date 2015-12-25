# Release process:
#  1. Add a news entry in news-entries (see news/entries/README)
#  2. Updated the documentation ('make web' in the gnutls source)
#  3. Type 'make'
#  4. Type 'make tweet'

WML=wml
WMLFLAGS=-DTABLE_BGCOLOR="\#e5e5e5" -DTABLE_HDCOLOR="\#ccbcbc" \
	-DTABLE_BGCOLOR2="\#e0d7d7" -DWHITE="\#ffffff" -DEMAIL=\"bugs@gnutls.org\" \
	-DSTABLE_VER="3.4" -DSTABLE_OLD_VER="3.3" -DSTABLE_ABI="3.4.0" -DSTABLE_OLD_ABI="3.0.0"

#-DSTABLE_NEXT_VER="3.5" 
#-DSTABLE_NEXT_ABI="3.5.0" 

COMMON=common.wml bottom.wml head.wml rawnews.wml
OUTPUT=index.html contrib.html devel.html support.html	\
 download.html gnutls-logo.html news.html future.html	\
 documentation.html help.html openpgp.html \
 security.html commercial.html soc.html faq.html \
 comparison.html admin/bugs.html manual/index.html css/layout.css

all: $(OUTPUT) news.atom
	@for i in news-entries/*.xml;do X=0; if ! test -e $$i.tweet;then X=1;fi;done;if test "$$X" = "1";then echo "There are unsubmitted news. Use 'make tweet'.";fi

release: all
	cd abi-tracker && perl ../../abi-monitor/abi-monitor.pl -get --limit=4 gnutls.json
	cd abi-tracker && perl ../../abi-monitor/abi-monitor.pl -build --limit=8 gnutls.json
	cd abi-tracker && perl abi-tracker -build gnutls.json

.PHONY: clean manual/index.html tweet security.html

manual/index.html: manual/index.html.bak
	@cp -f manual/index.html.bak $@

NEWS_FILES=$(shell ls news-entries/*.xml)

news.atom: $(NEWS_FILES) scripts/atom.pl
	perl scripts/atom.pl >$@

tweet: $(NEWS_FILES)
	perl scripts/tweet.pl

security.html: security.wml rawsecurity.wml $(COMMON)
	$(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

news.html: news.wml $(COMMON) $(NEWS_FILES)
	$(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

index.html: gnutls.wml $(COMMON) $(NEWS_FILES)
	$(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

%.html: %.wml $(COMMON)
	$(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

%.css: %.cwml $(COMMON)
	$(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

clean:
	rm -f *~ $(OUTPUT)
