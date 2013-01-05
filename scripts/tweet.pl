#!/usr/bin/perl

use Net::Twitter;
use Scalar::Util 'blessed';
#use WWW::Shorten 'TinyURL';

if (!-e 'scripts/passwords.pl') {
  print "You need passwords.pl for this script\n";
  exit;
}

require 'scripts/lib-news.pl';

require 'scripts/passwords.pl';

my $nt = Net::Twitter->new(
          traits   => [qw/OAuth API::REST/],
          consumer_key        => $consumer_key,
          consumer_secret     => $consumer_secret,
          access_token        => $token,
          access_token_secret => $token_secret,
          ssl => 1,
);

my %tt1 = (); #contents
my %tt2 = (); #url

fetch_non_tweeted(\%tt1, \%tt2);

foreach my $key (sort {$b cmp $a} keys %tt1) {
	my $contents = $tt1{$key};
	my $lurl = $tt2{$key};
	my $url;
	my $result;

	chomp $contents;
	$contents =~ s/^\s+//;
	$contents =~ s/\s+$//;
	$contents =~ s/\s+/ /g;
	chomp $contents;

   
        #$url = makeashorterlink($lurl);
        $url = $lurl;
        #length of URL is twitter is 20
        my $url_length = 20;
	
	$message = "$contents $url\n";

	if ($message eq ' ') {
          next;
        }

        if (length($message) >= 140) {
          my $t = substr($contents, 0, 140-$url_length-4);
          $message = $t . "... $url";

          print "Updating status to: $message\n";
          $result = $nt->update("$message");
        } else {
          print "Updating status to: $message (" . length($message).")\n";
          $result = $nt->update("$message");
        }

        if ( my $err = $@ ) {
          die $@ unless blessed $err && $err->isa('Net::Twitter::Error');

        warn "HTTP Response Code: ", $err->code, "\n",
         "HTTP Message......: ", $err->message, "\n",
         "Twitter error.....: ", $err->error, "\n";
    }

}
