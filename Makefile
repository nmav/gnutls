# Release process:
#  1. Add a news entry in news-entries (see news/entries/README)
#  2. Updated the documentation ('make web' in the gnutls source)
#  3. Type 'make'
#  4. Type 'make tweet'

WML=wml
WMLFLAGS=-DTABLE_BGCOLOR="\#e5e5e5" -DTABLE_HDCOLOR="\#ccbcbc" \
	-DTABLE_BGCOLOR2="\#e0d7d7" -DWHITE="\#ffffff" -DEMAIL=\"bug-gnutls@gnu.org\"
COMMON=common.wml bottom.wml head.wml rawnews.wml
OUTPUT=index.html contrib.html devel.html lists.html	\
 download.html gnutls-logo.html news.html future.html	\
 documentation.html help.html openpgp.html \
 security.html commercial.html soc.html \
 comparison.html admin/bugs.html manual/index.html

all: $(OUTPUT) news.atom
	@for i in news-entries/*.xml;do X=0; if ! test -e $$i.tweet;then X=1;fi;done;if test "$$X" = "1";then echo "There are unsubmitted news. Use 'make tweet'.";fi
#	cvs commit -m "Generated." .

.PHONY: clean manual/index.html tweet

manual/index.html: manual/index.html.bak
	@cp -f manual/index.html.bak $@

NEWS_FILES=$(shell ls news-entries/*.xml)

news.atom: $(NEWS_FILES) scripts/atom.pl
	perl scripts/atom.pl >$@

tweet: $(NEWS_FILES)
	perl scripts/tweet.pl

news.html: news.wml $(COMMON) $(NEWS_FILES)
	$(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

index.html: gnutls.wml $(COMMON) $(NEWS_FILES)
	$(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

%.html: %.wml $(COMMON)
	$(WML) $(WMLFLAGS) $< > $@.tmp
	mv $@.tmp $@

clean:
	rm -f *~ $(OUTPUT)
