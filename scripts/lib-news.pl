use strict;
use warnings;
use POSIX qw(strftime);
use HTML::Parser;

my $s_refhash;
my $t_refhash;

my $directory = './news-entries';
#my $base = "http://www.gnu.org/software/gnutls";
#my $self = "$base/news.atom";
#my $direct = "$base/news.html";

my $mode = '';
my $date ='';

sub start_handler
{
  my $tagname = shift;
  my $rtext = shift;
  if ($tagname ne "title") {
    $s_refhash->{$date} .= $rtext;
    return;
  }
  $mode = 'title';
}

sub text_handler
{
  my $txt = shift;
  
  if ($mode eq 'title') {
    $t_refhash->{$date} .= $txt;
    return;
  }
  
  $s_refhash->{$date} .= $txt;
}

sub end_handler
{
  my $tagname = shift;
  my $rtext = shift;
  if ($tagname eq "title") {
    $mode = '';
    return;
  }

  $s_refhash->{$date} .= $rtext;
}

#input is one reference to a title hash and a reference to summary hash.

sub parse_news
{
  my @c;
  
  $t_refhash = $_[0];
  $s_refhash = $_[1];

  opendir (DIR, $directory) or die $!;
  
  while (my $file = readdir(DIR)) {
	next if ($file =~ m/^\./);
	next if ($file =~ m/~/);
	next if (-d "$directory/$file");
	$file =~ m/(.*).xml$/;
	$date = $1;
	next if (!defined($date) || $date eq '');

        @c = ();

        my $p = HTML::Parser->new(api_version => 3);
        $p->handler( start => \&start_handler, "tagname,text,self");
        $p->handler( text=> \&text_handler, "dtext,self");
        $p->handler( end => \&end_handler, "tagname,text,self");
        $p->parse_file("$directory/$file") || die $!;
  }

  closedir DIR;
}

sub start_tweet_handler
{
  my $tagname = shift;
  my $rtext = shift;
  my $attr_ref = shift;

  if ($tagname eq "a" && defined($attr_ref)) {
    $t_refhash->{$date} = $attr_ref->{"href"};
    return;
  }

  if ($tagname ne "title" && defined($rtext)) {
    $s_refhash->{$date} .= $rtext;
    return;
  }

  $mode = 'title';
}

sub text_tweet_handler
{
  my $txt = shift;

  if ($mode ne 'title') {
    $s_refhash->{$date} .= $txt;
    return;
  }
}

sub end_tweet_handler
{
  my $tagname = shift;
  my $rtext = shift;
  if ($tagname eq "title") {
    $mode = '';
    return;
  }

  if (defined($rtext)) {
    $s_refhash->{$date} .= $rtext;
  }
}


#input is a hash for summary and a hash for URLs
sub fetch_non_tweeted
{
  my @c;
  
  $s_refhash = $_[0];
  $t_refhash = $_[1];

  opendir (DIR, $directory) or die $!;
  
  while (my $file = readdir(DIR)) {
	next if ($file =~ m/^\./);
	next if ($file =~ m/~/);
	next if (-d "$directory/$file");
	next if (-e "$directory/$file.tweet");
	$file =~ m/(.*).xml$/;
	$date = $1;
	next if (!defined($date) || $date eq '');

        @c = ();

        my $p = HTML::Parser->new(api_version => 3);
        $p->handler( start => \&start_tweet_handler, "tagname,dtext,attr,self");
        $p->handler( text=> \&text_tweet_handler, "dtext,self");
        $p->handler( end => \&end_tweet_handler, "tagname,dtext,self");
        $p->parse_file("$directory/$file") || die $!;
        
        system("touch $directory/$file.tweet");
        system("git add $directory/$file.tweet");
  }

  closedir DIR;
}

1;

