#use lib qw(/u/e-consulter/lib /u/e-consulter_dev/lib/ /home/httpd/perl);
use lib qw(/var/www/e-consulter.com/www/lib /u/e-consulter_dev/lib/);

# enable if the mod_perl 1.0 compatibility is needed
#use Apache2::compat ();

use ModPerl::Util (); #for CORE::GLOBAL::exit

use Apache2::RequestRec ();
use Apache2::RequestIO ();
use Apache2::RequestUtil ();

use Apache2::ServerRec ();
use Apache2::ServerUtil ();
use Apache2::Connection ();
use Apache2::Log ();

use APR::Table ();

use ModPerl::Registry ();

use Apache2::Const -compile => ':common';
use APR::Const -compile => ':common';

1;
