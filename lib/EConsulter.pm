#!/usr/bin/perl -w

package EConsulter;

use Contavia::DBI;

require Exporter;
@ISA = qw(Exporter);
@EXPORT_OK = qw(ldapinit);

sub new
{
  my $proto = shift;
  my $class = ref($proto) || $proto;
  my $self = {};
  bless ($self, $class);
  return $self;
}

sub dbh
{
  my $self = shift;
  if (@_) { $self->{dbh} = shift }
  return $self->{dbh};
}

sub connect
{
  my $self = shift;
  my $db = shift || $ENV{DATABASE} || 'projekt';
  my $host = $ENV{PGHOST};
  my $user = $ENV{PGUSER};
  my $pass = $ENV{PGPASS};
  $self->{dbh} = Contavia::DBI->connect("dbi:Pg:dbname=$db;host=$host", $user, $pass,
    { RaiseError => 1, AutoCommit => 0 } );
  return $self->{dbh};
}

sub fehler
{
  my $self = shift;
  my $msg = shift;
  die "<fehler>$msg</fehler>";
}

sub translate
{
  my $self = shift;
  my $de   = shift;
  my $n    = shift;

  my $dbh = $self->connect;

  if (defined($n))
  {
    $self->{sth_trans_n} =
      $dbh->prepare( "select en from translate where de = ? and n = ?" )
        unless $self->{sth_trans_n};
    $sth = $self->{sth_trans_n};

    return $de && $dbh->selectrow_array($sth, {}, $de, $n) || $de;
  }
  else
  {
    $self->{sth_trans} = $dbh->prepare( "select en from translate where de = ?" )
      unless $self->{sth_trans};
    $sth = $self->{sth_trans};

    return $de && $dbh->selectrow_array($sth, {}, $de) || $de;
  }
}

sub ldapinit
{
  my $ldapmsg;
  my $ldap = Net::LDAP->new('localhost') or die $@;

  if (-r '/etc/ldap.secret')
  {
    open S, '</etc/ldap.secret' or die $!;
    my $pw = <S>;
    chomp $pw;
    close S;

    $ldapmsg = $ldap->bind( dn => ldapadmin(), password => $pw );
  }
  else
  {
    $ldapmsg = $ldap->bind();
  }

  $ldapmsg->code && die $ldapmsg->error;
  
  return $ldap;
}

sub basedn
{
  return $ENV{BASEDN} || 'ou=e-consulter, o=contavia, c=de';
}

sub ldapadmin
{
  return $ENV{LDAPADMIN} || 'cn=e-consulter, ' . basedn;
}

1;
