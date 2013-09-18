#!/usr/bin/perl

use strict;
use warnings;
use POSIX qw(strftime);
use HTML::Parser;

my $max = 20;
my $directory = './news-entries';
my $base = "http://www.gnutls.org";
my $self = "$base/news.atom";
my $direct = "$base/news.html";

sub print_author ()
{
print "    <author>\n";
print "      <name>Nikos Mavrogiannopoulos</name>\n";
print "      <email>nmav\@gnutls.org</email>\n";
print "    </author>\n";
}

my $now_string = strftime "%Y-%m-%dT%H:%M:%S+00:00", localtime;

print "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
print "<feed xmlns=\"http://www.w3.org/2005/Atom\">\n";

print "<id>$self</id>\n";
print "<link href=\"$self\" rel=\"self\"/> \n";
print "<title>GnuTLS - News</title>\n";
#print "<subtitle>The latest reports from http://www.gnutls.org</subtitle>\n";
print "<updated>$now_string</updated>\n";
#print_author();

my $date;
my $contents;
my $id;
my $title;

my $mode = '';

require 'scripts/lib-news.pl';

my %title_hash = ();
my %summary_hash = ();

parse_news(\%title_hash, \%summary_hash);

foreach my $key (sort {$b cmp $a} keys %summary_hash) {
	$date = $id = $key;
	$date .= "T00:00:00+00:00";
	$title = $title_hash{$key};
	if (!defined($title) || $title eq '') {
  		$title = "News $id";
	}
	$contents = $summary_hash{$key};

	print "\n  <entry>\n";
	print "    <id>";
	print "$direct#$id";
	print "</id>\n";
	print "    <link rel='alternate' href='$direct#$id'/>\n";
	print "    <title>$title</title>\n";
	print "    <updated>$date</updated>\n";
	print_author();
	print "    <content type='xhtml' xml:base='$base/news-entries/$id.xml'><div xmlns='http://www.w3.org/1999/xhtml'>\n";
	print "$contents\n    </div>\n";
	print "    </content>\n  </entry>\n";

	$max--;
	last if ($max <= 0);
}

print "</feed>\n";
