#!/usr/bin/perl
#
# Create a base64 MD5 hash of the input
#

use Digest::MD5;

$ctx = Digest::MD5->new;
$ctx->addfile(*STDIN);

print $ctx->b64digest, "\n";
