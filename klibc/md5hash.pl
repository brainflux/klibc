#!/usr/bin/perl
#
# Create a base64 MD5 hash of the input
#

use Digest::MD5;

$ctx = Digest::MD5->new;
$ctx->addfile(*STDIN);

$hash = $ctx->b64digest;

$hash =~ tr/\//_/;		# Slashes not wanted

print $hash, "\n";
