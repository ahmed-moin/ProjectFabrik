#!/usr/bin/perl -w

use strict;


foreach (@ARGV)
{
  my $n = $_;

  open SZ, ">statusziel${n}" or die $!;
  print SZ "<& statusziel, %ARGS, zid => $n &>\n";
  close SZ;

  open SZK, ">statusziel${n}_kosten" or die $!;
  print SZK "<& statusziel_kosten, %ARGS, zid => $n &>\n";
  close SZK;

  open SZT, ">statusziel${n}_tabelle" or die $!;
  print SZT "<& statusziel_tabelle, %ARGS, zid => $n &>\n";
  close SZT;
}
