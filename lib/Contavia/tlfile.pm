=head1 NAME

Contavia::tlfile - trusted local file access

=head1 SYNOPSIS

=head1 DESCRIPTION

=head1 AUTHOR

Copyright 2001, Tommi Mäkitalo, Contavia AG

=cut

package Contavia::tlfile;

use strict;
use IPC::Open3;
#require Exporter;
#@ISA = qw(Exporter);
#@EXPORT = qw(tlfile);

my $tlfile = $ENV{TLFILE} || '/u/e-consulter/bin/tlfile';

sub new
{
  shift;
  my $uid = shift;
  my $self = { uid => $uid };
  bless($self);
  return $self;
}

sub execreadcmd
{
  my $self = shift; 
  #my $uid = $self->{uid};
  my $uid = "e-consulter";
  my $cmd = shift;
  my $path= shift;
  $path = "daten/$path";
  my $pid = open3(\*WTRFH, \*RDRFH, \*ERRFH, $tlfile, $uid, $cmd, $path);
  close WTRFH;

  local $/;
  undef $/;
  my $d = <RDRFH>;
  close RDRFH;

  my $err = <ERRFH>;
  die "<fehler>$err [$cmd, $path]</fehler>" if $err;

  return $d;
}

sub dir
{
  my $self = shift;
  return $self->execreadcmd('dir', @_);
}

sub ldir
{
  my $self = shift;
  return $self->execreadcmd('ldir', @_);
}

sub ls
{
  my $self = shift;
  return $self->execreadcmd('ls', @_);
}

sub get
{
  my $self = shift;
  return $self->execreadcmd('get', @_);
}

sub put
{
  my $self = shift; 
  my $uid = $self->{uid};
  my $fname = shift;
  my $data = shift;

  $fname =~ s/'/''/g;
  open FH, "|$tlfile e-consulter put '$fname'";
  print FH $data;
  close FH;
}

sub mkdir
{
  my $self = shift;
  $self->execreadcmd('mkdir', @_);
}

sub rmdir
{
  my $self = shift;
  $self->execreadcmd('rmdir', @_);
}

sub del
{
  my $self = shift;
  $self->execreadcmd('del', @_);
}

