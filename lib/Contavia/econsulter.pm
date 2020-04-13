#!/usr/bin/perl -w

package Contavia::econsulter;

sub new
{
  my $proto = shift;
  my $class = ref($proto) || $proto;
  my $self = {};
  $self->{dbh} = shift;
  bless ($self, $class);
  return $self;
}

sub projekte
{
  my $self = shift;
  my $projekte = $self->{projekte};
  unless ($projekte)
  {
    my $dbh = $self->{dbh};
    my $prj = $dbh->selectall_arrayref( <<"EOF" );
     select prj_id, prj_titel
       from projekt
EOF
    $projekte = {}
    foreach (@$prj)
    {
      my ($prj_id, $prj_titel) = @$_;
      $projekte
    }
  }
}

1;
