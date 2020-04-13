#!/usr/bin/perl -w

package EConsulter::LDAP;

require Exporter;
@ISA = qw(Exporter);

use Net::LDAP;
use Carp;
use vars qw($AUTOLOAD);

sub new
{
  my $proto = shift;
  my $host = shift || 'localhost';

  my $class = ref($proto) || $proto;

  my $self = {};
  $self->{ldap} = Net::LDAP->new($host) or die $@;

  bless ($self, $class);
  return $self;
}

sub bind
{
  my $self = shift;
  my %args = @_;

  my $ldapmsg;

  if (!$args{dn} && -r '/etc/ldap.secret')
  {
    open S, '</etc/ldap.secret' or die $!;
    my $pw = <S>;
    chomp $pw;
    close S;

    $args{dn}       = ldapadmin();
    $args{password} = $pw;
  }

  $ldapmsg = $self->{ldap}->bind(%args);
  $ldapmsg->code && die $ldapmsg->error;
  
  return $ldapmsg;
}

sub basedn
{
  return $ENV{BASEDN} || 'ou=e-consulter, o=contavia, c=de';
}

sub ldapadmin
{
  return $ENV{LDAPADMIN} || 'cn=e-consulter, ' . basedn;
}

sub AUTOLOAD
{
  my $self = shift;
  my $type = ref($self) or croak "$self is not an object";
  my $method = $AUTOLOAD;
  $method =~ s/.*://;
  $self->{ldap}->$method(@_);
}

sub DESTROY
{
  $self->{ldap}->unbind if $self->{ldap};
}

1;
