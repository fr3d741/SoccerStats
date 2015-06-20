#!/usr/bin/perl
use strict;
use warnings;
use LWP::Simple;

my $file = $ARGV[1];
my $content = get($ARGV[0]) or die 'Unable to get page';

open FH, ">$file" or die "Can't open file!\n";

print FH $content;
close FH;

print "Done."
